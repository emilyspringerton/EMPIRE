# EMPIRE — NORTHSTAR (corrected scope, 2026-10-06)

**Status:** design/scoping, repos now checked out locally (`ECOWAR`, `REDGARDEN`, `DEADWEIGHT_2`,
`TRAPX`, `MONOREPO` pulled fresh this session alongside existing `BIG_O`/`DEADWEIGHT`/`SHANKPIT`),
no merge code written yet.

## 0) Correction (founder, real-time, 2026-10-06 — supersedes this doc's earlier v0)

The first draft of this doc read "merge" as "share a ladder/identity backend between three
otherwise-separate games." That was wrong. Founder, verbatim:

> "EMPIRE is literally a merging of BIG_O, ECOWAR, and DEADWEIGHT 1+2 — full ECOWAR RTS style,
> card places units, Clash Royale style, but the factions are actually the BIG_O simulation. The
> game is ECOWAR. BIG_O is the tech tree. TRAPX is the universe. TYLER is the lore. SHANKPIT is
> the operating system. REDGARDEN is the hero UI. DEADWEIGHT is the card language. Merge ECOWAR
> and BIG_O first — ECOWAR is the client, BIG_O is the headless sim that needs a GUI resource
> management sim [wrapped in] TRAPX doctrine."

This is one product with a named role for every repo in the stack, not three products sharing a
backend. Each role below is checked against that repo's own real, current code — not assumed from
its name.

## 1) The stack, role by role (checked against real code, not guessed)

| Repo | Role in EMPIRE | What's actually there today |
|---|---|---|
| **ECOWAR** | **The game.** Clash Royale-shaped client: play cards, cards place/affect units, resolved in real time. | Hard fork of REDGARDEN (`apps/arena`/`apps/arena_server`, C99/SDL2/OpenGL, server-authoritative UDP). **16 real cards shipped** (`packages/simulation/arena_game.c`'s `ECOWAR_CARDS`, resolution via `PARENA/stdlib/ecowar/card_effect_mod.prn` — real I32 decision logic, not a bare trigger), each grounded in a `TYLER/multiverse_heroes.md` hero. **Gap, confirmed in ECOWAR's own README**: no deck/hand UI, no unit-placement input wiring yet — the cards are callable and tested end-to-end, but nothing in-match actually plays one. This is exactly the "Clash Royale style" layer that doesn't exist yet. Own matchmaker+bot-pool real and live (`:9779`, `ECOWAR-BOTS` IDUNA M2M agent).
| **BIG_O** | **The tech tree / faction simulation**, headless, driving ECOWAR's factions from underneath. Needs a GUI resource-management layer wrapped around it. | Real, live precedent for exactly this shape: `core/lab_sim.h/.c` (headless cloning-facility equipment pipeline, "simulate real lab equipment," zero server/GUI logic), Attention/Heat rules module (PARENA-scalar, headless tests + a tiny text sim to *play* it — i.e. BIG_O already proves out headless-sim-plus-minimal-harness as its own pattern). Day/night/basement loop *is* the tech-tree/economy shape the founder means by "factions are actually the BIG_O simulation" — basement lab + pheromone army + Attention/Heat *is* a faction-management sim with no GUI today.
| **DEADWEIGHT (1) / DEADWEIGHT_2** | **The card language.** The grammar cards are written in, not the cards themselves. | `DEADWEIGHT/core/card_rules.c/.h` — a real, pure, headless, deterministic card-resolution engine (6x6 grid, splittable items, fragmentation tax — the mechanic DEADWEIGHT v1 built for its own combat is, structurally, a card-effect resolver). `DEADWEIGHT_2` ("CRASH_OVERRIDE") is real, shipped CI/releases (Windows+native client, v0.3.0 tag found in its own git log) — **shipped**: v0.3.0 released 2026-09-25 (Linux + Windows assets), v0.6.0 is latest. The earlier "built, not shipped" note is stale. See `audits/deadweight2_ship_audit.md`.
| **TRAPX** | **The universe/doctrine** — the systemic rules every faction operates under. | Real, separate repo (not the `SHANKPIT/docs2/TRAPX_NORTHSTAR.md` derivative) — TRAPX v1 "BLOCK ZERO": single-player systemic urban sim, a 10x10/12x12 grid, no avatar, no direct unit control, no win condition, cells tracking alignment (Neutral/Player/Rival/Institutional) and signed pressure, influenced by a deck of operations. **This is the doctrine layer BIG_O's faction sim should be an instance of** — TRAPX is the general systemic-influence ruleset, BIG_O's Attention/Heat/faction-politics is one concrete application of it.
| **TYLER** | **The lore.** Narrative substrate every hero/card/faction is actually *from*. | `TYLER/multiverse_heroes.md` is already ECOWAR's real card-content source today — this role is already live, not aspirational.
| **SHANKPIT** | **The operating system** — engine/platform substrate (netcode, rendering, level/portal system) everything else runs on top of. | Matches `SHARED_ENGINE_NORTHSTAR.md`'s own framing of SHANKPIT as the canonical engine source; BIG_O is literally built on the SHANKPIT engine already (`BIG_O/NORTHSTAR.md` day-loop = "SHANKPIT FPS (existing)").
| **REDGARDEN** | **The hero UI** — not gameplay anymore, the visual/UX layer for presenting heroes/cards. | ECOWAR's fork source; `REDGARDEN/docs/HEROES_VS0.md` already has real, concrete hero kits (LoL/DOTA-familiar stat lines) — this is the real asset to lift into a UI role (hero select, card frames, stat display) rather than keep developing REDGARDEN as a second independent RTS alongside ECOWAR.

## 2) What "merge" means now (corrected from v0's "shared backend only" read)

Still true from v0, kept: this is not "collapse four C codebases into one Bazel target blindly."
What changes: the four repos aren't peer products sharing a ladder — they're **layers of one
product**, and the merge direction is a real dependency chain, not a bus everyone plugs into:

```
TRAPX (doctrine)  ──────────────┐
TYLER (lore)      ──────────────┤
                                 ▼
                    BIG_O (headless faction/tech-tree sim)
                                 │  (needs: a GUI resource-management
                                 │   front end — doesn't exist yet)
                                 ▼
          ECOWAR (the game — client, cards place units, Clash-Royale real-time resolution)
                                 │  (cards are written in)
                                 ▼
                    DEADWEIGHT's card_rules language
                                 │  (presented via)
                                 ▼
                    REDGARDEN's hero UI assets
                                 │  (running on)
                                 ▼
                    SHANKPIT (engine/OS substrate)
```

`EMPIRE` as a repo is the place this assembly actually happens — not a fifth independent codebase,
a real integration point where ECOWAR's client calls into a BIG_O-derived sim for faction state,
resolves cards through DEADWEIGHT's rules engine, and renders hero content through REDGARDEN's UI
assets, all on SHANKPIT. Each source repo keeps developing its own layer; `EMPIRE` is where they
compose, same relationship PARENA's own per-game `stdlib/{ecowar,gfd,shankpit}` mod directories
already have to `PARENA/stdlib/` core — a consuming layer, not a replacement for any of them.

## 3) Merge order: ECOWAR + BIG_O first (founder's own explicit sequencing)

**Status (2026-10-08):** §3.1 has a first real bridge in this repo: `bridge/faction_bridge.{h,c}`, tested
headlessly against a vendored copy of BIG_O's witness rules (`make test`). The decorum gate and write-back are
real; the MYTHIC/MUNDANE mapping is an invented v1 tuning pending founder sign-off. ECOWAR's own copy (`aff6ce0`)
is not switched onto this module yet. §3.2 TECHTREE shipped as a BIG_O phone app (BIG_O `8f28e26`), shell only.

**§3.3 (kanban #578, 2026-10-08):** the deck and unit-placement model is built here in `deck/`: an 8-card
cycle, a 4-slot hand, a DECK screen and a UNITS placement grid, gated and written back through the bridge. It
is headless and tested (`make test`). Not yet rendered: a phone shell has to draw it. Open: where that shell lives,
and whether ECOWAR's own client hosts it instead.

**§3.3 wiring (kanban #579, 2026-10-08):** the deck UI now binds to a shared `EmpireFaction` record
(`bridge/faction_state.{h,c}`) instead of owning its own decorum. The BIG_O side loads authoritative decorum
into it and both sides read and write the same numbers. The live feed from a BIG_O server is still missing:
BIG_O has no server-side faction decorum store yet (only the scenario sim `core/sim.h`).

**Why this pair, first:** ECOWAR already has real cards with no UI; BIG_O already has a real
headless sim with no GUI. Both halves of "client ↔ sim" exist independently and neither is
wired to anything yet — this is the cheapest real integration available, not a new build.

- **3.1 — Give ECOWAR's cards somewhere to read faction state from.** Today `ecowar_resolve_card_effect`
  computes a card's magnitude in isolation (I32 decision logic, no external state). BIG_O's
  Attention/Heat module is exactly shaped to be that external state: a headless, PARENA-scalar,
  deterministic rules module. Concretely: expose BIG_O's faction/Attention state as a read (and,
  for some cards, read-write) input to `card_effect_mod.prn`, so playing a card is affected by —
  and affects — which faction currently holds pressure/alignment on the relevant TRAPX-doctrine
  grid cell. This is new wiring, not new game design — both sides already compute the right shape
  of number, they just don't talk yet.
- **3.2 — Build the GUI resource-management layer BIG_O needs, as the "BURNER UI" (founder's own
  name, real-time, this session).** Correction to the first draft of this step: BIG_O does **not**
  have zero GUI — it has a real, already-shipped phone shell (`BigoPhone`,
  `day/packages/common/bigo_phone.h`), a home-screen app grid (`BP_APP_COUNT`, grid-nav cursor)
  with real apps already live: `MESSAGES`, `CONTACTS`, `MAP`, `CAMERA`, `NOTES` (TYLER-spec),
  `LAB`, `CARGO`, `SKILLS`, `LOADOUT`, `WARDROBE`, `STATUS` (BIG_O-specific). This phone *is* the
  "BURNER UI" — the founder's explicit instruction is to run **every** in-game affordance across
  the merged product through it, not just BIG_O's own menus. Concretely:
  - BIG_O's tech-tree/resource-management screen (3.1/3.2's original ask) ships as a **new phone
    app** (`BP_APP_TECHTREE` or similar), not a new screen system — same home-grid, same input
    model (`BP_UP/DOWN/LEFT/RIGHT/SELECT/BACK`), same render path as `LAB`/`CARGO`/`SKILLS` today.
  - ECOWAR's own named gap — no deck/hand UI, no unit-placement input (§3.3) — becomes **phone
    apps too** (e.g. `BP_APP_DECK`, `BP_APP_UNITS`), not a second, ECOWAR-native UI stack built in
    parallel. This is the real, concrete answer to "unify the design language": one phone shell,
    one app-grid home screen, one input model, used by both BIG_O's sim-facing apps and ECOWAR's
    card/unit-facing apps.
  - This reframes open question 3 below from "is tech tree literal" to a now-answered "tech tree
    and units both live as phone apps" — still open: the *content* of those apps (what a tech-tree
    phone screen actually shows) isn't designed yet, only the shell they render through.
- **3.3 — Deck/hand UI + unit placement, wired to the above.** ECOWAR's own already-named gap
  (no deck/hand UI, no in-match card-play input) gets built now with a real reason to exist: cards
  play against/into BIG_O-derived faction state from 3.1, displayed through 3.2's screen. Building
  deck/hand UI before 3.1/3.2 would just be REDGARDEN's RTS move-set over again with new card art.
- **3.4 — Only then:** TRAPX-doctrine generalization (is BIG_O's faction sim really one instance
  of TRAPX's grid/alignment/pressure ruleset, or does it need its own rules that merely resemble
  TRAPX's?), DEADWEIGHT/DEADWEIGHT_2's card-language formalization (should ECOWAR's card effects
  be re-expressed in DEADWEIGHT's `card_rules.c` grammar, gaining its fragmentation-tax-shaped
  mechanics as a side effect?), and REDGARDEN's hero-UI lift (reskin ECOWAR's card presentation
  using `HEROES_VS0.md` stat lines and REDGARDEN's existing UI, rather than ECOWAR growing a third
  independent UI). Each of these is real work with its own open questions (§5) — sequenced after
  3.1-3.3 because they're refinements of an ECOWAR+BIG_O link that doesn't exist yet at all.

## 4) Full `github.com/emilyspringerton/*` inventory — what else is relevant, checked live

Pulled via `gh repo list emilyspringerton --limit 200` this session (91 repos total, real org
listing, not the repo table in the root `CLAUDE.md` which is a hand-maintained summary and can
drift). Relevant hits beyond the six named above:

- **`MONOREPO`** — exists, but is operational glue only (`go.work`, deploy scripts, sudo-queue) —
  **not** a prior attempt at this same merge. No conflict with this doc's plan.
- **`GoblinFoxDragon`** — ECOWAR's README explicitly supersedes GFD's `apps2/battlegrounds_gui` as
  ECOWAR's own original (now-abandoned) interface-fork path. Confirmed unrelated going forward.
- **`WEAKNIGHT_BEDROCK_RACERS`**, **`BRAWLPIT`**, **`SLOWBOT_LEAGUE`** — other PvP/ladder products
  in the same general family (bot pools, Elo, IDUNA `games.Registry`), not part of this specific
  four-repo merge but worth knowing about before building a second bot-pool/ladder pattern from
  scratch in `EMPIRE` — `SLOWBOT_LEAGUE/NORTHSTAR.md`'s own audit of `games.Registry` is the
  precedent to reuse if/when EMPIRE needs ranked matchmaking.
- **`DEADWEIGHT_2`** is real and distinct from `DEADWEIGHT` (confirmed: own repo, own CI, own
  "CRASH_OVERRIDE" identity, v0.3.0 release tag) — not a typo, not the same repo. "DEADWEIGHT 1 +
  2" in the founder's framing means literally both repos' card-language code are in scope.
- Nothing else in the 91-repo list names BIG_O, ECOWAR, TRAPX, or EMPIRE as a target — no other
  repo is already attempting this merge.

## 5) Open questions (real, not resolved here)

1. **Is BIG_O's Attention/Heat module's data shape actually compatible with what ECOWAR's card
   effects need to read/write**, or does 3.1 need a translation layer? Not checked at the bit
   level yet — both are "headless, PARENA-scalar, deterministic," which is necessary but not
   sufficient for a clean read/write bridge.
2. ~~What, specifically, is DEADWEIGHT_2 missing to ship?~~ **Resolved (kanban #580, 2026-10-08):** nothing.
   v0.3.0 shipped 2026-09-25 with Linux and Windows assets, and v0.6.0 is latest. See `audits/deadweight2_ship_audit.md`.
3. ~~Does "BIG_O is the tech tree" mean a literal research/upgrade tree UI?~~ **Resolved, same
   session**: tech tree and ECOWAR's units both render as apps inside the BURNER UI phone shell
   (§3.2). Still open: the actual *content/layout* of a tech-tree phone app and a units phone app
   — not designed, only the shell they'll use is settled.
4. Should the TRAPX-doctrine generalization (3.4) run through the standalone `TRAPX` repo's own
   real ruleset (grid/alignment/pressure/deck-of-operations) directly, or through
   `SHANKPIT/docs2/TRAPX_NORTHSTAR.md`'s derivative city-sim framing (Field Offices, Watchers, K9,
   Fame table) that BIG_O already partially implements? These are two different, only
   partially-overlapping specs sharing one name — flagged, not reconciled here.
5. REDGARDEN still has no auth model (carried over from the prior session's finding) — irrelevant
   to the hero-UI role itself (3.4's asset lift doesn't need REDGARDEN's own account system) but
   worth remembering if REDGARDEN ever needs to run live alongside ECOWAR rather than just donate
   UI assets to it.
