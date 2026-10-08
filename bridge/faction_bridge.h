/* faction_bridge.h -- EMPIRE's composition point between ECOWAR's card system and BIG_O's
 * attention/decorum rules (EMPIRE NORTHSTAR §3.1, kanban #576).
 *
 * Canonical home for the bridge. Each game repo keeps its own layer: ECOWAR resolves cards,
 * BIG_O owns the decorum rules (vendor/big_o, synced by scripts/sync_big_o.sh). This module only
 * composes them: read the caster's band to gate casting, then write the cast back to decorum.
 *
 * INVENTED v1 MAPPING, pending founder sign-off: a MYTHIC cast is a visible supernatural act, so it
 * spends decorum as BIG_O's SAY_APOCALYPSE (-25); a MUNDANE cast is quiet and earns QUIET_TICK (+1).
 */
#ifndef EMPIRE_FACTION_BRIDGE_H
#define EMPIRE_FACTION_BRIDGE_H

/* BIG_O's decorum band values (witness_rules.prn): 0 OK, 1 SUSPICION, 2 HYSTERIC, 3 CANCELLED. */
#define EMPIRE_BAND_CANCELLED 3

/* A new player's standing (BIG_O decorum_start()). */
int empire_faction_start(void);

/* The band for a decorum value (BIG_O decorum_band()). */
int empire_faction_band(int decorum);

/* 1 if a faction at this decorum may play a card. CANCELLED is BIG_O's fail state, so it can't. */
int empire_faction_can_cast(int decorum);

/* Decorum after one successful cast, clamped to 0..cap (BIG_O decorum_after()). */
int empire_faction_after_cast(int decorum, int is_mythic);

#endif
