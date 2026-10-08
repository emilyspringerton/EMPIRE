/* faction_state.h -- the shared BIG_O faction state that EMPIRE's card UI reads and writes (EMPIRE
 * NORTHSTAR §3.3, kanban #579).
 *
 * One EmpireFaction per player faction. The BIG_O side loads its authoritative decorum into it with
 * empire_faction_load(). The deck UI binds to the same record, gates casting on it, and writes each
 * committed cast back through the bridge. Anything else that holds the pointer sees the same numbers,
 * which is how the card UI and BIG_O's own state stay one thing rather than two.
 *
 * Honest gap: BIG_O has no live, server-side faction decorum store yet. Its only per-player decorum is
 * the scenario sim (core/sim.h SimPlayer). Nothing in this repo reads from a live BIG_O server today --
 * the loader is the seam for that feed, not the feed itself.
 */
#ifndef EMPIRE_FACTION_STATE_H
#define EMPIRE_FACTION_STATE_H

typedef struct {
    int decorum;   /* 0..decorum_cap(), BIG_O's own clamp */
    int band;      /* BIG_O decorum band, recomputed on every change: 0 OK .. 3 CANCELLED */
    int can_cast;  /* 1 unless the band is CANCELLED */
    int revision;  /* bumps on every change, so a host can tell when to re-sync */
} EmpireFaction;

/* Sets decorum from the authoritative BIG_O value (clamped), recomputes band and can_cast, bumps revision. */
void empire_faction_load(EmpireFaction *f, int decorum);

/* Applies one successful cast through the bridge (MYTHIC spends, MUNDANE earns) and returns the new decorum. */
int empire_faction_apply_cast(EmpireFaction *f, int is_mythic);

#endif
