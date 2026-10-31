# GNUSki (Asciiverse packaging)

GNUSki 0.3 by Rudolf Olah (2007), a small ncurses downhill-skiing game in
the spirit of SkiFree, with fixes, packaged for the Asciiverse catalog and
as system packages. Not our code: keep changes small and list every one in
PATCHES.md.

- No upstream repository (SourceForge has only release tarballs and a
  patch tracker); history starts from the 0.3 tarball (tag `0.3`), then
  tracker patch #2, then ours. `origin` is to be
  github.com/theasciiverse/gnuski.
- Release: tag `v0.3-av.<n>` and push it.
- Local build: `make` (needs libncurses-dev). Static build: `docker run
  --rm -v "$PWD":/src -w /src alpine:3.22 sh asciiverse/build.sh` leaves
  `./gnuski`.
- Not the same game as ESR's `ski` (Python) or asciijump (ski jumping).
- Preview: `python3 asciiverse/record-preview.py` (needs pyte).
