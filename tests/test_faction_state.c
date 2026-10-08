/* test_faction_state.c -- headless tests for the shared EMPIRE faction record (kanban #579). */
#include <stdio.h>
#include "../bridge/faction_state.h"

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

int main(void) {
    EmpireFaction f;
    empire_faction_load(&f, 80);
    CHECK(f.decorum == 80 && f.band == 0 && f.can_cast == 1, "a loaded faction at 80 is OK and can cast");
    int rev = f.revision;
    empire_faction_load(&f, 500);
    CHECK(f.decorum == 100, "a load above the cap is clamped to decorum_cap() = 100");
    empire_faction_load(&f, -5);
    CHECK(f.decorum == 0 && f.band == 3 && f.can_cast == 0, "a load below zero clamps and reads as CANCELLED");
    CHECK(f.revision > rev, "every load bumps the revision so a host can tell it changed");
    empire_faction_load(&f, 40);
    CHECK(f.band == 1 && f.can_cast == 1, "40 is the SUSPICION band and can still cast");
    int after = empire_faction_apply_cast(&f, 1);
    CHECK(after == 15 && f.decorum == 15 && f.band == 2, "a MYTHIC cast from 40 spends 25 and drops to HYSTERIC");
    after = empire_faction_apply_cast(&f, 1);
    CHECK(after == 0 && f.can_cast == 0, "a cast that drives decorum to 0 leaves the faction CANCELLED");
    printf("\n%s\n", failures == 0 ? "ALL PASS" : "SOME FAILED");
    return failures == 0 ? 0 : 1;
}
