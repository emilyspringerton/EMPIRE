# EMPIRE -- composition layer for ECOWAR + BIG_O (+ DEADWEIGHT, TRAPX later). See NORTHSTAR.md.
CC ?= cc
# PARENA_NO_GRAPHICS: the vendored parena_runtime.h otherwise pulls in SDL2; this bridge is headless.
CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Werror -DPARENA_NO_GRAPHICS

VENDOR = vendor/big_o/witness_rules.c

test: build/test_faction_bridge build/test_faction_state build/test_deck build/test_bit_compat
	./build/test_faction_bridge
	./build/test_faction_state
	./build/test_deck
	./build/test_bit_compat

build/test_faction_state: tests/test_faction_state.c bridge/faction_state.c bridge/faction_bridge.c $(VENDOR)
	@mkdir -p build
	$(CC) $(CFLAGS) -I vendor/big_o -o $@ tests/test_faction_state.c bridge/faction_state.c bridge/faction_bridge.c $(VENDOR)

build/test_deck: tests/test_deck.c deck/deck.c deck/ui.c bridge/faction_state.c bridge/faction_bridge.c $(VENDOR)
	@mkdir -p build
	$(CC) $(CFLAGS) -I vendor/big_o -o $@ tests/test_deck.c deck/deck.c deck/ui.c bridge/faction_state.c bridge/faction_bridge.c $(VENDOR)

build/test_faction_bridge: tests/test_faction_bridge.c bridge/faction_bridge.c $(VENDOR)
	@mkdir -p build
	$(CC) $(CFLAGS) -I vendor/big_o -o $@ tests/test_faction_bridge.c bridge/faction_bridge.c $(VENDOR)

sync-big-o:
	./scripts/sync_big_o.sh

clean:
	rm -rf build

.PHONY: test sync-big-o clean

# Bit-level compat with ECOWAR's real compiled card mod (kanban #582). Needs a sibling ECOWAR checkout.
ECOWAR_DIR ?= ../ECOWAR
compat: build/test_bit_compat_ecowar
	./build/test_bit_compat_ecowar

build/test_bit_compat: tests/test_bit_compat.c bridge/faction_bridge.c deck/deck.c $(VENDOR)
	@mkdir -p build
	$(CC) $(CFLAGS) -I vendor/big_o -o $@ tests/test_bit_compat.c bridge/faction_bridge.c deck/deck.c $(VENDOR)

build/test_bit_compat_ecowar: tests/test_bit_compat.c bridge/faction_bridge.c deck/deck.c $(VENDOR) $(ECOWAR_DIR)/packages/simulation/card_effect_mod.c
	@mkdir -p build
	$(CC) $(CFLAGS) -DECOWAR_CARD_MOD -I vendor/big_o -I $(ECOWAR_DIR)/packages/simulation -o $@ tests/test_bit_compat.c bridge/faction_bridge.c deck/deck.c $(VENDOR) $(ECOWAR_DIR)/packages/simulation/card_effect_mod.c

.PHONY: compat
