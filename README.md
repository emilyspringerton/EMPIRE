# EMPIRE

Composition layer for the EMPIRE stack: ECOWAR (the game), BIG_O (faction/tech-tree sim), DEADWEIGHT
(card language), TRAPX (doctrine). Each source repo keeps developing its own layer; the integration
code lives here. See `NORTHSTAR.md` for the scope and merge order.

## What is here

- `bridge/faction_bridge.{h,c}` — ECOWAR card play gated and written back through BIG_O's decorum rules
  (NORTHSTAR §3.1, kanban #576). CANCELLED casters can't play a card; a successful cast spends or earns
  decorum. The MYTHIC/MUNDANE mapping (-25 / +1) is an invented v1 tuning pending founder sign-off.
- `deck/deck.{h,c}` — the deck model: an 8-card cycle of ECOWAR card ids, a 4-slot hand, and Clash Royale
  style refill (playing a slot draws the next card in the cycle). Also the MYTHIC lookup, copied from
  `card_effect_mod.prn`.
- `deck/ui.{h,c}` — the deck and unit-placement apps (NORTHSTAR §3.3, kanban #578). Pure input -> state ->
  effect, the same shape as BIG_O's `bigo_phone.h`. DECK picks a card; UNITS places it on a 3x6 grid
  (invented v1 layout). SELECT is gated on the faction's decorum, and a committed placement writes decorum
  back through the bridge. BACK cancels with nothing spent.
- `vendor/big_o/` — BIG_O's PARENA-generated `witness_rules.c/.h` plus `parena_runtime.h`, copied verbatim.
  `MANIFEST.sha256` records their hashes. Re-sync with `make sync-big-o` (needs a sibling `../BIG_O`
  checkout, or set `BIG_O_DIR`). Never edit `vendor/` by hand.

## Build and test

```bash
make test        # headless; builds with -DPARENA_NO_GRAPHICS so no SDL2 is needed
```

## Status and limits

- Headless tests: 11 bridge checks and 24 deck/UI checks, all passing.
- The deck/UI is an input model only. Nothing renders it yet. A phone shell (BIG_O's `BigoPhone` or a future one) must
  draw it and feed it actions. No costs, cooldowns or elixir-style economy are modelled yet.
- ECOWAR still carries its own vendored copy of the same gate and write-back (ECOWAR `aff6ce0`). Moving
  that consumer onto this module is not done yet; it needs a decision on how ECOWAR depends on EMPIRE.
- Decorum is not networked and has no HUD yet.
