/* faction_bridge.c -- see faction_bridge.h. Thin composition over the vendored BIG_O rules. */
#include "faction_bridge.h"
#include "../vendor/big_o/witness_rules.h"

/* BIG_O decorum action ids (witness_rules.prn): 1 = SAY_APOCALYPSE, 5 = QUIET_TICK. */
#define ACTION_SAY_APOCALYPSE 1
#define ACTION_QUIET_TICK 5

int empire_faction_start(void) { return decorum_start(); }

int empire_faction_band(int decorum) { return decorum_band(decorum); }

int empire_faction_can_cast(int decorum) { return decorum_band(decorum) != EMPIRE_BAND_CANCELLED; }

int empire_faction_after_cast(int decorum, int is_mythic) {
    return decorum_after(decorum, is_mythic ? ACTION_SAY_APOCALYPSE : ACTION_QUIET_TICK);
}
