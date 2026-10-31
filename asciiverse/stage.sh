#!/bin/sh
# Lays out the av archive in stage/: GNUSki has no data files, so the
# binary sits at the top with its licence and readme.
set -eu
rm -rf stage && mkdir -p stage
cp gnuski stage/gnuski
cp GPL.txt readme PATCHES.md stage/
