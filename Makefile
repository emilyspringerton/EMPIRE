# EMPIRE -- composition layer for ECOWAR + BIG_O (+ DEADWEIGHT, TRAPX later). See NORTHSTAR.md.
CC ?= cc
# PARENA_NO_GRAPHICS: the vendored parena_runtime.h otherwise pulls in SDL2; this bridge is headless.
CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Werror -DPARENA_NO_GRAPHICS

VENDOR = vendor/big_o/witness_rules.c

test: build/test_faction_bridge
	./build/test_faction_bridge

build/test_faction_bridge: tests/test_faction_bridge.c bridge/faction_bridge.c $(VENDOR)
	@mkdir -p build
	$(CC) $(CFLAGS) -I vendor/big_o -o $@ tests/test_faction_bridge.c bridge/faction_bridge.c $(VENDOR)

sync-big-o:
	./scripts/sync_big_o.sh

clean:
	rm -rf build

.PHONY: test sync-big-o clean
