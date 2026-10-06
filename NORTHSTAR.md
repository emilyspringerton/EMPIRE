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

**Honest caveat, updated 2026-10-06 (founder corrections, same session):** this sandbox has
`BIG_O` and `DEADWEIGHT` (v1) checked out. `ECOWAR` and `REDGARDEN` are not present here at all —
grounded only in repo-table/sibling-doc descriptions, not a direct code read. "DEADSPACE1"
(earlier founder message) is confirmed a typo for `DEADWEIGHT`. **`DEADWEIGHT_2` is real** — founder
confirms it is already built, just not shipped yet, and not checked out in this sandbox either, so
its actual code/scope below is inferred from the founder's own description plus `DEADWEIGHT` v1's
spec, not read directly — flagged, not fabricated as verified. **`REDGARDEN` has no auth model**
(founder, real-time) — this corrects §1's earlier assumption below that DEADWEIGHT's guest-identity
ask could reuse "the REDGARDEN model": there is no REDGARDEN model to reuse. That ask was always
asking for *new* IDUNA work (Phase 1 already scoped it that way); the correction removes a false
sense that a working reference implementation exists somewhere to copy from. It also means
REDGARDEN itself cannot plug into Empire's identity backbone until it gets *some* auth model — this
is now a named blocker, not assumed solved, for any future REDGARDEN-into-Empire step (REDGARDEN
is not one of this doc's three merge targets, but it's the lineage ECOWAR forked from, so the gap
is adjacent and worth tracking in the same doc).

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

## 2a) The fusion, as actually asked for (founder, real-time, same session)

> "cook on EMPIRE — merge ECOWAR and BIG_O via a RTS Clash of Clans-like interface, take any bits
> from DEADWEIGHT, and build out a DEADWEIGHT_2 full RTS-style shipping and world ecology
> management sim — play cards to organize the resources of the runners... mix in BIG_O, all the
> factions are there, we have a full built world with lore and factions, and TRAPX is the glue
> that ties it all together."

This is more specific than §1-2's backbone-only read — it asks for a real gameplay fusion, not
just shared ladder/identity plumbing. Mapped against what each piece actually is (checked against
the real descriptions on record, not invented fresh):

- **The Clash of Clans layer is BIG_O's own "day/night/basement" structure, renamed to its genre.**
  CoC's loop is: a persistent home base you build between raids, an attack phase against another
  player's base (or an async ghost of it), and a resource economy that gates both. BIG_O already
  *is* this shape — basement (build/manage), day (go out and take resources from the world),
  basement-again (spend them), night (the social-stealth layer CoC doesn't have, BIG_O's own
  genuinely novel Attention/Heat system). The "RTS Clash of Clans interface" isn't a new system to
  invent — it's BIG_O's basement-and-day loop given a top-down base-view UI instead of (or
  alongside) its first-person SHANKPIT-engine view.
- **ECOWAR supplies the attack/combat resolution.** ECOWAR's real, shipped 16-card decision-logic
  system (`stdlib/ecowar/card_effect_mod.prn`) is a card-driven combat resolver that already works
  standalone. In the fusion, an ECOWAR "attack" becomes what happens when you raid another
  player's BIG_O-shaped base: the CoC-style base view is the strategic layer, ECOWAR's card system
  is what actually resolves the fight once it starts — same relationship Clash Royale's card combat
  has to Clash of Clans' base-building, which is very likely the actual reference point being
  reached for here even though only "Clash of Clans" was named.
- **DEADWEIGHT's real idea — the grid-packing/fragmentation-tax knapsack — becomes the *shipping*
  mechanic, not the combat mechanic.** DEADWEIGHT v1 used its 6x6 grid for combat positioning;
  "shipping and world ecology management" reframes the same spatial-knapsack idea around packing
  cargo into **runners** (the transport unit the founder names directly) — splittable items,
  fragmentation tax, same engine, different fiction: not "pack your combat loadout," but "pack
  your supply run before it ships out into a contested world." This is the "bits from DEADWEIGHT"
  the founder asked to take, named precisely rather than vaguely.
- **"Play cards to organize the resources of the runners"** is DEADWEIGHT's card layer
  (`card_rules.prn` — real, pure, headless, dual-emittable per `BIG_O/NORTHSTAR.md`'s own citation
  of it) repointed at logistics instead of combat: a card represents a cargo/route/crew decision
  for a runner, resolved the same deterministic, PARENA-scalar way DEADWEIGHT already resolves
  combat cards. Same module shape, new domain — not a new system.
- **TRAPX is the lore/faction substrate underneath all of it**, exactly as named:
  `SHANKPIT/docs2/TRAPX_NORTHSTAR.md` already has a real faction roster (The Frequency, The Bloc,
  Procurement Houses, Oversight Sects, Media Apparatus, and the newer Trustees) with a Fame/
  reputation table. BIG_O is already explicitly built in the TRAPX universe (its own NORTHSTAR
  frames the night-society/thought-police setting as TRAPX-flavored). ECOWAR and DEADWEIGHT are
  **not** currently TRAPX-attached in anything on record — their card/combat systems are
  faction-agnostic today. "All the factions are there" is the ask to retrofit ECOWAR's 16 cards
  and DEADWEIGHT's runner/cargo cards as faction-aligned content (a card belongs to The Frequency,
  The Bloc, etc.) rather than a neutral deck, using TRAPX's existing Fame mechanic as the
  faction-reputation layer across all three games at once — this is the actual "glue."

**What this means concretely for `DEADWEIGHT_2`** (since the founder says it is already built, not
shipped): if its real current scope doesn't yet include the shipping/ecology/runner-card layer
above, that's the gap between "built" and "the thing just asked for" — not a reason to assume
`DEADWEIGHT_2` needs a rewrite. This doc cannot check `DEADWEIGHT_2`'s actual current code (not
in this sandbox) — the real next step is reading it directly before scoping further, not
extending this design blind.

## 3) Ship-first priority (founder, real-time: "ship ship fast iterate")

Reordering §3's original phase plan under actual urgency rather than dependency-clean sequencing:

1. **Ship `DEADWEIGHT_2` first.** It's built, not shipped — the highest-leverage move available is
   finding out *why* and clearing that, not starting new design work on top of it. The two most
   likely blockers given everything on record: (a) no guest-identity provider in IDUNA yet (§3
   Phase 1 below — real, confirmed-missing), (b) whatever its own untested/unverified edges are,
   which this doc can't see without reading `DEADWEIGHT_2` directly. **Next real action: open
   `DEADWEIGHT_2` (wherever it actually lives — ask the founder for its path/repo if not obvious)
   and audit it against a real ship checklist before writing more design docs about it.**
2. **Then** the ECOWAR+BIG_O Clash-of-Clans fusion (§2a) — real design work, not yet started,
   correctly sequenced after the thing that's already 90% done ships.
3. **Then** the backbone phases below (§3 original) — they matter most once there's more than one
   live game sharing players, which is truer after step 1 ships than before.

## 3a) Original phased plan (dependency order, now step 3 above)

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
4. **REDGARDEN has no auth model at all** (founder, real-time, 2026-10-06) — a real, named gap,
   not assumed fixed by anything in this doc. ECOWAR forked from REDGARDEN; if ECOWAR inherited
   the same gap rather than building its own real auth on top during the fork, that's worth
   checking directly before Phase 1's guest-identity work is treated as "add a provider" rather
   than "build auth for ECOWAR from scratch too." Not resolved here — flagged for a direct read of
   ECOWAR's actual account code once it's available in a sandbox that has it checked out.
5. Does the Clash-of-Clans fusion (§2a) want ECOWAR's card system and DEADWEIGHT's runner-card
   system to become ONE card engine (faction-tagged cards usable as either a combat move or a
   logistics move depending on game context), or two separate card systems that merely share the
   same TRAPX faction-tagging convention? The founder's "mix in BIG_O, all the factions are there"
   reads as the latter (shared *fiction*, not shared *mechanic*) but isn't explicit either way —
   worth confirming before building a unified card engine that might be over-engineering two
   genuinely different card domains (combat resolution vs. logistics resolution) into one.
