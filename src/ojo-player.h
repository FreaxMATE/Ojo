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
#include "ojo-controlbox.h"

typedef struct _OjoPlayer
{
   libvlc_instance_t *inst ;
   libvlc_media_player_t *media_player ;
   OjoTrack **tracks ;
   int n_tracks ;
   int64_t duration ;
   int media_index ;
   GRand *rand ;
} OjoPlayer ;

OjoPlayer *ojo_player_initialize(void) ;
void ojo_player_quit(OjoPlayer *ojo_player) ;
void ojo_player_tracks_free(OjoPlayer *ojo_player) ;
void ojo_player_media_open(OjoPlayer *ojo_player, GSList *list, int n_tracks, int add) ;
int ojo_player_media_play(OjoPlayer *ojo_player, int index) ;

void ojo_player_play(OjoPlayer *ojo_player) ;
void ojo_player_pause(OjoPlayer *ojo_player) ;
void ojo_player_stop(OjoPlayer *ojo_player) ;
void ojo_player_prev_track(OjoPlayer *ojo_player) ;
void ojo_player_next_track(OjoPlayer *ojo_player) ;
void ojo_player_backward(OjoPlayer *ojo_player) ;
void ojo_player_forward(OjoPlayer *ojo_player) ;
void ojo_player_random_track(OjoPlayer *ojo_player) ;

int ojo_player_get_n_tracks(OjoPlayer *ojo_player) ;
int64_t ojo_player_get_duration(OjoPlayer *ojo_player) ;
int64_t ojo_player_get_current_time(OjoPlayer *ojo_player) ;
void ojo_player_set_current_time(OjoPlayer *ojo_player, double time) ;
char *ojo_player_get_title_by_index(OjoPlayer *ojo_player, int index) ;
char *ojo_player_get_album(OjoPlayer *ojo_player) ;
char *ojo_player_get_artist(OjoPlayer *ojo_player) ;
FileType ojo_player_get_filetype(OjoPlayer *ojo_player) ;
gboolean ojo_player_is_playing(OjoPlayer *ojo_player) ;
gboolean ojo_player_end_reached(OjoPlayer *ojo_player) ;
int ojo_player_get_media_index(OjoPlayer *ojo_player) ;
libvlc_media_player_t *ojo_player_get_media_player(OjoPlayer *ojo_player) ;
int ojo_player_get_mousepos_x(OjoPlayer *ojo_player) ;
int ojo_player_get_mousepos_y(OjoPlayer *ojo_player) ;
int ojo_player_get_size_x(OjoPlayer *ojo_player) ;
int ojo_player_get_size_y(OjoPlayer *ojo_player) ;
void ojo_player_set_volume(OjoPlayer *ojo_player, double volume) ;

#endif /* _ojo_player_h_ */

