#!/usr/bin/env bash
# Re-vendors BIG_O's PARENA-generated witness rules into vendor/big_o/ and records their sha256.
# Source of truth is BIG_O/core (generated from PARENA/stdlib/big_o/witness_rules.prn). Never edit vendor/ by hand.
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC="${BIG_O_DIR:-$ROOT/../BIG_O}"
mkdir -p "$ROOT/vendor/big_o"
cp "$SRC/core/witness_rules.c" "$SRC/core/witness_rules.h" "$SRC/core/runtime/parena_runtime.h" "$ROOT/vendor/big_o/"
(cd "$ROOT/vendor/big_o" && sha256sum witness_rules.c witness_rules.h parena_runtime.h) > "$ROOT/vendor/big_o/MANIFEST.sha256"
echo "synced from $SRC"
