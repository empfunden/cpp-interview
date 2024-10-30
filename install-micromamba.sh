#!/bin/bash

set -euo pipefail

mkdir -p ~/.mamba/pkgs
mkdir -p "$1"
curl -Ls https://micro.mamba.pm/api/micromamba/linux-64/2.0.2 | tar -xvj -C "$1" --strip-components=1 bin/micromamba
