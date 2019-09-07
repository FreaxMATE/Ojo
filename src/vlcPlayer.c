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

void reload_media()
{
   libvlc_media_player_set_media(vlc.media_player, vlc.media[0]) ;
   pause_player() ;
}

void init_vlc()
{
   vlc.inst = libvlc_new(0, NULL) ;
   vlc.media_player = libvlc_media_player_new(vlc.inst) ;
}

void quit_vlc()
{
   free(settings) ;
   libvlc_media_player_release(vlc.media_player) ;
   libvlc_release(vlc.inst) ;
}

void open_media(Playlist playlist)
{
   int i ;
   char meta_data_buffer[1024] ;
   vlc.media_index = 0 ;

   libvlc_media_player_set_xwindow(vlc.media_player, GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(player_widget)))) ;
   vlc.media_list = libvlc_media_list_new(vlc.inst) ;

   vlc.media = calloc(playlist.n_items, sizeof(libvlc_media_t *)) ;
   meta_data.title = calloc(playlist.n_items, sizeof(char *)) ;
   for (i = 0; i < playlist.n_items; ++i)
   {
      vlc.media[i] = libvlc_media_new_path(vlc.inst, playlist.uri[i]) ;
      libvlc_media_list_add_media(vlc.media_list, vlc.media[i]) ;
   }
   libvlc_media_player_set_media(vlc.media_player, vlc.media[0]) ;
   libvlc_audio_set_volume(vlc.media_player, 100) ;
   play_player() ;
   for (i = 0; i < playlist.n_items; ++i)
   {
      strcpy(meta_data_buffer, libvlc_media_get_meta(vlc.media[i], libvlc_meta_Title)) ;
      meta_data.title[i] = calloc(playlist.n_items, strlen(meta_data_buffer)*sizeof(char)) ;
      strncpy(meta_data.title[i], meta_data_buffer, 57) ;
   }
   set_playlist_item_title() ;
   start_seek_bar() ;
   set_title(meta_data.title[0]) ;
}

void play_media(int index)
{
   vlc.media_index = index ;
   libvlc_media_player_set_media(vlc.media_player, vlc.media[index]) ;
   gtk_list_box_select_row(playlist_box, gtk_list_box_get_row_at_index(playlist_box, vlc.media_index)) ;
   play_player() ;
   set_title(meta_data.title[vlc.media_index]) ;
}

void play_player()
{
   libvlc_media_player_play(vlc.media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON)) ;
}

void pause_player()
{
   libvlc_media_player_pause(vlc.media_player) ;
   gtk_button_set_image (GTK_BUTTON(playpause_button), gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON)) ;
}

int64_t get_duration()
{
   return libvlc_media_get_duration(vlc.media[vlc.media_index]) ;
}

int64_t get_current_time()
{
   return libvlc_media_player_get_time(vlc.media_player) ;
}

void set_current_time(double time)
{
   libvlc_media_player_set_time(vlc.media_player, time) ;
}


