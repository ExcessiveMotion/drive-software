#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
VENV="$ROOT/.venv"

if [[ ! -d "$VENV" ]]; then
  python3 -m venv "$VENV"
  source "$VENV/bin/activate"
  pip install -r "$ROOT/requirements.txt"
else
  echo "Virtual environment already exists"
fi
