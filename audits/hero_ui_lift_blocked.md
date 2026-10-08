# REDGARDEN hero-UI lift (kanban #584, EMPIRE NORTHSTAR §3.4): blocked on missing source

**Card:** pull `HEROES_VS0.md` stat lines and UI into ECOWAR's card presentation.

**Status: blocked. Not built.** Two facts make the lift impossible as written:

1. **The 16 ECOWAR card heroes have no stat lines in `REDGARDEN/docs/HEROES_VS0.md`.** That file covers REDGARDEN's
   own kits: The Duck, The Unicorn, The Ghost, The Frog, The Tree, The Pizza, Buer, TYLER, Flamel, Loki, Gary and
   others. Checked by name: Senior Archivist, ARCHIVIST-7, Stolas, Amon, Andrealphus, Bael, Furfur, Astaroth, Agares,
   Marchosias, Nidhogg, Wren-3, MOOR-8 and the rest do not appear. Only Nidhogg appears, in passing.
2. **The source document ECOWAR cites is not in this checkout.** `packages/simulation/arena_game.c` and
   `PARENA/stdlib/ecowar/card_effect_mod.prn` both name `TYLER/multiverse_heroes.md` as the source of each card's
   hero flavor and MUNDANE/MYTHIC tier. `multiverse_heroes.md` is absent from `~/TYLER` and from the whole monorepo.
   Its stat lines, if any, cannot be read.

## Why not invent the stats

Writing stat lines for these 16 heroes would be a design decision, not a lift. Filling them from memory or from the
card names would publish invented numbers as if they came from the hero bible. The repo's own honesty bar
(`ECOWAR/README.md`, `card_effect_mod.prn`) rules that out.

## What is safe to do now (not done)

- Build the card-frame data (name, source hero, tier, effect kind, base magnitude) from ECOWAR's real catalog only.
  That needs no hero stats and could feed the phone shell with the same shape as `deck/ui`.

## What unblocks it

- The founder restores `TYLER/multiverse_heroes.md` (or says where it lives now), and the stat lines are read from it.
- Or the founder decides the 16 ECOWAR heroes get their own stat lines written as new content. That is a design
  call, so it stays with the founder.
