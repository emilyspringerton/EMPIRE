/* test_faction_bridge.c -- headless tests for the EMPIRE faction bridge. Runs against the real
 * vendored BIG_O rules, not a hand-copied substitute. */
#include <stdio.h>
#include "../bridge/faction_bridge.h"

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

int main(void) {
    CHECK(empire_faction_start() == 80, "a new faction starts at BIG_O decorum_start() = 80");
    CHECK(empire_faction_band(80) == 0, "starting standing is the OK band");
    CHECK(empire_faction_band(59) == 1, "below 60 is SUSPICION");
    CHECK(empire_faction_band(29) == 2, "below 30 is HYSTERIC");
    CHECK(empire_faction_band(0) == EMPIRE_BAND_CANCELLED, "zero decorum is CANCELLED");
    CHECK(empire_faction_can_cast(80), "OK band may cast");
    CHECK(empire_faction_can_cast(29), "HYSTERIC band may still cast -- only CANCELLED is the fail state");
    CHECK(!empire_faction_can_cast(0), "CANCELLED band may not cast");
    CHECK(empire_faction_after_cast(80, 1) == 55, "a MYTHIC cast spends 25 (SAY_APOCALYPSE)");
    CHECK(empire_faction_after_cast(80, 0) == 81, "a MUNDANE cast earns +1 (QUIET_TICK)");
    CHECK(empire_faction_after_cast(100, 0) == 100, "write-back caps at decorum_cap() = 100");
    CHECK(empire_faction_after_cast(10, 1) == 0, "write-back floors at 0 -- a big spend cancels, never goes negative");
    printf("\n%s\n", failures == 0 ? "ALL PASS" : "SOME FAILED");
    return failures == 0 ? 0 : 1;
}
