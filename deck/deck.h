/* deck.h -- EMPIRE's deck and hand model for ECOWAR's card system (EMPIRE NORTHSTAR §3.3, kanban #578).
 *
 * Pure, headless, deterministic. A deck is an ordered cycle of ECOWAR card ids (0..15). The hand is
 * the first EMPIRE_HAND_SIZE slots; playing a slot refills it from the cycle, Clash Royale style.
 * ECOWAR owns the card catalog and effects; this module only knows ids and which ids are MYTHIC.
 */
#ifndef EMPIRE_DECK_H
#define EMPIRE_DECK_H

#define EMPIRE_CARD_COUNT 16   /* ECOWAR_CARDS, packages/simulation/arena_game.c */
#define EMPIRE_HAND_SIZE 4
#define EMPIRE_DECK_MAX 16

typedef struct {
    int cards[EMPIRE_DECK_MAX];  /* the cycle, in order */
    int count;                   /* 0 = not initialised */
    int hand[EMPIRE_HAND_SIZE];  /* card id per slot */
    int next;                    /* index into cards[] of the next card to draw */
} EmpireDeck;

/* Returns 1 on success. Fails (0) if the count is outside EMPIRE_HAND_SIZE..EMPIRE_DECK_MAX or any
 * id is outside 0..EMPIRE_CARD_COUNT-1. Deals the first EMPIRE_HAND_SIZE cards into the hand. */
int empire_deck_init(EmpireDeck *d, const int *card_ids, int n);

/* Plays hand[slot]: returns the card id played and refills that slot from the cycle. Returns -1 for
 * an out-of-range slot or an uninitialised deck. */
int empire_deck_play(EmpireDeck *d, int slot);

/* 1 if this ECOWAR card is MYTHIC-tier (PARENA/stdlib/ecowar/card_effect_mod.prn is-mythic-card?).
 * The id list is copied here; keep it in sync with that module. */
int empire_card_is_mythic(int card_id);

#endif
