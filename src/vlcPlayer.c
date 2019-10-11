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

#include "vlcPlayer.h"

Vlc *initialise_vlc()
{
   Vlc *new ;
   new = malloc (sizeof(Vlc)) ;
   new->inst = libvlc_new(0, NULL) ;
   new->media_player = libvlc_media_player_new(new->inst) ;
   // tracks get initialised as they are opened
   new->n_tracks = 0 ;
   new->duration = 0 ;
   new->media_index = 0 ;
   return new ;
}

void quit_vlc()
{
   free(settings) ;
   libvlc_media_player_release(vlc->media_player) ;
   libvlc_release(vlc->inst) ;
}

void free_tracks()
{
   int i = 0 ;
   while(i < vlc->n_tracks)
   {
      free(vlc->tracks[i++]) ;
   }
   free(vlc->tracks) ;
}

void initialise_tracks (GSList *list, int n_tracks, gboolean add)
{
   libvlc_media_track_t ***libvlc_tracks ;
   libvlc_tracks = malloc(sizeof(libvlc_media_track_t ***)) ;
   int n_streams = 0, track_index = 0 ;

   if (add == TRUE)
   {
      track_index = vlc->n_tracks ;
      vlc->n_tracks += n_tracks ;
      vlc->tracks = realloc(vlc->tracks, (vlc->n_tracks)*sizeof(Track **)) ;
   }
   else
   {
      vlc->n_tracks = n_tracks ;
      vlc->tracks = calloc(n_tracks, sizeof(Track **)) ;
   }

   if (vlc->tracks == NULL)
   {
      fprintf (stderr, "ERROR: open_media() in vlcPlayer.c: vlc->tracks calloc/realloc returned NULL\n") ;
      return ;
   }
   // initalize every track
   while (list != NULL)
   {
      vlc->tracks[track_index] = malloc(sizeof(Track)) ;
      strcpy(vlc->tracks[track_index]->uri, list->data) ;

      vlc->tracks[track_index]->media = libvlc_media_new_path(vlc->inst, vlc->tracks[track_index]->uri) ;
      if (vlc->tracks[track_index]->media == NULL)
      {
         fprintf (stderr, "ERROR: open_media() in vlcPlayer.c: on opening media check uri\n") ;
         return ;
      }
      if (libvlc_media_parse_with_options(vlc->tracks[track_index]->media, libvlc_media_fetch_local, 0) == -1)
      {
         fprintf (stderr, "ERROR: open_media() in vlcPlayer.c: media_parse returned -1\n") ;
         return ;
      }
      while (libvlc_media_get_parsed_status(vlc->tracks[track_index]->media) != libvlc_media_parsed_status_done)
         ;
      vlc->tracks[track_index]->title = libvlc_media_get_meta(vlc->tracks[track_index]->media, libvlc_meta_Title) ;
      vlc->tracks[track_index]->artist = libvlc_media_get_meta(vlc->tracks[track_index]->media, libvlc_meta_Artist) ;
      vlc->tracks[track_index]->album = libvlc_media_get_meta(vlc->tracks[track_index]->media, libvlc_meta_Album) ;
      if ((n_streams = libvlc_media_tracks_get(vlc->tracks[track_index]->media, libvlc_tracks)) == 0)
      {
         fprintf (stderr, "WARNING: open_media() in vlcPlayer.c: could not get track description\n") ;
         return ;
      }
      if (libvlc_tracks[0][0]->i_type == libvlc_track_audio)
         vlc->tracks[track_index]->type = AUDIO ;
      else if (libvlc_tracks[0][0]->i_type == libvlc_track_video)
         vlc->tracks[track_index]->type = VIDEO ;
      else
      {
         vlc->tracks[track_index]->type = -1 ;
         fprintf (stderr, "WARNING: open_media() in vlcPlayer.c: media type neither audio nor video\n") ;
         return ;
      }
      libvlc_media_tracks_release(libvlc_tracks[0], n_streams) ;
      g_free(list->data) ;
      track_index++ ;
      list = list->next ;
   }
   g_slist_free(list) ;
}

void open_media (GSList *list, int n_tracks, gboolean add)
{
   initialise_tracks(list, n_tracks, add) ;
   libvlc_media_player_set_xwindow(vlc->media_player, GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(drawing_area)))) ;
   initialize_gtk_playlist() ;

   play_media(0) ;
}

int play_media(int index)
{
   if (index < vlc->n_tracks && index >= 0)
   {
      vlc->media_index = index ;
      libvlc_media_player_set_media(vlc->media_player, vlc->tracks[vlc->media_index]->media) ;
      gtk_list_box_select_row (playlist_box, gtk_list_box_get_row_at_index(playlist_box, vlc->media_index)) ;
      play_player() ;
      set_title(vlc->tracks[vlc->media_index]->title) ;
   }
   else
   {
      fprintf (stderr, "WARNING: play_media() in vlcPlayer.c: index out of track range\n") ;
      return -1 ;
   }
   start_seek_bar() ;
   format_display_for_media() ;

   return 0 ;
}

void play_player()
{
   libvlc_media_player_play(vlc->media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON)) ;
}

void pause_player()
{
   libvlc_media_player_pause(vlc->media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON)) ;
}

int64_t get_duration()
{
   return  libvlc_media_get_duration(libvlc_media_player_get_media(vlc->media_player)) ;
}

int64_t get_current_time()
{
   return libvlc_media_player_get_time(vlc->media_player) ;
}

void set_current_time(double time)
{
   libvlc_media_player_set_time(vlc->media_player, time) ;
}

char *get_album()
{
   return vlc->n_tracks > 0 ? vlc->tracks[vlc->media_index]->album : NULL ;
}

char *get_artist()
{
   return vlc->n_tracks > 0 ? vlc->tracks[vlc->media_index]->artist : NULL ;
}


