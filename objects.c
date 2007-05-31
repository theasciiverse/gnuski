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

void
setupColors ()
{
  if (has_colors () == false)
    {
      endwin ();
      printf ("*** Your terminal does not support color ***\n");
      exit (1);
    }
  start_color ();
  init_pair (1, COLOR_GREEN, COLOR_BLACK);
  init_pair (2, COLOR_WHITE, COLOR_BLACK);
}

struct Object
makeObject (enum objectType type, int x, int y)
{
  struct Object o;
  o.type = type;
  o.x = x;
  o.y = y;
  return o;
}

void
setPosition (struct Object* o, int x, int y)
{
  o->x = x;
  o->y = y;
}

void
moveObject (struct Object* o, char facing, int speed)
{
  switch (facing)
    {
    case 'n': o->y -= speed; break;
    case 'e': o->x += speed; break;
    case 's': o->y += speed; break;
    case 'w': o->x -= speed; break;
    case '1': o->y -= speed; o->x -= speed; break;
    case '2': o->y -= speed; o->x += speed; break;
    case '3': o->y += speed; o->x += speed; break;
    case '4': o->y += speed; o->x -= speed; break;
    }
}

void
draw (struct Object o, char facing)
{
  switch (o.type)
    {
    case skier:
      switch (facing)
	{
	case 'n':
	case 's':
	  move (o.y, o.x);
	  printw ("||");
	  break;
	case 'w':
	  move (o.y, o.x);
	  printw ("==");
	  break;
	case 'e':
	  move (o.y, o.x+1);
	  printw ("==");
	  break;
	case '1':
	case '3':
	  move (o.y, o.x);
	  printw ("\\\\");
	  break;
	case '2':
	case '4':
	  move (o.y, o.x);
	  printw ("//");
	  break;
	}
      break;
    case tree:
      attron (COLOR_PAIR (1));
      move (o.y, o.x);
      printw ("/|\\");
      move (o.y+1, o.x);
      printw ("/|\\");
      move (o.y+2, o.x);
      printw (" | ");
      attroff (COLOR_PAIR (1));
      break;
    case rock:
      attron (COLOR_PAIR (2));
      move (o.y, o.x);
      printw ("o");
      attroff (COLOR_PAIR (2));
      break;
    case hill:
      move (o.y, o.x);
      printw ("/^\\");
    case none: default:
      break;
    }
  move (0, 0);
}

int
collision (struct Object player, struct Object target)
{
  switch (target.type)
    {
    case tree:
      return player.y == target.y+2 && player.x == target.x+1;
      break;
    case rock:
      return player.y == target.y && player.x == target.x;
      break;
    case none: default:
      return 0;
    }
  return 0;
}
