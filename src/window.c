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

gboolean fullscreen = FALSE ;
gboolean dark_theme = FALSE;

void on_ojo_menu_open_activate()
{
	gtk_dialog_run(GTK_DIALOG(filechooser_dialog)) ;
}

void on_ojo_filechooser_cancel_clicked()
{
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;
}

void on_ojo_filechooser_open_clicked()
{
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;
   char *uri ;
   uri = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(filechooser_dialog)) ;
   openMedia(uri) ;
   g_free(uri) ;
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
	about_dialog_response = gtk_dialog_run(GTK_DIALOG(about)) ;
}

void on_ojo_onAbout_response()
{
   gtk_widget_hide (GTK_WIDGET(about)) ;
}

gboolean updateBar()
{
   double currentTime = (double)getCurrentTime() ; // in ms
   double duration = (double)getDuration() ;       // in ms

   gtk_label_set_text(GTK_LABEL(timeLabel), timeToString(currentTime, duration)) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, duration) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), currentTime) ;

   

   return TRUE ;
}

void start_seek_bar()
{
   g_timeout_add_seconds(1, G_SOURCE_FUNC(updateBar), NULL) ;
}

void on_ojo_seek_bar_value_changed()
{
   if (gtk_range_get_value(GTK_RANGE(seek_bar)) != (double)getCurrentTime())
      setCurrentTime(gtk_range_get_value(GTK_RANGE(seek_bar))) ;
}

void on_ojo_seek_bar_button_press_event()
{
   pausePlayer() ;
}

void on_ojo_seek_bar_button_release_event()
{
   playPlayer() ;
}

void on_ojo_fullscreen_clicked()
{
   if (fullscreen == TRUE)
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu_fullscreen)) == TRUE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_fullscreen), FALSE) ;
      gtk_window_unfullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image (GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON)) ;
      fullscreen = FALSE ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image (GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-restore", GTK_ICON_SIZE_BUTTON)) ;
      fullscreen = TRUE ;
   }
}

void on_ojo_menu_fullscreen_toggled()
{
   on_ojo_fullscreen_clicked() ;
}

void on_ojo_preferences_apply_clicked()
{
    if (dark_theme)
        g_object_set (gtk_settings_get_default (), "gtk-application-prefer-dark-theme", TRUE, NULL);
    else if (!dark_theme)
        g_object_set (gtk_settings_get_default (), "gtk-application-prefer-dark-theme", FALSE, NULL);
    gtk_widget_hide (GTK_WIDGET(preferences_dialog)) ;
}

void on_ojo_preferences_close_clicked()
{
    (dark_theme == TRUE) ? (dark_theme = TRUE) : (dark_theme = FALSE) ;
    on_ojo_preferences_apply_clicked() ;
}

void on_ojo_menu_preferences_activate()
{
    gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;
}

void on_ojo_preferences_dark_mode_toggled()
{
    (dark_theme == TRUE) ? (dark_theme = FALSE) : (dark_theme = TRUE) ;
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
   menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_fullscreen")) ;
   fileitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_submenu")) ;
   filemenuOpenitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;
   seek_bar = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_seek_bar")) ;
   volumeButton = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_volume")) ;
   fullscreen_button = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_fullscreen")) ;
   playpauseButton = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause")) ;
   timeLabel = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_time_lbl")) ;
   about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_onAbout")) ;
   preferences_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_preferences_dialog")) ;
   filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_filechooser_dialog")) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, 60.0) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), 0.0) ;

   g_object_unref(builder) ;
}


