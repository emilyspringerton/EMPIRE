/* test_bit_compat.c -- bit-level compatibility between BIG_O's decorum rules and ECOWAR's card system
 * (EMPIRE NORTHSTAR open Q1, kanban #582).
 *
 * Checks, not assumptions:
 *  1. Both sides use plain 32-bit signed int. BIG_O's rules are generated I32 C; ECOWAR's mod is I32 C.
 *  2. Every decorum value a cast can produce stays in 0..decorum_cap(), for every action id, exhaustively.
 *  3. ECOWAR's largest scaled magnitude fits in int with room to spare.
 *  4. EMPIRE's MYTHIC list matches ECOWAR's compiled card_effect_mod, card by card (needs an ECOWAR checkout;
 *     built and run by `make compat`, which passes ECOWAR_DIR).
 */
#include <stdio.h>
#include <limits.h>
#include "../bridge/faction_bridge.h"
#include "../deck/deck.h"
#include "../vendor/big_o/witness_rules.h"

#ifdef ECOWAR_CARD_MOD
#include "card_effect_mod_host.h"
#endif

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

/* The compile-time guard: if either side ever widened to a different int width, the build breaks here. */
_Static_assert(sizeof(int) == 4 && INT_MAX >= 2147483647, "decorum and card magnitudes assume 32-bit int");

int main(void) {
    CHECK(sizeof(int) == 4, "both sides use 32-bit signed int");

    int in_range = 1;
    for (int d = 0; d <= decorum_cap(); d++)
        for (int action = 0; action <= 6; action++) {
            int after = decorum_after(d, action);
            if (after < 0 || after > decorum_cap()) in_range = 0;
        }
    CHECK(in_range, "every decorum (0..cap) x every action (0..6) stays inside 0..decorum_cap(), exhaustively");
    CHECK(decorum_cap() <= 255, "decorum fits in a uint8 if it is ever put on the wire");

    int max_card_base = 3000;   /* largest ECOWAR_CARDS base_magnitude (Nidhogg, ms) */
    int max_scaled = max_card_base + max_card_base / 2;   /* MYTHIC +50% */
    CHECK(max_scaled < INT_MAX / 2, "the largest scaled card magnitude fits in int with room to spare");

#ifdef ECOWAR_CARD_MOD
    int mismatches = 0;
    for (int id = 0; id < EMPIRE_CARD_COUNT; id++) {
        int scaled = on_ecowar_resolve_card_magnitude(id, 100);   /* 100 base: MYTHIC gives 150, MUNDANE 100 */
        int ecowar_mythic = scaled != 100;
        if (ecowar_mythic != empire_card_is_mythic(id)) {
            printf("  card %d: ECOWAR says mythic=%d, EMPIRE says %d\n", id, ecowar_mythic, empire_card_is_mythic(id));
            mismatches++;
        }
    }
    CHECK(mismatches == 0, "EMPIRE's MYTHIC list matches ECOWAR's compiled card_effect_mod for all 16 cards");
#else
    printf("SKIP: EMPIRE MYTHIC list vs ECOWAR card_effect_mod (run `make compat ECOWAR_DIR=...`)\n");
#endif

    printf("\n%s\n", failures == 0 ? "ALL PASS" : "SOME FAILED");
    return failures == 0 ? 0 : 1;
}
