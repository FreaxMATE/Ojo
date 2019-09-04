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

struct vlc
{
   libvlc_instance_t *inst ;
   libvlc_media_t *media ;
   libvlc_media_player_t *media_player ;
   int64_t duration ;
} vlc ;

struct _meta_data
{
   char title[64] ;

} meta_data ;

void init_vlc(void) ;
void quit_vlc(void) ;
void open_media(const char* uri) ;
void play_player(void) ;
void pause_player(void) ;
int64_t get_duration(void) ;
int64_t get_current_time(void) ;
void start_seek_bar(void) ;
void set_current_time(double time) ;

#endif /* _vlc_player_h_ */

