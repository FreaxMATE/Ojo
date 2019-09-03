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
gboolean dark_theme = FALSE ;


/*
 *   FILECHOOSER dialog
 */
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
   open_media(uri) ;
   g_free(uri) ;
}


/*
 *   PLAYBACK CONTROL
 */
void on_ojo_play_pause_clicked()
{
   if(libvlc_media_player_is_playing(vlc.media_player) == 1)
   {
      pause_player() ;
   }
   else
   {
      play_player() ;
   }
}

void on_ojo_forw_clicked()
{
   libvlc_media_player_set_position(vlc.media_player, libvlc_media_player_get_position(vlc.media_player)+0.05) ;
}

void on_ojo_stop_clicked()
{
   pause_player() ;
   libvlc_media_player_stop(vlc.media_player) ;
}

void on_ojo_prev_clicked()
{
   libvlc_media_player_set_position(vlc.media_player, libvlc_media_player_get_position(vlc.media_player)-0.05) ;
}

void on_ojo_volume_value_changed()
{
    double volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(volume_button));
    libvlc_audio_set_volume(vlc.media_player, (int)(100*volume)) ;
}

// FULLSCREEN
void on_ojo_fullscreen_clicked()
{
   if (fullscreen == TRUE)
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == TRUE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), FALSE) ;
      gtk_window_unfullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image(GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON)) ;
      fullscreen = FALSE ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image (GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-restore", GTK_ICON_SIZE_BUTTON)) ;
      fullscreen = TRUE ;
   }
}

void on_ojo_menu_fullscreen_toggled()
{
   on_ojo_fullscreen_clicked() ;
}

// SEEKBAR
gboolean update_bar()
{
   double current_time = (double)get_current_time() ; // in ms
   double duration = (double)get_duration() ;         // in ms

   gtk_label_set_text(GTK_LABEL(time_label), time_to_string(current_time, duration)) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, duration) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), current_time) ;

   return TRUE ;
}

void start_seek_bar()
{
   g_timeout_add_seconds(1, update_bar, NULL) ;
}

void on_ojo_seek_bar_value_changed()
{
   if (gtk_range_get_value(GTK_RANGE(seek_bar)) != (double)get_current_time())
      set_current_time(gtk_range_get_value(GTK_RANGE(seek_bar))) ;
}

void on_ojo_seek_bar_button_press_event()
{
   pause_player() ;
}

void on_ojo_seek_bar_button_release_event()
{
   play_player() ;
}


/*
 *   ABOUT dialog
 */
void on_ojo_menu_about_activate()
{
	about_dialog_response = gtk_dialog_run(GTK_DIALOG(about)) ;
}

void on_ojo_onAbout_response()
{
   gtk_widget_hide(GTK_WIDGET(about)) ;
}


/*
 *   PREFERENCES dialog
 */
void on_ojo_preferences_dark_mode_toggled()
{
    (dark_theme == TRUE) ? (dark_theme = FALSE) : (dark_theme = TRUE) ;
}

void on_ojo_menu_preferences_activate()
{
    gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;
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


/*
 *   WINDOW
 */
void setup_window()
{
   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

   window = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
   gtk_builder_connect_signals(builder, NULL);

   player_widget = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "ojo_drawing_area")) ;
   menu_bar = GTK_MENU_BAR(gtk_builder_get_object(builder, "ojo_menu")) ;
   file_menu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_menu_item")) ;
   view_menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_fullscreen")) ;
   file_submenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_submenu")) ;
   file_menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;
   seek_bar = GTK_SCALE(gtk_builder_get_object(builder, "ojo_seek_bar")) ;
   volume_button = GTK_VOLUME_BUTTON(gtk_builder_get_object(builder, "ojo_volume")) ;
   fullscreen_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_fullscreen")) ;
   playpause_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause")) ;
   time_label = GTK_LABEL(gtk_builder_get_object(builder, "ojo_time_lbl")) ;
   about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_onAbout")) ;
   preferences_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_preferences_dialog")) ;
   filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_filechooser_dialog")) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, 60.0) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), 0.0) ;

   g_object_unref(builder) ;
}

void set_title(char *track_name)
{
   char title[64] ;
   sprintf(title, "Ojo - %s", track_name) ;
   gtk_window_set_title(GTK_WINDOW(window), title) ;
   return ;
}

void on_window_main_destroy()
{
   gtk_main_quit() ;
}


/*
 *   Other
 */
char *time_to_string(double current_time, double duration)
{
   current_time /= 1000 ;
   duration /= 1000 ;

   int cur_minutes = ((int)current_time)/60 ; int all_minutes = ((int)duration)/60 ; 
   int cur_seconds = ((int)current_time)%60 ; int all_seconds = ((int)duration)%60 ;

   if (cur_minutes > 59 || all_minutes > 59)
   {
       int cur_hours = ((int)cur_minutes)/60 ; int all_hours = ((int)all_minutes)/60 ;
       sprintf(time_string, "%02d:%02d:%02d / %02d:%02d:%02d  ", cur_hours, cur_minutes-(cur_hours*60), cur_seconds, all_hours, all_minutes-(all_hours*60), all_seconds) ;
   }
   else
   {
      sprintf(time_string, "%02d:%02d / %02d:%02d  ", cur_minutes, cur_seconds, all_minutes, all_seconds) ;
   }

   return time_string ;
}

