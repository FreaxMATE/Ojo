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

#ifndef _ojo_track_h_
#define _ojo_track_h_

#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <vlc/vlc.h>

#include "ojo.h"
#include "ojo-player.h"

typedef struct _OjoTrack
{
   libvlc_media_t *media ;
   FileType type ;
   char *uri ;
   char *title ;
   char *artist ;
   char *album ;
} OjoTrack ;

OjoTrack *ojo_track_initialize() ;
void ojo_track_set_media(OjoTrack *track, libvlc_media_t *new_media) ;
void ojo_track_set_type(OjoTrack *track, FileType new_type) ;
void ojo_track_set_uri(OjoTrack *track, char *new_uri) ;
void ojo_track_set_title(OjoTrack *track, char *new_title) ;
void ojo_track_set_artist(OjoTrack *track, char *new_artist) ;
void ojo_track_set_album(OjoTrack *track, char *new_album) ;

#endif /* _ojo_track_h_ */

