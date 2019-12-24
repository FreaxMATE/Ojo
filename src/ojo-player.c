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

#include <vlc/vlc.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "ojo-player.h"

void ojo_player_tracks_initialize (GSList *list, int n_tracks, gboolean add) ;

OjoPlayer *ojo_player_initialize()
{
   OjoPlayer *new ;
   new = malloc (sizeof(OjoPlayer)) ;
   new->inst = libvlc_new(0, NULL) ;
   new->media_player = libvlc_media_player_new(new->inst) ;
   // tracks get initialised as they are opened
   new->n_tracks = 0 ;
   new->duration = 0 ;
   new->media_index = 0 ;
   return new ;
}

void ojo_player_quit()
{
   libvlc_media_player_release(ojo_player->media_player) ;
   libvlc_release(ojo_player->inst) ;
}

void ojo_player_tracks_initialize (GSList *list, int n_tracks, gboolean add)
{
   libvlc_media_track_t ***libvlc_tracks ;
   libvlc_tracks = malloc(sizeof(libvlc_media_track_t ***)) ;
   libvlc_media_parsed_status_t parsed_status ;
   int n_streams = 0, track_index = 0 ;

   if (add == TRUE)
   {
      track_index = ojo_player->n_tracks ;
      ojo_player->n_tracks += n_tracks ;
      ojo_player->tracks = realloc(ojo_player->tracks, (ojo_player->n_tracks)*sizeof(OjoTrack **)) ;
   }
   else
   {
      ojo_player->n_tracks = n_tracks ;
      ojo_player->tracks = calloc(ojo_player->n_tracks, sizeof(OjoTrack **)) ;
   }

   if (ojo_player->tracks == NULL)
   {
      fprintf (stderr, "ERROR: ojo_player_tracks_initialize() in ojo-player.c: player->tracks calloc/realloc returned NULL\n") ;
      return ;
   }
   // initalize every track
   while (list != NULL)
   {
      ojo_player->tracks[track_index] = ojo_track_initialize() ;
      ojo_track_set_uri(ojo_player->tracks[track_index], list->data) ;
      ojo_track_set_media(ojo_player->tracks[track_index], libvlc_media_new_path(ojo_player->inst,
                          ojo_player->tracks[track_index]->uri)) ;
      if (ojo_player->tracks[track_index]->media == NULL)
      {
         fprintf (stderr, "ERROR: ojo_player_tracks_initialize() in ojo-player.c: on opening media check uri\n") ;
         return ;
      }
      if (libvlc_media_parse_with_options(ojo_player->tracks[track_index]->media, libvlc_media_fetch_local, 0) == -1)
      {
         fprintf (stderr, "ERROR: ojo_player_tracks_initialize() in ojo-player.c: media_parse returned -1\n") ;
         return ;
      }
      while ((parsed_status=libvlc_media_get_parsed_status(ojo_player->tracks[track_index]->media))
             != libvlc_media_parsed_status_done)
      {
         if (parsed_status == libvlc_media_parsed_status_skipped ||
             parsed_status == libvlc_media_parsed_status_failed  ||
             parsed_status == libvlc_media_parsed_status_timeout)
         {
            fprintf (stderr, "WARNING: ojo_player_tracks_initialize() in ojo-player.c: failed to fetch metadata\n") ;
            break ;
         }
      }
      ojo_track_set_title(ojo_player->tracks[track_index],
                          libvlc_media_get_meta(ojo_player->tracks[track_index]->media, libvlc_meta_Title)) ;
      ojo_track_set_artist(ojo_player->tracks[track_index],
                          libvlc_media_get_meta(ojo_player->tracks[track_index]->media, libvlc_meta_Artist)) ;
      ojo_track_set_album(ojo_player->tracks[track_index],
                          libvlc_media_get_meta(ojo_player->tracks[track_index]->media, libvlc_meta_Album)) ;

      if ((n_streams = libvlc_media_tracks_get(ojo_player->tracks[track_index]->media, libvlc_tracks)) == 0)
      {
         fprintf (stderr, "WARNING: open_media() in vlcPlayer.c: could not get track description\n") ;
         return ;
      }
      if (libvlc_tracks[0][0]->i_type == libvlc_track_audio)
         ojo_track_set_type(ojo_player->tracks[track_index], AUDIO) ;
      else if (libvlc_tracks[0][0]->i_type == libvlc_track_video)
         ojo_track_set_type(ojo_player->tracks[track_index], VIDEO) ;
      else
      {
         ojo_track_set_type(ojo_player->tracks[track_index], UNKNOWN) ;
         fprintf (stderr, "WARNING: open_media() in vlcPlayer.c: media type neither audio nor video\n") ;
         return ;
      }
      libvlc_media_tracks_release(libvlc_tracks[0], n_streams) ;
      track_index++ ;
      // g_free(list->data) ; TODO: free uris; causes errors
      list = list->next ;
   }
   g_slist_free_full(list, g_free) ;
}

void ojo_player_tracks_free()
{
   int i = 0 ;
   while(i < ojo_player->n_tracks)
   {
      free(ojo_player->tracks[i++]) ;
   }
   free(ojo_player->tracks) ;
}

void ojo_player_media_open (GSList *list, int n_tracks, gboolean add)
{
   ojo_player_tracks_initialize(list, n_tracks, add) ;
   libvlc_media_player_set_xwindow(ojo_player->media_player, GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(drawing_area)))) ;
   ojo_playlist_gtk_initialize() ;
   ojo_player_media_play(0) ;
}

int ojo_player_media_play(int index)
{
   if (index < ojo_player->n_tracks && index >= 0)
   {
      ojo_player->media_index = index ;
      libvlc_media_player_set_media(ojo_player->media_player, ojo_player->tracks[ojo_player->media_index]->media) ;
      ojo_playlist_select_row(ojo_player->media_index) ;
      ojo_player_play() ;
      ojo_window_set_title(ojo_player->tracks[ojo_player->media_index]->title) ;
   }
   else
   {
      fprintf (stderr, "WARNING: play_media() in vlcPlayer.c: index out of track range\n") ;
      return -1 ;
   }
   ojo_window_seek_bar_start() ;
   ojo_window_format_display_for_media() ;

   return 0 ;
}

void ojo_player_play()
{
   libvlc_media_player_play(ojo_player->media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON)) ;
}

void ojo_player_pause()
{
   libvlc_media_player_pause(ojo_player->media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON)) ;
}

void ojo_player_stop()
{
   libvlc_media_player_stop(ojo_player->media_player) ;
}

int64_t ojo_player_get_duration()
{
   return libvlc_media_get_duration(libvlc_media_player_get_media(ojo_player->media_player)) ;
}

int64_t ojo_player_get_current_time()
{
   return libvlc_media_player_get_time(ojo_player->media_player) ;
}

void ojo_player_set_current_time(double time)
{
   libvlc_media_player_set_time(ojo_player->media_player, time) ;
}

int ojo_player_get_n_tracks()
{
   return ojo_player->n_tracks ;
}

char *ojo_player_get_title(int index)
{
   return ojo_player->n_tracks > 0 ? ojo_player->tracks[index]->title : NULL ;
}

char *ojo_player_get_album()
{
   return ojo_player->n_tracks > 0 ? ojo_player->tracks[ojo_player->media_index]->album : NULL ;
}

char *ojo_player_get_artist()
{
   return ojo_player->n_tracks > 0 ? ojo_player->tracks[ojo_player->media_index]->artist : NULL ;
}

FileType ojo_player_get_filetype_by_index(int index)
{
   return ojo_player->n_tracks > 0 ? ojo_player->tracks[index]->type : UNKNOWN ;
}

FileType ojo_player_get_filetype()
{
   return ojo_player->n_tracks > 0 ? ojo_player->tracks[ojo_player->media_index]->type : UNKNOWN ;
}

gboolean ojo_player_is_playing()
{
   if (libvlc_media_player_is_playing(ojo_player->media_player) == 1)
      return TRUE ;
   return FALSE ;
}

void ojo_player_prev_track()
{
   ojo_player->media_index-1 < 0 ? ojo_player_media_play(0) : ojo_player_media_play(ojo_player->media_index-1) ;
}

void ojo_player_next_track()
{
   ojo_player->media_index+1 < ojo_player->n_tracks ? ojo_player_media_play(ojo_player->media_index+1) : ojo_player_media_play(0) ;
}

void ojo_player_backward()
{
   libvlc_media_player_set_position(ojo_player->media_player, libvlc_media_player_get_position(ojo_player->media_player)-0.05) ;
}

void ojo_player_forward()
{
   libvlc_media_player_set_position(ojo_player->media_player, libvlc_media_player_get_position(ojo_player->media_player)+0.05) ;
}

gboolean ojo_player_end_reached()
{
   if (libvlc_media_player_get_state(ojo_player->media_player) == libvlc_Ended)
      return TRUE ;
   return FALSE ;
}

int ojo_player_get_media_index()
{
   return ojo_player->media_index ;
}

libvlc_media_player_t *ojo_player_get_media_player()
{
   return ojo_player->media_player ;
}


