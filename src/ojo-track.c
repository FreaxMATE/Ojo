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

#include "ojo-track.h"

OjoTrack *ojo_track_initialize()
{
   OjoTrack *new ;
   new = malloc (sizeof(OjoTrack)) ;
   new->media = NULL ;
   new->type = UNKNOWN ;
   new->uri = NULL ;
   new->title = NULL ;
   new->artist = NULL ;
   new->album = NULL ;
   return new ;
}

void ojo_track_set_media(OjoTrack *track, libvlc_media_t *new_media)
{
   track->media = libvlc_media_duplicate(new_media) ;
}

void ojo_track_set_type(OjoTrack *track, FileType new_type)
{
   track->type = new_type ;
}

void ojo_track_set_uri(OjoTrack *track, char *new_uri)
{
   if (new_uri != NULL)
   {
      track->uri = calloc (strlen(new_uri)+1, sizeof (char)) ;
      strcpy (track->uri, new_uri) ;
   }
   else
   {
      track->uri = NULL ;
      fprintf(stderr, "SERIOUS ERROR: ojo_track_set_uri() in  ojo-track.c: uri is NULL\n") ;
   }
}

void ojo_track_set_title(OjoTrack *track, char *new_title)
{
   if (new_title != NULL)
   {
      track->title = calloc (strlen(new_title)+1, sizeof (char)) ;
      strcpy (track->title, new_title) ;
   }
   else if (track->uri != NULL)
   {
      track->title = calloc (strlen(basename(track->uri))+1, sizeof (char)) ;
      strcpy (track->title, basename(track->uri)) ;
   }
   else
   {
      track->title = calloc (strlen("<Unknown>")+1, sizeof (char)) ;
      strcpy (track->title, "<Unknown>") ;
   }
}

void ojo_track_set_artist(OjoTrack *track, char *new_artist)
{
   if (new_artist != NULL)
   {
      track->artist = calloc (strlen(new_artist)+1, sizeof (char)) ;
      strcpy (track->artist, new_artist) ;
   }
   else
   {
      track->artist = calloc (strlen("<Unknown>")+1, sizeof (char)) ;
      strcpy (track->artist, "<Unknown>") ;
   }
}

void ojo_track_set_album(OjoTrack *track, char *new_album)
{
   if (new_album != NULL)
   {
      track->album = calloc (strlen(new_album)+1, sizeof (char)) ;
      strcpy (track->album, new_album) ;
   }
   else
   {
      track->album = calloc (strlen("<Unknown>")+1, sizeof (char)) ;
      strcpy (track->album, "<Unknown>") ;
   }
}



