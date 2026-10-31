#!/usr/bin/env python3
"""Records asciiverse/gnuski.cast, GNUSki's catalog preview, at 100x30
(hub/RECORDING.md): the title line, then about 30 s of skiing by a simple
skier that reads the screen (pyte) and leans away from whatever is in its
path, then q and the score screen. Needs pyte. Usage, from the repo root
after building ./gnuski:
    python3 asciiverse/record-preview.py"""
import os, pty, struct, fcntl, select, termios, time
import pyte

COLS, ROWS = 100, 30
root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
env = dict(os.environ, TERM="xterm-256color", COLORTERM="truecolor", LANG="C.UTF-8")
screen = pyte.Screen(COLS, ROWS)
stream = pyte.ByteStream(screen)

pid, fd = pty.fork()
if pid == 0:
    os.chdir(root)      # so the recording shows no local paths
    os.execvpe("asciinema", ["asciinema", "rec", "--overwrite", "-q", "-i", "2",
        "--window-size", f"{COLS}x{ROWS}", "--capture-env", "TERM,COLORTERM,LANG",
        "-c", "./gnuski", "asciiverse/gnuski.cast"], env)
fcntl.ioctl(fd, termios.TIOCSWINSZ, struct.pack("HHHH", ROWS, COLS, 0, 0))

done = False
def alive():
    global done
    if not done and os.waitpid(pid, os.WNOHANG)[0]: done = True
    return not done

def pump(t):
    end = time.time() + t
    while time.time() < end:
        r, _, _ = select.select([fd], [], [], 0.01)
        if r:
            try: stream.feed(os.read(fd, 65536))
            except OSError: return

def key(k, wait=0.0):
    os.write(fd, k); pump(wait)

# the skier sits at the middle (main.c: maxCols/2, maxRows/2), two cells
# wide; the slope scrolls up past it
SX, SY = COLS // 2, ROWS // 2
LOOK = 12

def clear_run(dx):
    """How many rows down the path for heading dx (-1, 0, 1) stays clear."""
    rows = screen.display
    for k in range(1, LOOK + 1):
        y = SY + k
        if y >= ROWS - 2: return LOOK
        for x in (SX + dx * k - 1, SX + dx * k, SX + dx * k + 1, SX + dx * k + 2):
            if 0 <= x < COLS and rows[y][x] in "/|\\o":
                return k - 1
    return LOOK

pump(2.5)
key(b" ", 0.05)                 # "Press any key to start"
heading = 0
end = time.time() + float(os.environ.get("PLAY_SECONDS", 30))
while time.time() < end and alive():
    pump(0.1)
    if "You lost" in "\n".join(screen.display): break
    runs = {d: clear_run(d) for d in (-1, 0, 1)}
    # stay on course while it is clear; prefer straight down
    if runs[heading] >= LOOK and not (heading and runs[0] >= LOOK):
        continue
    best = max((-1, 0, 1), key=lambda d: (runs[d], d == 0, d == heading))
    while heading != best:      # Left/Right step through -1, 0, 1
        if best < heading: key(b"h", 0.0); heading -= 1
        else: key(b"l", 0.0); heading += 1
pump(0.5)
if alive() and "You lost" not in "\n".join(screen.display):
    key(b"q", 2.5)              # end the run: the score screen
else:
    pump(2.5)
if alive():
    key(b" ", 1.0)
for _ in range(30):
    if not alive(): break
    pump(0.2)
