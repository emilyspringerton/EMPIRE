# EMPIRE — NORTHSTAR (scoping only, no code, local repo — no upstream yet)

**Status:** new, NORTHSTAR only, same precedent `BIG_O`/`PARENA`/`DEADWEIGHT` already set (scope
first, hand phased sub-tasks to `EMILY/BACKLOG.md`, don't start building the whole thing).

## 0) Founder framing (verbatim, real-time, 2026-10-06)

> "merge ECOWAR, DEADWEIGHT[2], and BIG_O [into one] main repo... it's all the same, it's like an
> empire of worlds."

Frame-break, same lens as `SHANKPIT/docs2/SHARED_ENGINE_NORTHSTAR.md`'s own opening: the specific
ask ("merge three game repos") is one instance of the general pattern this monorepo keeps hitting —
real shared structure is discovered after three products are built independently, and the fix is
never "collapse the three codebases into one binary," it's "name the one layer that's actually
shared and put it under all three." `SHARED_ENGINE_NORTHSTAR.md` named that for
SHANKPIT/REDGARDEN/GFD (netcode/reconcile/humanness/itemstat/matchmaking). This doc does the same
analysis for ECOWAR/DEADWEIGHT/BIG_O — a different cut, because these three share a different
layer: not real-time movement/engine primitives, but **competitive PvP meta** — ladder, bot pool,
identity, economy.

**Honest caveat before anything else:** this sandbox has `BIG_O` and `DEADWEIGHT` checked out.
`ECOWAR` is not present here (referenced only via the main `CLAUDE.md` repo table and
`DEADWEIGHT/NORTHSTAR.md`'s own description of it) and "`DEADWEIGHT2`" does not exist as a repo
anywhere in this monorepo index — only `DEADWEIGHT` does. This doc treats "`DEADWEIGHT2`" as
`DEADWEIGHT` and flags the discrepancy rather than guessing a second repo into existence. If a
real `DEADWEIGHT2` exists upstream and hasn't been pulled, that's the first thing to check before
treating this doc as grounded in current reality.

## 1) The actual shared layer — checked, not assumed

| | ECOWAR | DEADWEIGHT | BIG_O |
|---|---|---|---|
| Core loop | 1v1 card-RTS skirmish (16-card system, real PARENA decision logic) | 1v1 grid-packing + combat (6x6 spatial knapsack, fragmentation tax) | day/night/basement loops — day harvest (SHANKPIT FPS), night social-stealth, basement async shadow war |
| PvP shape | real-time 1v1 match | real-time 1v1 match | **async** — bases attack each other on a clock, not a live match |
| Bot pool | real, shipped: own matchmaker + bot-pool (`:9779`) | explicitly spec'd "same bot pool set up as ECOWAR" (founder quote, `DEADWEIGHT/NORTHSTAR.md`) — not yet built, but the shared-bot-pool intent is already on record, written by the founder before this doc existed | basement shadow war needs an async opponent-resolution system — not a live bot pool, but the same underlying "something stands in for or represents the other player" problem |
| Identity/account | ECOWAR's own match tracking (REDGARDEN lineage) | spec explicitly wants "online services the same model as REDGARDEN — full tracking of games and profiles... guest-first, name-only, no email required to play, can't recover without linking one" | not yet scoped for day/night, but basement shadow war is inherently cross-player and needs the same account model to mean anything |
| Target client | PARENA-mod-on-native-host (REDGARDEN lineage) | explicit founder ask: "1 client, 2 platforms (Android+Windows), like Hearthstone" — found in `DEADWEIGHT/NORTHSTAR.md` to be **not real yet** (PARENA Java emitter is v0/scalar-only) | native SHANKPIT engine (C/SDL2 + Go/Dragonfly) |

The real finding: **ECOWAR and DEADWEIGHT already share — or are explicitly spec'd to share — a
bot pool and an IDUNA-backed guest-identity/match-tracking model.** BIG_O doesn't share that
mechanism today, but its basement shadow war has the same *shape* (cross-player async
competition needs a ladder/identity backbone even without a live bot pool). "Empire of worlds" is
the right name for what's actually shared: **one ladder/identity/economy backbone, with each game
as a world plugged into it** — not a merged game engine (that's `SHARED_ENGINE_NORTHSTAR.md`'s
job, a separate and mostly orthogonal layer: ECOWAR/DEADWEIGHT/BIG_O's engine needs are PARENA
decision-logic + SHANKPIT-descended rendering, not the netcode/reconcile primitives that doc
scoped for SHANKPIT/REDGARDEN/GFD specifically).

## 2) What "merge into one main repo" should actually mean

Lesson already learned once in this monorepo and worth stating up front: a "merge" that collapses
three different C/game-client codebases into one binary produces exactly the kind of mis-scope
`BURROW` had to self-correct out of ("just a new Go backend" → real feature-parity rewrite) and
the kind `DEADWEIGHT`'s own capability audit already warned against ("1 PARENA client, 2
platforms" isn't real yet — two native shells sharing one backend is the honest answer). Applying
that same discipline here: **`EMPIRE` is a new, real main repo, but it holds the backbone, not the
three games' own client/engine code.**

`EMPIRE` main repo holds:
- **Ladder/ranking** — one Elo (or Elo-family) service, per-game, same shape
  `SLOWBOT_LEAGUE/NORTHSTAR.md` already proposed for its own IDUNA `games.Registry` row: one new
  row + migration per game, not a new subsystem per game.
- **Guest-first identity** — the account model DEADWEIGHT's own spec explicitly asked for
  (name-only, no email, unrecoverable without linking one), built once against IDUNA, reused by
  all three. IDUNA's `players` table today supports only `google`/`iduna_local` — this guest
  provider is genuinely new IDUNA work, not duplicated three times.
- **Shared bot pool** — ECOWAR's real `:9779` matchmaker+bot-pool service becomes the one DEADWEIGHT
  already asked to reuse by name; BIG_O's async shadow-war resolver is evaluated separately in
  Phase 2 below rather than forced into the same shape prematurely.
- **Economy/portal surface** — WOTAN's own real precedent (GFD Flow currency, cosmetic store,
  esports/stats hub split out of a single marketing page into its own repo+subdomain) is the
  right template for an Empire-wide portal: one place a player logs into, sees their standing
  across all three worlds, spends one currency.

`EMPIRE` main repo does **not** hold: ECOWAR's card logic, DEADWEIGHT's grid/combat PARENA mods,
or BIG_O's day/night/basement SHANKPIT-engine code. Each stays its own repo, becomes a client of
`EMPIRE`'s backbone the same way a game becomes a client of IDUNA today.

## 3) Phased plan

- **Phase 0 — name the backbone, touch nothing live.** This doc. Hand sub-items to
  `EMILY/BACKLOG.md` under a new section rather than starting build here (Principle 19 — unscoped
  asks get cut into real phased work, not built whole).
- **Phase 1 — guest identity in IDUNA.** Add the anonymous/guest provider to `players` (real new
  work, checked: doesn't exist today). This single item unblocks DEADWEIGHT's own already-written
  spec and is the one piece of Empire genuinely needed before DEADWEIGHT can ship at all — not
  Empire-specific scope creep, DEADWEIGHT already asked for exactly this.
- **Phase 2 — ladder service.** One `games.Registry`-backed Elo service (ECOWAR + DEADWEIGHT first,
  both real-time 1v1 — same shape). BIG_O's async shadow war is evaluated *after* this lands,
  honestly, because it may need a different ranking shape (asynchronous resolution, not a
  completed-match Elo update) — do not assume it fits the same service without checking BIG_O's
  actual basement-war resolution rules first (not yet specified in `BIG_O/NORTHSTAR.md` beyond
  "async deterministic shadow war").
- **Phase 3 — shared bot pool.** Point DEADWEIGHT's matchmaking at ECOWAR's real `:9779` service
  (or extract it to a shape both can run as separate processes against, per `SLOWBOT_LEAGUE`'s own
  "second server process" precedent) instead of DEADWEIGHT re-implementing ECOWAR's bot-pool code.
- **Phase 4 — WOTAN-pattern portal.** One Empire-wide stats/ladder/cosmetic-economy page, built on
  WOTAN's real design-system precedent, reused once rather than three times (same "built once,
  reused twice" lesson `SLOWBOT_LEAGUE/NORTHSTAR.md` already named for its own leaderboard need).

## 4) Open questions (not resolved here)

1. Does BIG_O's basement shadow war actually want a live ranked ladder, or is it closer to an
   async PvE-against-a-ghost model that only *looks* like PvP? This changes whether Phase 2
   includes it at all. Needs BIG_O's own basement-war rules specified first.
2. Is "`DEADWEIGHT2`" a real, separate planned repo (a sequel/split) the founder has in mind but
   hasn't created yet, or a naming slip for `DEADWEIGHT`? Flagged in §0 — worth a direct check
   before more work is scoped against a name that may not refer to anything real.
3. Should `EMPIRE` absorb WOTAN's own repo, or sit alongside it as a consumer of WOTAN's portal
   pattern? `EMPIRE` is the identity/ladder/bot-pool backbone; WOTAN is already scoped as the
   cosmetic-economy/stats-hub frontend. Treating them as two repos with a clean API boundary
   (same relationship IDUNA has to every product that authenticates against it) is the current
   read — not forced to merge just because both are "empire of worlds"-shaped asks from the same
   session.
