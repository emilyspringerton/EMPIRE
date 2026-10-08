/* ui.h -- the BURNER-UI deck and unit-placement apps for ECOWAR's card system (EMPIRE NORTHSTAR §3.2
 * and §3.3, kanban #578). Pure input -> state -> effect, the same shape as BIG_O's bigo_phone.h, so a
 * phone shell can render it and feed it abstract actions. No rendering, no network, no SDL.
 *
 * Two screens:
 *   DECK   -- the hand. LEFT/RIGHT move across the 4 slots, SELECT picks a card to play.
 *   UNITS  -- the placement grid. Arrows move the cursor (clamped), SELECT commits the placement,
 *             BACK cancels with nothing spent.
 * The caster's BIG_O decorum gates SELECT on DECK (CANCELLED can't play), and a committed placement
 * writes decorum back through faction_bridge. Costs and cooldowns are not modelled yet.
 */
#ifndef EMPIRE_UI_H
#define EMPIRE_UI_H

#include "deck.h"

#define EMPIRE_GRID_W 6   /* v1 layout, invented: 3 lanes x 6 columns */
#define EMPIRE_GRID_H 3

typedef enum { EUI_UP, EUI_DOWN, EUI_LEFT, EUI_RIGHT, EUI_SELECT, EUI_BACK } EmpireAction;

typedef enum { EUI_SCREEN_DECK = 0, EUI_SCREEN_UNITS } EmpireScreen;

typedef enum {
    EUI_FX_NONE = 0,
    EUI_FX_PLACED,   /* a card was committed to a grid cell; card_id, cell_x, cell_y are set */
    EUI_FX_BLOCKED   /* SELECT refused: the faction is CANCELLED */
} EmpireFxKind;

typedef struct {
    EmpireFxKind kind;
    int card_id;
    int cell_x, cell_y;
} EmpireEffect;

typedef struct {
    EmpireDeck deck;
    int decorum;        /* BIG_O decorum, 0..100; the faction's standing */
    int screen;         /* EmpireScreen */
    int hand_cursor;    /* 0..EMPIRE_HAND_SIZE-1 */
    int pending_slot;   /* hand slot chosen on DECK, waiting for a cell; -1 = none */
    int cell_x, cell_y; /* UNITS cursor */
} EmpireUi;

/* Starts a faction at BIG_O's decorum_start() with the given deck. Returns 0 if the deck is invalid. */
int empire_ui_init(EmpireUi *u, const int *card_ids, int n);

EmpireEffect empire_ui_input(EmpireUi *u, EmpireAction a);

#endif
