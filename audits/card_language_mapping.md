# ECOWAR effects in DEADWEIGHT's card grammar (kanban #583, EMPIRE NORTHSTAR §3.4)

**Question:** can ECOWAR's card effects be re-expressed in DEADWEIGHT's `card_rules.prn` grammar?

**Answer:** only partly. Three of ECOWAR's five effect types map to a DEADWEIGHT channel, two do not, and the
MYTHIC tier scaling has no equivalent. Closing the gap means changing DEADWEIGHT's grammar. That changes two shipped
games, so it is a founder decision, not a mechanical port. This doc records the mapping and the gaps.

## Source of each side

- ECOWAR: `packages/simulation/arena_game.c` `ECOWAR_CARDS` and `PARENA/stdlib/ecowar/card_effect_mod.prn`.
  Effect types: DAMAGE, HEAL, SLOW, SILENCE, FLOW. MYTHIC cards get +50% magnitude. Durations are in ms.
- DEADWEIGHT: `PARENA/stdlib/deadweight/card_rules.prn` (the header lists channels 01–43, conditions 00–21,
  modes 0–9). An effect word is decimal `CCNNPPMAA`: channel, condition, condition parameter, amount mode,
  amount. Durations are in rounds.

## Mapping, effect by effect

| ECOWAR effect | Nearest DEADWEIGHT channel | Fit | Gap |
|---|---|---|---|
| DAMAGE (armor-reduced damage) | 01 BONUS (extra damage) | Partial | BONUS only lands inside an exchange and is zeroed when countered. ECOWAR damage is standalone. |
| HEAL | 20 HEAL | Exact | MYTHIC +50% needs a tier mode the grammar lacks. |
| FLOW (resource gain) | 23 ENERGY (self +) | Partial | ENERGY is DEADWEIGHT's own energy resource, not ECOWAR's Flow. |
| SLOW (`slowed_ms`) | none | **No** | No channel slows an opponent's speed. 34 DISABLE is hand slots, not movement. |
| SILENCE (`silenced_ms`) | none | **No** | No channel disables abilities. 34 DISABLE is hand slots. |
| MYTHIC scaling (+50%) | none | **No** | The grammar's modes (0–9) key on AA, energy, cost, round, hull, credits. None keys on a card's tier. |
| Duration units (ms vs rounds) | BURN / REGEN encode rounds in AA | Mismatch | ECOWAR durations are real ms and need a clock. |
| BIG_O decorum gate | none | Out of scope | Lives in EMPIRE's bridge (`bridge/faction_bridge`), not in the card grammar. |

## What closing the gap would take (not done)

1. Add a SLOW channel and a SILENCE channel (or confirm DEADWEIGHT doesn't need them).
2. Add a tier mode, or a card-tier condition, so MYTHIC scaling is expressible.
3. Decide whether durations are rounds or ms, and convert one side.
4. Regenerate DEADWEIGHT's parity vectors. Both the C and Java emitters need to agree again.

## Why this is a decision, not a port

- Items 1–4 change the shipped DEADWEIGHT grammar, which the Android and Windows clients depend on.
- The earlier fusion design (EMPIRE git history, commit `61b1477`, its question 5) already asks whether ECOWAR and
  DEADWEIGHT should share one card engine or two engines that share a faction convention. This mapping can't answer that without the founder.

## Recommendation

Keep ECOWAR's card system in its own PARENA mod for now (`card_effect_mod.prn`). Only the shared faction layer goes
through EMPIRE (the bridge and the deck). Revisit the grammar merge once Q5 is answered.
