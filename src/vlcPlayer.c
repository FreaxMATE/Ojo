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
   libvlc_media_player_set_media(vlc.media_player, vlc.media) ;pause_player() ;
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

void open_media(const char* uri)
{
   libvlc_media_player_set_xwindow(vlc.media_player, GDK_WINDOW_XID(gtk_widget_get_window(GTK_WIDGET(player_widget)))) ;
   if ((vlc.media = libvlc_media_new_path(vlc.inst, uri)) == NULL)
      fprintf (stderr, "Error: in open_media() vlcPlayer.c: invalid file path\n") ;
   libvlc_media_player_set_media(vlc.media_player, vlc.media) ;
   libvlc_audio_set_volume(vlc.media_player, 100) ;
   play_player() ;
   start_seek_bar() ;
   strncpy(meta_data.title, libvlc_media_get_meta(vlc.media, libvlc_meta_Title), 57) ;
   set_title(meta_data.title) ;
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
   return libvlc_media_get_duration(vlc.media) ;
}

int64_t get_current_time()
{
   return libvlc_media_player_get_time(vlc.media_player) ;
}

void set_current_time(double time)
{
   libvlc_media_player_set_time(vlc.media_player, time) ;
}


