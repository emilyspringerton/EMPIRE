#include "ui.h"
#include "../bridge/faction_bridge.h"

int empire_ui_init(EmpireUi *u, const int *card_ids, int n) {
    if (!empire_deck_init(&u->deck, card_ids, n)) return 0;
    u->decorum = empire_faction_start();
    u->screen = EUI_SCREEN_DECK;
    u->hand_cursor = 0;
    u->pending_slot = -1;
    u->cell_x = 0;
    u->cell_y = 0;
    return 1;
}

static int clamp(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

static EmpireEffect none(void) { EmpireEffect e = { EUI_FX_NONE, -1, -1, -1 }; return e; }

EmpireEffect empire_ui_input(EmpireUi *u, EmpireAction a) {
    if (u->screen == EUI_SCREEN_DECK) {
        if (a == EUI_LEFT) u->hand_cursor = (u->hand_cursor + EMPIRE_HAND_SIZE - 1) % EMPIRE_HAND_SIZE;
        else if (a == EUI_RIGHT) u->hand_cursor = (u->hand_cursor + 1) % EMPIRE_HAND_SIZE;
        else if (a == EUI_SELECT) {
            if (!empire_faction_can_cast(u->decorum)) {
                EmpireEffect e = { EUI_FX_BLOCKED, u->deck.hand[u->hand_cursor], -1, -1 };
                return e;
            }
            u->pending_slot = u->hand_cursor;
            u->screen = EUI_SCREEN_UNITS;
            u->cell_x = 0;
            u->cell_y = 0;
        }
        return none();
    }

    /* UNITS */
    if (a == EUI_LEFT) u->cell_x = clamp(u->cell_x - 1, 0, EMPIRE_GRID_W - 1);
    else if (a == EUI_RIGHT) u->cell_x = clamp(u->cell_x + 1, 0, EMPIRE_GRID_W - 1);
    else if (a == EUI_UP) u->cell_y = clamp(u->cell_y - 1, 0, EMPIRE_GRID_H - 1);
    else if (a == EUI_DOWN) u->cell_y = clamp(u->cell_y + 1, 0, EMPIRE_GRID_H - 1);
    else if (a == EUI_BACK) {
        u->pending_slot = -1;
        u->screen = EUI_SCREEN_DECK;
    } else if (a == EUI_SELECT && u->pending_slot >= 0) {
        int card = empire_deck_play(&u->deck, u->pending_slot);
        u->decorum = empire_faction_after_cast(u->decorum, empire_card_is_mythic(card));
        EmpireEffect e = { EUI_FX_PLACED, card, u->cell_x, u->cell_y };
        u->pending_slot = -1;
        u->screen = EUI_SCREEN_DECK;
        return e;
    }
    return none();
}
