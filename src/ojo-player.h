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

#ifndef _ojo_player_h_
#define _ojo_player_h_

#include <vlc/vlc.h>

#include "ojo.h"
#include "ojo-track.h"
#include "ojo-window.h"

typedef struct _OjoPlayer
{
   libvlc_instance_t *inst ;
   libvlc_media_player_t *media_player ;
   OjoTrack **tracks ;
   int n_tracks ;
   int64_t duration ;
   int media_index ;
} OjoPlayer ;

OjoPlayer *ojo_player_initialize(void) ;
void ojo_player_quit(void) ;
void ojo_player_tracks_free(void) ;
void ojo_player_media_open(GSList *list, int n_tracks, int add) ;
int ojo_player_media_play(int index) ;

void ojo_player_play(void) ;
void ojo_player_pause(void) ;
void ojo_player_stop(void) ;
int64_t ojo_player_get_duration(void) ;
int64_t ojo_player_get_current_time(void) ;
void ojo_player_set_current_time(double time) ;
int ojo_player_get_n_tracks(void) ;
char *ojo_player_get_title(int index) ;
char *ojo_player_get_album(void) ;
char *ojo_player_get_artist(void) ;
FileType ojo_player_get_filetype_by_index(int index) ;
FileType ojo_player_get_filetype() ;
gboolean ojo_player_is_playing() ;
void ojo_player_prev_track() ;
void ojo_player_next_track() ;
void ojo_player_backward() ;
void ojo_player_forward() ;
gboolean ojo_player_end_reached() ;
int ojo_player_get_media_index() ;
libvlc_media_player_t *ojo_player_get_media_player() ;

#endif /* _ojo_player_h_ */

