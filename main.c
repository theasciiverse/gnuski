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
#define REFRESH_RATE 5000

int main (int argc, char* argv[])
{
  struct Object player, objects[MAX_OBJECTS];
  unsigned int c = 0, i = 0, maxRows, maxCols, ticker = 0, score = 0,
    distance = 0, speed = 1, style = 0;
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
    objects[i] = makeObject (rand () % 3+1,
			     rand () % maxCols,
			     rand () % (maxRows*4 + maxRows/2));

  /* Menu will go here */
  printw ("GNUSki 0.3 - Skifree clone using NCurses, licensed under the GNU GPL.\n");
  printw ("Press any key to start...");
  refresh ();
  getch ();
  nodelay (stdscr, true); /* No waiting for input from the user */
  while (state == loop)
    {
      switch (c)
	{
	case KEY_LEFT: case 'h': case 'H':
	  facing[0] = '4';
	  facing[1] = '2';
	  break;

	case KEY_RIGHT: case 'l': case 'L':
	  facing[0] = '3';
	  facing[1] = '1';
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

      if (ticker == REFRESH_RATE)
	{
	  clear ();
	  ticker = 0;

	  /* Check for collisions and draw the objects */
	  for (i = 0; i < MAX_OBJECTS; i++)
	    {
	      if (objects[i].type != none && collision (player, objects[i]))
		state = lose;
	      moveObject (&objects[i], facing[1], speed);
	      if (objects[i].y < 0)
		setPosition (&objects[i], rand () % (maxCols*2),
			     rand () % maxRows + maxRows);
	      if (objects[i].x > 2 && objects[i].x < maxCols-2
		  && objects[i].y > 2 && objects[i].y < maxRows-2)
		draw (objects[i], facing[1]);
	    }
	  draw (player, facing[0]);
	  printw ("Time:   0:00:00.00\n");
	  printw ("Dist:     %02im\n", distance);
	  printw ("Speed:    %02im/s\n", speed);
	  printw ("Style:    %4i", style);
	  distance += speed;
	}
      refresh ();
      if (speed > 0)
	ticker++;
      c = getch ();
    }
  clear ();
  if (state == win)
    printw ("You won.");
  else
    printw ("You lost.");
  printw ("Your score was %i. You traveled %im.\nPress any key to continue...",
	  distance+style, distance);
  refresh ();
  nodelay (stdscr, false);
  getch ();

  endwin ();
  return 0;
}
