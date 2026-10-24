### Sheridan Makefile ###
#
# Builds the Quake 2 game library. Settings can be overridden on the command
# line or in a file named ".config" next to this Makefile.
#
#   make                      build for this machine's architecture
#   make CONFIG_32BIT=1       build a 32-bit library (gamei386.so)
#   make CONFIG_64BIT=1       build a 64-bit library (gamex86_64.so on x86)
#   make CONFIG_WINDOWS=1     cross-compile gamex86.dll with MinGW
#   make CONFIG_WINDOWS=1 CONFIG_64BIT=1
#                             cross-compile gamex86_64.dll with MinGW
#   make V=1                  show the full compiler commands
#
# Output goes in build-<cpu>/ (or build-win-<cpu>/), so "make clean" just
# removes that directory.

-include .config

ifdef CONFIG_32BIT
ifdef CONFIG_64BIT
    $(error CONFIG_32BIT and CONFIG_64BIT can't both be set)
endif
endif

ifndef CPU
    CPU := $(shell uname -m | sed -e s/i.86/i386/ -e s/amd64/x86_64/ -e s/sun4u/sparc64/ -e s/aarch64/arm64/ -e s/armv7l/arm/ -e s/sa110/arm/ -e s/alpha/axp/)
endif

CC ?= gcc
STRIP ?= strip
RM ?= rm -f

# The engine finds the game library through GetGameAPI, which is marked
# visibility("default") in g_main.c, so everything else can stay hidden.
# NO_ZLIB: q_shared.h includes zlib.h, but nothing in the mod uses it.
CFLAGS ?= -O2 -g -fno-strict-aliasing -Wall -Wno-missing-braces
CFLAGS += -MMD -DNO_ZLIB -DC_ONLY
LDFLAGS ?=
LDFLAGS += -shared
LIBS ?=

ifdef CONFIG_WINDOWS
    ifdef CONFIG_64BIT
        CPU := x86_64
        MINGW ?= x86_64-w64-mingw32
    else
        CPU := x86
        MINGW ?= i686-w64-mingw32
    endif
    CC := $(MINGW)-gcc
    STRIP := $(MINGW)-strip
    EXT := dll
    BUILDDIR ?= build-win-$(CPU)
    # q_shared.h has MSVC-only pragmas that GCC doesn't know.
    CFLAGS += -Wno-unknown-pragmas
    # Don't depend on the MinGW runtime DLLs being installed on the server.
    LDFLAGS += -static-libgcc -Wl,--nxcompat,--dynamicbase
    DEF := game.def
else
    CFLAGS += -fPIC -fvisibility=hidden
    LIBS += -lm
    EXT := so
    ifdef CONFIG_32BIT
        CFLAGS += -m32
        LDFLAGS += -m32
        CPU := i386
    endif
    ifdef CONFIG_64BIT
        CFLAGS += -m64
        LDFLAGS += -m64
        ifeq ($(CPU),i386)
            CPU := x86_64
        endif
    endif
endif

BUILDDIR ?= build-$(CPU)
TARGET ?= game$(CPU).$(EXT)

SRCDIR := src
SRCS := $(notdir $(wildcard $(SRCDIR)/*.c))
OBJS := $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
OUTPUT := $(BUILDDIR)/$(TARGET)

all: $(OUTPUT)

default: all

.PHONY: all default clean strip

# Define V=1 to show command line.
ifdef V
    Q :=
    E := @true
else
    Q := @
    E := @echo
endif

-include $(OBJS:.o=.d)

$(BUILDDIR):
	$(Q)mkdir -p $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(E) [CC] $@
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(OUTPUT): $(OBJS) $(DEF)
	$(E) [LD] $@
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJS) $(DEF) $(LIBS)

clean:
	$(E) [CLEAN] $(BUILDDIR)
	$(Q)$(RM) -r $(BUILDDIR)

strip: $(OUTPUT)
	$(E) [STRIP] $(OUTPUT)
	$(Q)$(STRIP) $(OUTPUT)
