#include "faction_state.h"
#include "faction_bridge.h"
#include "../vendor/big_o/witness_rules.h"

static void refresh(EmpireFaction *f) {
    f->band = empire_faction_band(f->decorum);
    f->can_cast = empire_faction_can_cast(f->decorum);
    f->revision++;
}

void empire_faction_load(EmpireFaction *f, int decorum) {
    f->decorum = clamp_decorum(decorum);
    refresh(f);
}

int empire_faction_apply_cast(EmpireFaction *f, int is_mythic) {
    f->decorum = empire_faction_after_cast(f->decorum, is_mythic);
    refresh(f);
    return f->decorum;
}
