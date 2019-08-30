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

GtkBuilder  *builder; 
GtkWidget   *window ;
GtkWidget   *playerWidget ;
GtkWidget   *seek_bar ;
GtkWidget   *menubar ;
GtkWidget   *filemenu ;
GtkWidget   *fileitem ;
GtkWidget   *filemenuOpenitem ;
GtkWidget   *volumeButton ;
GtkWidget   *timeLabel ;
GtkButton   *playpauseButton ;
GtkDialog	*about ;
int about_dialog_response ;

void setupWindow(void) ;
void destroy(void) ;
void on_ojo_stop_clicked(void) ;
void on_ojo_menu_open_activate() ;
void setTitle(char *trackName) ;
char *timeToString(double currentTime, double duration) ;
char string[32] ;

#endif /* _window_h_ */
