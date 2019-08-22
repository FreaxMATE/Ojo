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

#ifndef _widget_h_
#define _widget_h_

#include <math.h>

#include "window.h"
#include "vlcPlayer.h"

GtkWidget *playpauseButton ;
GtkWidget *seekForwardButton ;
GtkWidget *seekBackwardButton ;
GtkWidget *image ;
GtkWidget *playerWidget ;
GtkWidget *buttonBox ;
GtkWidget *controlBox ;
GtkWidget *progressBox ;
GtkWidget *barBox ;
GtkWidget *stopButton ;
GtkWidget *progressBar ;
GtkWidget *timeLabel ;
char string[16] ;

void setupWidgets(void) ;
void setButtonIcon(char *iconName) ;
GtkWidget *getPlayerWidget() ;
gboolean updateBar(void) ;
char *timeToString(double currentTime, double duration) ;

#endif
