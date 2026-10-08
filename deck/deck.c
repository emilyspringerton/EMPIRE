#include "deck.h"

int empire_deck_init(EmpireDeck *d, const int *card_ids, int n) {
    if (n < EMPIRE_HAND_SIZE || n > EMPIRE_DECK_MAX) return 0;
    for (int i = 0; i < n; i++)
        if (card_ids[i] < 0 || card_ids[i] >= EMPIRE_CARD_COUNT) return 0;
    for (int i = 0; i < n; i++) d->cards[i] = card_ids[i];
    d->count = n;
    for (int i = 0; i < EMPIRE_HAND_SIZE; i++) d->hand[i] = d->cards[i];
    d->next = EMPIRE_HAND_SIZE % n;
    return 1;
}

int empire_deck_play(EmpireDeck *d, int slot) {
    if (d->count == 0 || slot < 0 || slot >= EMPIRE_HAND_SIZE) return -1;
    int played = d->hand[slot];
    d->hand[slot] = d->cards[d->next];
    d->next = (d->next + 1) % d->count;
    return played;
}

/* card_effect_mod.prn is-mythic-card?: ids 3, 6..15 are MYTHIC; 0,1,2,4,5 are MUNDANE. */
int empire_card_is_mythic(int card_id) {
    if (card_id == 3) return 1;
    return card_id >= 6 && card_id <= 15;
}
