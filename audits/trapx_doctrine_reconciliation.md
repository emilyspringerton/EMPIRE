# TRAPX doctrine reconciliation (kanban #581, EMPIRE NORTHSTAR open Q4)

**Question:** should the TRAPX-doctrine generalization (§3.4) run through the standalone `TRAPX` repo, or through
`SHANKPIT/docs2/TRAPX_NORTHSTAR.md`? The two share a name and some vocabulary but are not the same spec.

**Recommendation:** the standalone `TRAPX` repo is the doctrine. The SHANKPIT document is a separate product
concept that uses the TRAPX name. Treat them as two specs, not one with two versions.

## The two specs

| | Standalone `TRAPX` (`README.md` "BLOCK ZERO" + `index.html` prototype) | `SHANKPIT/docs2/TRAPX_NORTHSTAR.md` |
|---|---|---|
| What it is | Single-player systemic sim on one neighborhood grid. The player intervenes only with cards. Frozen v1 acceptance criteria. | A voxel city (Detroit VS0) with an RPG, FIELDOFFICE claims, Watchers, cops, media, K9, and a doom clock. |
| Core state | Per cell: alignment (Neutral/Player/Rival/Institutional), signed pressure, stability 0–1, population 0–255 | Per district: Flow, Pressure, Claim, Field Office; Narrative Pressure; Tech Pressure tiers |
| Player input | Hand of exactly 5 operation cards, each with cost, cooldown, and placement rules | Embodied: jobs, quests, class unlocks, a flip-phone HUD |
| Win/lose | No victory. The game ends on neighborhood collapse or on exit. | Milestone ladder M0–M9 to shipping |
| Non-goals | No avatar, no direct control, no combat, no quests, no RPG | Is itself the RPG/combat product |
| Faction model | Alignment per cell, four values | Five named factions with Fame tiers (`server/fame`) |

Both use the word "pressure", and the standalone README's mapping table names the same concepts ("Corrupted" ↔
"Institutional Pressure"). The state shapes and rules are still different, so the shared word is not a shared
mechanic.

## Discrepancies inside the standalone repo

1. **Grid size.** The README says "10×10 or 12×12". `index.html` sets `GRID_SIZE = 20`. The code is the one that
   runs, so either the README or the prototype needs fixing. This is recorded here, not silently picked.
2. **Tick interval.** The README says "e.g. every 2 seconds", which is an example, not a rule. The prototype runs
   at `SIM_INTERVAL = 1200` (changed from 250 ms in commit `a0ff668`). Not a conflict.
3. **Hand size.** The README says exactly 5 cards. The prototype has five (Outreach, Investment, Enforcement,
   Logistics, Suppression). Consistent.
4. **Warmup.** The README says "N ticks, e.g. 50–100". The prototype uses 80. Consistent.

## What §3.4 should and should not do

- **Generalize from the standalone doctrine only.** Its rules are frozen and explicit: a cell grid, alignment and
  pressure, cards, no win condition.
- **BIG_O is not yet an instance of it.** BIG_O's live faction rule is the witness/decorum model
  (`core/witness_rules`), not the cell alignment/pressure grid. Making BIG_O "an instance of TRAPX" would be a new
  design decision, not a finding. Record it as not done.
- **The SHANKPIT TRAPX document is out of scope for EMPIRE's doctrine.** Its Flow/Pressure/Claim model is a
  separate, larger product that shares only the name. Do not use it as the doctrine source.

## Still needs a founder decision

- Confirm the standalone repo is the doctrine and the SHANKPIT doc is a separate concept.
- Resolve the 10/12 vs 20 grid conflict: which value is canonical.
- Decide whether BIG_O's faction sim should adopt the standalone cell model. Until then §3.4 stays open.
