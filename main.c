/*  GNUSki - a clone of the old game Skifree where you race downhill
    avoiding rocks and trees and try to score points by doing some
    tricks.
    Copyright (C) 2007 Rudolf Olah

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "objects.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ncurses.h>

#define MAX_OBJECTS 300
#define FRAME_MS 100            /* one step of the slope every 100 ms */

/* Milliseconds from a fixed point, for the frame clock and the timer */
static long
nowMs (void)
{
  struct timespec t;
  clock_gettime (CLOCK_MONOTONIC, &t);
  return t.tv_sec * 1000L + t.tv_nsec / 1000000;
}

int main (int argc, char* argv[])
{
  struct Object player, objects[MAX_OBJECTS];
  unsigned int i = 0, step, maxRows, maxCols, score = 0,
    distance = 0, speed = 1, style = 0;
  int c = 0;
  long start, next, left, elapsed;
  enum mode {loop, lose, win} state = loop;
  char facing[2] = {'s', 'n'};   /* Player facing, object facing */

  srand ((unsigned)time (NULL)); /* Seed random-number generator */
  initscr ();             /* Start ncurses */
  cbreak ();              /* Allow control breaks (Ctrl+C) */
  noecho ();              /* Don't echo characters to screen */
  keypad (stdscr, true);  /* Allow cursor key usage */
  curs_set (0);           /* Hide the cursor */
  getmaxyx (stdscr, maxRows, maxCols); /* Get coordinates of the terminal */
  setupColors ();

  /* Create objects */
  player = makeObject (skier, maxCols/2, maxRows/2);
  for (i = 0; i < MAX_OBJECTS; i++)
    {
      objects[i] = makeObject (rand () % 3+1,
			       rand () % maxCols,
			       rand () % (maxRows*4 + maxRows/2));
      /* keep the start clear: nothing on or just below the skier */
      if (objects[i].y >= player.y - 3 && objects[i].y <= player.y + 6
	  && objects[i].x >= player.x - 4 && objects[i].x <= player.x + 4)
	objects[i].y += maxRows;
    }

  /* Menu will go here */
  printw ("GNUSki 0.3 - Skifree clone using NCurses, licensed under the GNU GPL.\n");
  printw ("Press any key to start...");
  refresh ();
  getch ();
  start = nowMs ();
  next = start;
  while (state == loop)
    {
      /* Wait for a key until the next frame is due */
      left = next - nowMs ();
      timeout (left > 0 ? (int)left : 0);
      c = getch ();
      switch (c)
	{
	case KEY_LEFT: case 'h': case 'H':
	  if (facing[0] == '3')   /* from right back to straight down */
	    {
	      facing[0] = 's';
	      facing[1] = 'n';
	    }
	  else
	    {
	      facing[0] = '4';
	      facing[1] = '2';
	    }
	  break;

	case KEY_RIGHT: case 'l': case 'L':
	  if (facing[0] == '4')   /* from left back to straight down */
	    {
	      facing[0] = 's';
	      facing[1] = 'n';
	    }
	  else
	    {
	      facing[0] = '3';
	      facing[1] = '1';
	    }
	  break;

	case KEY_UP: case 'k': case 'K':
	  if (speed > 0)
	    speed--;
	  break;

	case KEY_DOWN: case 'j': case 'J':
	  if (speed < 3)
	    speed++;
	  break;

	case 'Q': case 'q':
	  state = lose;
	  break;
	}

      if (state == loop && nowMs () >= next)
	{
	  next += FRAME_MS;
	  if (next < nowMs ())   /* fell behind (suspended?): don't race */
	    next = nowMs () + FRAME_MS;
	  clear ();

	  /* Move the objects one cell at a time, checking for collisions
	     at each, so nothing is skipped over at higher speeds; then
	     draw them */
	  for (i = 0; i < MAX_OBJECTS; i++)
	    {
	      for (step = 0; step < speed; step++)
		{
		  if (objects[i].type != none
		      && collision (player, objects[i]))
		    state = lose;
		  moveObject (&objects[i], facing[1], 1);
		}
	      /* new objects come in below the screen, wide enough to
		 drift into view whichever way the skier is heading */
	      if (objects[i].y < 0)
		setPosition (&objects[i],
			     (int)(rand () % (maxCols + 2*maxRows))
			     - (int)maxRows,
			     rand () % maxRows + maxRows);
	      if (objects[i].x > 2 && objects[i].x < (int)maxCols-2
		  && objects[i].y > 2 && objects[i].y < (int)maxRows-2)
		draw (objects[i], facing[1]);
	    }
	  draw (player, facing[0]);
	  elapsed = (nowMs () - start) / 10;   /* hundredths */
	  printw ("Time:   %ld:%02ld:%02ld.%02ld\n", elapsed / 360000,
		  elapsed / 6000 % 60, elapsed / 100 % 60, elapsed % 100);
	  printw ("Dist:     %02im\n", distance);
	  printw ("Speed:    %02im/s\n", speed);
	  printw ("Style:    %4i", style);
	  distance += speed;
	  refresh ();
	}
    }
  clear ();
  if (state == win)
    printw ("You won.");
  else
    printw ("You lost.");
  printw ("Your score was %i. You traveled %im.\nPress any key to continue...",
	  distance+style, distance);
  refresh ();
  napms (800);    /* a key still held from skiing must not skip this */
  flushinp ();
  timeout (-1);
  getch ();

  endwin ();
  return 0;
}
