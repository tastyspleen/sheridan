// License: GPL

#ifndef __SSP_FLAGS_H__
#define __SSP_FLAGS_H__

/*
	Most changes can be enabled independently at your discretion,
	by defining or undefining the SSP_ flags below.

	The following changes are always enabled and do not have
	accompanying SSP_ flags (but you can search for them in comments.)
	
		- SSP_RCON_EXPLOIT_FIX [-NOW HANDLED BY R1Q2-]
			In sv_user.c/SV_ExecuteUserCommand(), passes false instead
			of true to Cmd_TokenizeString() so that user commands are
			not macro-expanded.
			NOTE: As of V0.0.3 this is now handled by R1Q2.
		
		- SSP_NO_KILL_IN_SPEC
			In g_cmds.c/Cmd_Kill_f(), we take no action if client is
			a spectator.  THIS IS A BAND-AID FIX.  There seems to be
			a bug where the server may crash if a player hangs out dead
			in spectator for an indeterminite time.  It would be 
			preferable to find the underlying crash bug.
*/

#define SSP_SPAWN_PROTECT
//////
	// Yellow temporary invincibilty on spawn.
	// cvars: "tune_spawn_prot_time" (float)
	//			Number of seconds spawn protect lasts.
	//		  "tune_spawn_attack_unprot" (boolean)
	//			If true, player pressing attack button
	//			cancels extant spawn protection.

#define SSP_SPAWN_PROTECT_EXTREME_PREJUDICE
//////
	// You done gone hit yellow spawner with rail
	// and got blowed up, Bubba !
	// cvars: "tune_spawn_prot_ratio" (float)
	//			Defines how much of the inital spawn_protect_time
	//			is deadly if railed.

#if defined(SSP_SPAWN_PROTECT_EXTREME_PREJUDICE) && !defined(SSP_SPAWN_PROTECT)
#error SSP_SPAWN_PROTECT_EXTREME_PREJUDICE requires SSP_SPAWN_PROTECT
#endif


#define SSP_SIAMESE_SPAWN_FIX
//////
	// On some maps, players spawn on top of each other
	// without telefragging.  Typically these are maps that
	// don't use spawn pads, but spawn the player right on
	// the floor.  For whatever reason, trace() sees these
	// players as embedded in a solid.  Presumably the spawn
	// points are slightly underground, but not enough to
	// keep the player from moving?  Without delving too
	// deeply into this, this fix just tweaks g_utils.c/KillBox()
	// to ignore CONTENTS_SOLID in its trace.  Seems to work.
	// Would be preferable to understand the true cause of
	// the problem.


#define SSP_RESET_ENTERFRAME_LEAVING_SPEC
//////
	// Reset connect time to zero when player rejoins game
	// coming out of spectator.


#define SSP_TUNE_SPAWNS
//////
	// Allows certain item spawns to be suppressed, or replaced
	// with alternate items:
	//    tune_spawn_power_shield		"itemname" or "null"
	//    tune_spawn_quad
	//    tune_spawn_invulnerability
	//    tune_spawn_bfg
	//
	// Allows generic remap from item, to item:
	//    tune_spawn_remap1				"itemname_from,itemname_to" or "null"
	//    tune_spawn_remap2
	//    tune_spawn_remap3
	//		...
	//    tune_spawn_remap9


#define SSP_DUMP_HIGHSCORES_TO_CLIENT_CONSOLES
//////
	// At end of map (via BeginIntermission() in p_hud.c)
	// prints a high score summary of the top N scoring players,
	// and stats like frags-per-hour.
	// This patch submitted by Chattanoo.


// Wish list:
//		SSP_SSL_SECURE_RCON
//		SSP_MAPNAME_DOT_BSP_CRASH_FIX
//		SSP_SSL_MESSAGE_PORT
//		SSP_TUNE_DISALLOW_CR_IN_CHAT
//		SSP_TUNE_CVAR_WEAP_FIRE_RATE
//		SSP_TUNE_CVAR_WEAP_RELOAD_RATE
//		SSP_TUNE_CVAR_STARTING_HEALTH
//		SSP_TUNE_CVAR_STARTING_AMMO     (mg50 rkt10 whatever...)
//		SSP_TUNE_CVAR_STARTING_WEAPS    (or maybe just general starting_items)




/*****************************************************************************
 *****************************************************************************
 *****************************************************************************
	HISTORY

	New in V0.0.3
	  -	Merged SSP with R1Q2!  (SSP_RCON_EXPLOIT_FIX is now handled
		by R1Q2.)

	New in V0.0.2
	  -	Improved consistency of addip/removeip usage and feedback messages
		in g_svcmds.c (both ctf and game.)  Submitted by Karen Pouelle.

	New in V0.0.1
		SSP_RCON_EXPLOIT_FIX
		SSP_NO_KILL_IN_SPEC
		SSP_SPAWN_PROTECT
		SSP_SPAWN_PROTECT_EXTREME_PREJUDICE
		SSP_SIAMESE_SPAWN_FIX
		SSP_RESET_ENTERFRAME_LEAVING_SPEC
		SSP_TUNE_SPAWNS
		SSP_DUMP_HIGHSCORES_TO_CLIENT_CONSOLES

 *****************************************************************************
 *****************************************************************************
 *****************************************************************************/


#endif  // __SSP_FLAGS_H__

