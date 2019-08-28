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

void initVlc(GtkWidget *playerWidget)
{
   vlcInst = libvlc_new(0, NULL) ;
   mediaPlayer = libvlc_media_player_new(vlcInst) ;
   libvlc_media_player_set_xwindow(mediaPlayer, GDK_WINDOW_XID(gtk_widget_get_window(playerWidget))) ;
}

void quitVlc()
{
   libvlc_media_player_release(mediaPlayer) ;
   libvlc_release(vlcInst) ;
}

void openMedia(const char* uri)
{
   media = libvlc_media_new_location(vlcInst, uri) ;
   libvlc_media_player_set_media(mediaPlayer, media) ;
   libvlc_audio_set_volume(mediaPlayer, 100) ;
   startProgressBar() ;
   playPlayer() ;
   strcpy(metaData.title, libvlc_media_get_meta(media, libvlc_meta_Title)) ;
   setTitle(metaData.title) ;
   libvlc_media_release(media) ;
}

void playPlayer()
{
   libvlc_media_player_play(mediaPlayer) ;
   gtk_button_set_image (GTK_BUTTON(playpauseButton), gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON)) ;
}

void pausePlayer()
{
   libvlc_media_player_pause(mediaPlayer) ;
   gtk_button_set_image (GTK_BUTTON(playpauseButton), gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON)) ;
}

int64_t getDuration()
{
   return libvlc_media_get_duration(media) ;
   //printf ("Duration = %ld\n", (duration/1000)/60) ;
}

int64_t getCurrentTime()
{
   return libvlc_media_player_get_time(mediaPlayer) ;
}
