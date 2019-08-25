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

#ifndef _vlc_player_h_
#define _vlc_player_h_

#include <vlc/vlc.h>

#include "window.h"

libvlc_media_player_t *mediaPlayer ;
libvlc_media_t *media ;
libvlc_instance_t *vlcInst ;
int64_t duration ;

struct metaData
{
   char title[64] ;

} metaData ;

void initVlc(GtkWidget *player_widget) ;
void quitVlc(void) ;
void playerWidgetOnRealize(GtkWidget *widget) ;
void openMedia(const char* uri) ;
void on_ojo_play_pause_clicked(void) ;
void playPlayer(void) ;
void pausePlayer(void) ;
int64_t getDuration(void) ;
int64_t getCurrentTime(void) ;
void startProgressBar(void) ;
double getVolumeLevel() ;

#endif /* _vlc_player_h_ */
