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

#include <gtk/gtk.h>

#include "window.h"

void on_ojo_menu_open_activate()
{
   GtkWidget *dialog ;
   dialog = gtk_file_chooser_dialog_new("Choose Media", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL) ;
   if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
   {
      char *uri ;
      uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog)) ;
      openMedia(uri) ;
      g_free(uri) ;
   }
   gtk_widget_destroy(dialog) ;
}

void on_window_main_destroy()
{
   gtk_main_quit() ;
}

void on_ojo_play_pause_clicked()
{
   if(libvlc_media_player_is_playing(mediaPlayer) == 1)
   {
      pausePlayer() ;
   }
   else
   {
      playPlayer() ;
   }
}


void on_ojo_forw_clicked()
{
   libvlc_media_player_set_position(mediaPlayer, libvlc_media_player_get_position(mediaPlayer)+0.05) ;
}

void on_ojo_stop_clicked()
{
   pausePlayer() ;
   libvlc_media_player_stop(mediaPlayer) ;
}

void on_ojo_prev_clicked()
{
   libvlc_media_player_set_position(mediaPlayer, libvlc_media_player_get_position(mediaPlayer)-0.05) ;
}

void on_ojo_volume_value_changed()
{
    double volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(volumeButton));
    libvlc_audio_set_volume(mediaPlayer, (int)(100*volume)) ;
}

void on_ojo_menu_about_activate()
{
	gtk_dialog_run(GTK_DIALOG(about)) ;
}

void on_ojo_onAbout_close()
{

}

gboolean updateBar()
{
   double currentTime = (double)getCurrentTime() ;
   double duration = (double)getDuration() ;

   gtk_label_set_text(GTK_LABEL(timeLabel), timeToString(currentTime, duration)) ;

   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), currentTime/duration) ;
   if (gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressBar)) == 1)
      return FALSE ;
   return TRUE ;
}

void startProgressBar()
{
   g_timeout_add_seconds(1, G_SOURCE_FUNC(updateBar), NULL) ;
}

char *timeToString(double currentTime, double duration)
{
   currentTime /= 1000 ;
   duration /= 1000 ;

   int curMinutes = ((int)currentTime)/60 ; int allMinutes = ((int)duration)/60 ; 
   int curSeconds = ((int)currentTime)%60 ; int allSeconds = ((int)duration)%60 ;

   if (curMinutes > 59 || allMinutes > 59)
   {
       int curHours = ((int)curMinutes)/60 ; int allHours = ((int)allMinutes)/60 ;
       sprintf(string, "%02d:%02d:%02d / %02d:%02d:%02d  ", curHours, curMinutes-(curHours*60), curSeconds, allHours, allMinutes-(allHours*60), allSeconds) ;
   }
   else
   {
      sprintf(string, "%02d:%02d / %02d:%02d  ", curMinutes, curSeconds, allMinutes, allSeconds) ;
   }

   return string ;
}

void setTitle(char *trackName)
{
   char title[64] ;
   sprintf(title, "Ojo - %s", trackName) ;
   gtk_window_set_title(GTK_WINDOW(window), title) ;
   return ;
}

void setupWindow()
{
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    playerWidget = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_drawing_area")) ;
    menubar = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu")) ;
    filemenu = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_item")) ;
    fileitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_submenu")) ;
    filemenuOpenitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;
    progressBar = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_progress_bar"));
    volumeButton = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_volume"));
    playpauseButton = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause"));
    timeLabel = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_time_lbl"));
	about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_onAbout"));

    g_object_unref(builder);
}
