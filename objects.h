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

#ifndef OBJECTS_H
#define OBJECTS_H

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

enum objectType { none, tree, rock, hill, skier, bigfoot };

struct Object
{
  enum objectType type;
  int x, y;
};

struct Object makeObject (enum objectType type, int x, int y);
void setPosition (struct Object* o, int x, int y);

/* Move the object in a certain facing/direction */
/* facing is one of the following characters:
   1 n 2
   w   e
   4 s 3
*/
void moveObject (struct Object* o, char facing, int speed);

void draw (struct Object o, char facing);

/* Checks for collision between a player object and a target */
int collision (struct Object player, struct Object target);

#endif
