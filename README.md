# Sheridan

Sheridan is a Quake 2 deathmatch mod by Wheet, run on the TastySpleen (TS)
servers. It is standard Quake 2 deathmatch with a focus on **kill streaks**:
the scoreboard tracks who has the longest run of kills without dying, both
for the current map and all time. On top of that it changes a few weapons,
adds a melee spinkick and protects players for a moment after they respawn.

## Gameplay

### Scoring and streaks

Frags work as in normal deathmatch: +1 for a kill, -1 for a suicide or a
teamkill. In addition:

- **Streak:** every kill adds 1 to your streak. Dying in any way, including
  suicide, resets it to 0. Moving to spectator or disconnecting also clears it.
- The scoreboard shows three records above the player list:
  - **Streak:** the longest streak alive on the current map, and who has it.
  - **HighStreak:** the longest streak ever reached on this server.
  - **HighScore:** the highest frag count ever reached on this server.
- The all-time records are saved to disk at the end of every map and loaded
  when the server starts, so they last across maps and restarts.
- At the end of each map, the top 5 players by frags per hour are printed to
  everyone's console, with their minutes played and frags.

In short: get kills and stay alive. A long streak puts your name on the
scoreboard for everyone to see, and makes you the target.

### Weapon changes

| Weapon | Change |
|---|---|
| Blaster | Fires an instant-hit laser beam instead of a slow bolt. Same damage (15). |
| HyperBlaster | Also fires instant-hit laser beams instead of bolts. Same damage (15 per shot). |
| BFG10K | Much weaker. The ball does 110 damage on a direct hit (was 200), its splash radius is 55 (was 100), and its lasers do 1 damage per tick (was 5). |

All other weapons are unchanged.

### Spinkick

Players have a melee kick (see [Player commands](#player-commands)). It
reaches about 75 units in front of you. Against a player it does 20 damage
and a very large knockback, enough to send them off a ledge or into lava.
Against anything else it does 60 damage. There is a short cooldown, and
pressing kick again during the cooldown makes it longer, so mashing the key
doesn't help.

### Spawn protection

- After respawning you glow yellow and can't be hurt for 2 seconds
  (`tune_spawn_prot_time`).
- Firing your weapon ends the protection early (`tune_spawn_attack_unprot`).
- **Spawn camping with the railgun backfires.** Railing a player during the
  first 75% of their protection (`tune_spawn_prot_ratio`) gibs the shooter
  instead, with the message "tried to spawn camp and had a slight problem".

### Items

- You can't pick up a power shield or power screen if you already carry one,
  so a second one stays on the map for someone else.
- Server admins can remove or replace power-ups and weapons on every map (see
  [Item spawn cvars](#item-spawn-cvars)).

### Other fixes

- Players no longer spawn inside each other on maps without spawn pads.
- `kill` does nothing while spectating.
- The frags-per-hour count restarts when a player joins from spectator.

## Player commands

Bind these in your client config (`autoexec.cfg` or the console):

```
bind f "cmd spinkick"     // melee kick
bind t "cmd timeleft"     // print the time left on the map
```

| Command | Description |
|---|---|
| `spinkick` | Melee kick at whatever is directly in front of you. |
| `timeleft` | Prints the time left on the map, if the server has a `timelimit`. |

The scoreboard (`score`, or F1 by default) shows the streak
records and the time left. All the standard Quake 2 commands (`use`, `drop`,
`inven`, `weapnext`, `kill`, `playerlist` and so on) work as usual.

## Running a server

1. Build the game library (see [Building](#building)) or use a prebuilt one.
2. Create a `sheridan` folder in your Quake 2 directory and copy the library
   into it, for example `quake2/sheridan/gamex86_64.so`.
3. Start your dedicated server (R1Q2, Q2PRO or similar) with the mod:

   ```
   ./r1q2ded +set game sheridan +set deathmatch 1 +exec server.cfg
   ```

The all-time records are written to a file named `highscores` in the
server's working directory (usually the Quake 2 directory, not the
`sheridan` folder). Delete it to reset the records.

### Example server.cfg

```
// Standard deathmatch settings
set deathmatch 1
set fraglimit 0
set timelimit 20
set maxclients 16

// Spawn protection
set tune_spawn_prot_time 2.0       // seconds of protection after respawning (0 disables it)
set tune_spawn_attack_unprot 1     // 1 = firing ends protection early
set tune_spawn_prot_ratio 0.75     // part of the protection time when railing the player kills the attacker

// Item spawns (these are the defaults: everything spawns normally)
set tune_spawn_power_shield item_power_shield
set tune_spawn_quad item_quad
set tune_spawn_invulnerability item_invulnerability
set tune_spawn_bfg weapon_bfg

map q2dm1
```

### Server cvars

These are the cvars Sheridan adds. Everything else is standard Quake 2.

#### Spawn protection cvars

| Cvar | Default | Description |
|---|---|---|
| `tune_spawn_prot_time` | `2.0` | Seconds of protection after respawning. `0` turns spawn protection off. |
| `tune_spawn_attack_unprot` | `1` | `1`: firing a weapon ends the protection. `0`: protection always lasts the full time. |
| `tune_spawn_prot_ratio` | `0.75` | Part of the protection time (from the start) during which railing the protected player kills the attacker. `0` turns this off. |

#### Item spawn cvars

These are applied when a map loads, so change them before the next `map`
command. A value of `null` means the item doesn't spawn at all.

| Cvar | Default | Description |
|---|---|---|
| `tune_spawn_power_shield` | `item_power_shield` | What spawns in place of each power shield. |
| `tune_spawn_quad` | `item_quad` | What spawns in place of each Quad Damage. |
| `tune_spawn_invulnerability` | `item_invulnerability` | What spawns in place of each Invulnerability. |
| `tune_spawn_bfg` | `weapon_bfg` | What spawns in place of each BFG10K. |
| `tune_spawn_remap1` … `tune_spawn_remap9` | `null` | General replacements, written as `from,to`. |

Examples:

```
set tune_spawn_bfg null                              // no BFG on any map
set tune_spawn_quad item_adrenaline                  // replace quad with adrenaline
set tune_spawn_remap1 "weapon_railgun,weapon_rocketlauncher"
set tune_spawn_remap2 "item_armor_body,null"         // remove body armor
```

The values are Quake 2 entity classnames, such as `item_armor_body`,
`item_armor_combat`, `item_power_screen`, `item_health_mega`,
`weapon_railgun` and `ammo_slugs`.

## Building

You need GNU make and a C compiler (gcc or clang). Windows builds are
cross-compiled with MinGW-w64.

```
make                                    # this machine's architecture
make CONFIG_32BIT=1                     # 32-bit (needs gcc-multilib on 64-bit Linux)
make CONFIG_64BIT=1                     # 64-bit
make CONFIG_WINDOWS=1                   # 32-bit Windows DLL
make CONFIG_WINDOWS=1 CONFIG_64BIT=1    # 64-bit Windows DLL
make strip                              # strip debug symbols from the library
make clean                              # remove the build folder
make V=1                                # show full compiler commands
```

| Build | Output |
|---|---|
| 64-bit Linux | `build-x86_64/gamex86_64.so` |
| 32-bit Linux | `build-i386/gamei386.so` |
| 32-bit Windows | `build-win-x86/gamex86.dll` |
| 64-bit Windows | `build-win-x86_64/gamex86_64.dll` |

To change the default settings without passing them every time, put them in a
file named `.config` next to the Makefile, for example `CONFIG_32BIT=1`.

On Debian or Ubuntu, the packages are `build-essential`, plus `gcc-multilib`
for 32-bit builds and `mingw-w64` for Windows builds.

Optional features (spawn protection, item spawn tuning and the end-of-map
summary) can be turned off at compile time by commenting out their `SSP_`
lines in [ssp_flags.h](ssp_flags.h).
