/*
 * Copyright (c) 2019 FreaxMATE
 *
 * This file is part of Ojo.
 *
 * Ojo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ojo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ojo.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _window_h_
#define _window_h_

#include "vlcPlayer.h"

GtkWidget *window ;
GtkWidget *box ;

void setupWindow(void) ;
void destroy(void) ;
void onOpen(GtkWidget *widget, gpointer data) ;
GtkWidget *getWindow(void) ;
GtkWidget *getBox(void) ;
void setTitle(char *trackName) ;
void onAbout(GtkWidget *widget, gpointer data) ;

#endif
