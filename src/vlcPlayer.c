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

Vlc *vlc_instance_new()
{
   return malloc (sizeof(Vlc)) ;
}

void init_vlc()
{
   vlc = vlc_instance_new() ;
   vlc->inst = libvlc_new(0, NULL) ;
   vlc->media_player = libvlc_media_player_new(vlc->inst) ;
}

void quit_vlc()
{
   free(settings) ;
   libvlc_media_player_release(vlc->media_player) ;
   libvlc_release(vlc->inst) ;
}

Track *track_new()
{
   return malloc(sizeof(Track)) ;
}

void open_media(GSList *list, int n_tracks, int add)
{
   int i = 0 ;
   if (add == FALSE)
   {
      vlc->tracks = calloc(n_tracks, sizeof(Track **)) ;
   }
   else
   {
      vlc->tracks = realloc(vlc->tracks, (vlc->n_tracks+n_tracks)*sizeof(Track **)) ;
      i = vlc->n_tracks ;
   }

   if (vlc->tracks == NULL)
   {
      fprintf (stderr, "ERROR: open_media() in vlcPlayer.c: vlc->tracks calloc/realloc returned NULL\n") ;
      return ;
   }

   libvlc_media_player_set_xwindow(vlc->media_player, GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(player_widget)))) ;
   // initalize every track
   while (list != NULL)
   {
      vlc->tracks[i] = track_new() ;
      strcpy(vlc->tracks[i]->uri, list->data) ;printf ("URI: %s\n", vlc->tracks[i]->uri) ;
      vlc->tracks[i]->media = libvlc_media_new_path(vlc->inst, vlc->tracks[i]->uri) ;
      if (vlc->tracks[i]->media == NULL)
      {
         fprintf (stderr, "ERROR: open_media() in vlcPlayer.c: on opening media check uri\n") ;
         return ;
      }
      vlc->tracks[i]->title = libvlc_media_get_meta(vlc->tracks[i]->media, libvlc_meta_Title) ;
      i++ ;
      list = list->next ;
   }
   vlc->n_tracks = i ;

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


