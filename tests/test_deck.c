/* test_deck.c -- headless tests for the EMPIRE deck and placement UI (kanban #578). */
#include <stdio.h>
#include "../deck/ui.h"

static int failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); failures++; } \
    else { printf("PASS: %s\n", msg); } \
} while (0)

static const int DECK8[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };

static void test_deck_validation(void) {
    EmpireDeck d;
    int short_deck[3] = { 0, 1, 2 };
    int bad_id[4] = { 0, 1, 2, 16 };
    CHECK(!empire_deck_init(&d, short_deck, 3), "a deck smaller than the hand is rejected");
    CHECK(!empire_deck_init(&d, bad_id, 4), "an out-of-catalog card id is rejected");
    CHECK(empire_deck_init(&d, DECK8, 8), "an 8-card deck with valid ids initialises");
    CHECK(d.hand[0] == 0 && d.hand[3] == 3, "the first four cards are dealt into the hand");
}

static void test_deck_cycle_refills_the_played_slot(void) {
    EmpireDeck d;
    empire_deck_init(&d, DECK8, 8);
    CHECK(empire_deck_play(&d, 1) == 1, "playing slot 1 returns its card");
    CHECK(d.hand[1] == 4, "the played slot is refilled with the next card in the cycle");
    CHECK(empire_deck_play(&d, 9) == -1 && empire_deck_play(&d, -1) == -1, "an out-of-range slot plays nothing");
    int before = d.next;
    for (int i = 0; i < 8; i++) empire_deck_play(&d, 0);
    CHECK(d.next == before && d.next < 8, "eight plays on a full cycle wrap the index back to where it started");
}

static void test_mythic_lookup_matches_card_mod(void) {
    CHECK(empire_card_is_mythic(3) && empire_card_is_mythic(6) && empire_card_is_mythic(15), "MYTHIC ids 3, 6, 15 are flagged");
    CHECK(!empire_card_is_mythic(0) && !empire_card_is_mythic(5), "MUNDANE ids 0 and 5 are not flagged");
}

static void test_deck_screen_select_enters_placement(void) {
    EmpireUi u;
    empire_ui_init(&u, DECK8, 8);
    empire_ui_input(&u, EUI_RIGHT);
    EmpireEffect e = empire_ui_input(&u, EUI_SELECT);
    CHECK(e.kind == EUI_FX_NONE && u.screen == EUI_SCREEN_UNITS, "SELECT on the hand opens placement without spending anything yet");
    CHECK(u.pending_slot == 1, "the chosen hand slot is held as pending");
}

static void test_placement_clamps_and_commits(void) {
    EmpireUi u;
    empire_ui_init(&u, DECK8, 8);
    empire_ui_input(&u, EUI_SELECT);
    empire_ui_input(&u, EUI_LEFT);
    empire_ui_input(&u, EUI_UP);
    CHECK(u.cell_x == 0 && u.cell_y == 0, "the grid cursor clamps at the edges instead of wrapping");
    for (int i = 0; i < 10; i++) empire_ui_input(&u, EUI_RIGHT);
    CHECK(u.cell_x == EMPIRE_GRID_W - 1, "the grid cursor clamps at the right edge");
    empire_ui_input(&u, EUI_DOWN);
    EmpireEffect e = empire_ui_input(&u, EUI_SELECT);
    CHECK(e.kind == EUI_FX_PLACED && e.card_id == 0 && e.cell_x == EMPIRE_GRID_W - 1 && e.cell_y == 1,
          "SELECT on the grid commits the pending card to the chosen cell");
    CHECK(u.screen == EUI_SCREEN_DECK && u.pending_slot == -1, "a committed placement returns to the hand");
    CHECK(u.deck.hand[0] == 4, "the committed slot is refilled from the cycle");
}

static void test_back_cancels_with_nothing_spent(void) {
    EmpireUi u;
    empire_ui_init(&u, DECK8, 8);
    int before = u.deck.hand[0];
    empire_ui_input(&u, EUI_SELECT);
    EmpireEffect e = empire_ui_input(&u, EUI_BACK);
    CHECK(e.kind == EUI_FX_NONE && u.screen == EUI_SCREEN_DECK && u.pending_slot == -1, "BACK from placement returns to the hand");
    CHECK(u.deck.hand[0] == before && u.decorum == 80, "a cancelled placement spends no card and no decorum");
}

static void test_placement_writes_back_decorum(void) {
    EmpireUi u;
    empire_ui_init(&u, DECK8, 8);       /* hand[0] = card 0, MUNDANE */
    empire_ui_input(&u, EUI_SELECT);
    empire_ui_input(&u, EUI_SELECT);
    CHECK(u.decorum == 81, "a MUNDANE placement earns +1 decorum through the bridge");
    empire_ui_input(&u, EUI_RIGHT);     /* hand[1] = card 1, MUNDANE */
    empire_ui_input(&u, EUI_RIGHT);     /* hand[2] = card 2, MUNDANE */
    empire_ui_input(&u, EUI_RIGHT);     /* hand[3] = card 3, MYTHIC */
    empire_ui_input(&u, EUI_SELECT);
    empire_ui_input(&u, EUI_SELECT);
    CHECK(u.decorum == 56, "a MYTHIC placement spends 25 decorum through the bridge");
}

static void test_cancelled_faction_is_blocked(void) {
    EmpireUi u;
    empire_ui_init(&u, DECK8, 8);
    u.decorum = 0;                      /* CANCELLED */
    EmpireEffect e = empire_ui_input(&u, EUI_SELECT);
    CHECK(e.kind == EUI_FX_BLOCKED && u.screen == EUI_SCREEN_DECK, "a CANCELLED faction's SELECT is refused and stays on the hand");
    CHECK(u.deck.hand[0] == 0, "a blocked SELECT spends no card");
}

int main(void) {
    test_deck_validation();
    test_deck_cycle_refills_the_played_slot();
    test_mythic_lookup_matches_card_mod();
    test_deck_screen_select_enters_placement();
    test_placement_clamps_and_commits();
    test_back_cancels_with_nothing_spent();
    test_placement_writes_back_decorum();
    test_cancelled_faction_is_blocked();
    printf("\n%s\n", failures == 0 ? "ALL PASS" : "SOME FAILED");
    return failures == 0 ? 0 : 1;
}
