# Changes from upstream

This is the Asciiverse packaging of GNUSki 0.3 (GPL-2.0-or-later) by
Rudolf Olah, a small curses game in the spirit of SkiFree and its last
release (sourceforge.net/projects/gnuski, 2007). History here starts from
the release tarball (gnuski-0.3.tar.gz, sha256
1b629bd29dd6ad362b56055ccdb4c7ad462ff39d7a0deb915753c2096f5f959d),
tagged `0.3`.

## From the project's patch tracker

- **#2, declare setupColors** (Thierry Moisan, 2023): main.c called it
  without a declaration, an error under GCC 14 and later.

## Ours

- **A real frame clock.** The game stepped the slope once every 5000
  passes of a busy loop, so it ran at full CPU and its speed depended on
  the machine. It now steps every 100 ms and waits for keys in between.
- **Back to straight downhill.** Leaning left or right could never be
  undone: Left from a right lean (and Right from a left lean) now points
  the skier straight down again. Patch #1 on the tracker asked for this
  (2010) but came without code.
- **No skiing through obstacles.** At speed 2 and 3 objects moved two or
  three cells at once and could jump over the skier; they now move a cell
  at a time with a collision check at each. Both of the skier's two cells
  count, not just the left one.
- **New obstacles from both sides.** Objects that leave the top came back
  only from below and to the right, half of them off screen; they now
  come back across a band wide enough to drift into view whichever way
  the skier is heading.
- **A clear start.** Obstacles were scattered over the skier's own
  position, so a run could end in its first second; the start now keeps
  a small patch on and just below the skier free.
- **The timer runs.** "Time" showed 0:00:00.00 throughout.
- **The end screen stays.** A key still held from skiing used to dismiss
  it at once; it now waits 800 ms and drops pending keys first.

## Packaging (new files)

- `asciiverse/build.sh`: static Linux build in Alpine, macOS builds per
  architecture, through upstream's Makefile.
- `asciiverse/stage.sh`: the av archive (binary at the top, no data).
- `asciiverse/nfpm.yaml`: .deb, .rpm, .apk and Arch packages.
- `asciiverse/record-preview.py`: records the catalog preview with a
  skier that reads the screen through pyte; the kept
  take is `asciiverse/gnuski.cast`, best of several (a run ends at the
  first crash).
- `.github/workflows/release.yml`: builds all of it on a tag.

## Not changed

- What 0.3 never had: the Abominable Snow Man its readme mentions, tricks
  (Space), "Style" points and a way to win. A crash ends the run; q quits.
- No Windows build.
