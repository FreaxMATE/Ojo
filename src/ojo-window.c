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
#include <math.h>

#include "ojo-window.h"


void ojo_window_set_art_cover_image(char *artist, char *album)
{
   char uri[1024] ;

   sprintf (uri, "/home/%s/.cache/vlc/art/artistalbum/%s/%s/art.jpg", getenv("USER"), artist, album) ;
   if(g_file_test(uri, G_FILE_TEST_EXISTS) && ojo_settings_get_boolean(ojo_settings->gsettings, "view-coverart"))
      gtk_image_set_from_file(background_image, uri) ;
   else
      gtk_image_set_from_icon_name(background_image, "audio-x-generic", GTK_ICON_SIZE_DIALOG) ;
}

void ojo_window_media_open_prepare(GSList *uri_list, gboolean add)
{
   n_tracks = g_slist_length(uri_list) ;
   ojo_window_set_track_control_visibility(n_tracks) ;
   media_already_opened = TRUE ;
   ojo_player_media_open(uri_list, n_tracks, add) ;
}

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

void on_ojo_filechooser_add_clicked()
{
   if (media_already_opened == FALSE)
   {
      on_ojo_filechooser_open_clicked() ;
      return ;
   }
   ojo_playlist_entries_remove() ;
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;

   list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser_dialog)) ;
   if (list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_add_clicked() in window.c: no files specified\n") ;
      return ;
   }
   ojo_window_media_open_prepare(list, TRUE) ;
}

void on_ojo_filechooser_open_clicked()
{
   if (media_already_opened == TRUE)
   {
      ojo_playlist_entries_remove() ;
      ojo_player_tracks_free() ;
   }
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;

   list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser_dialog)) ;
   if (list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_open_clicked() in window.c: no files specified\n") ;
      return ;
   }
   ojo_window_media_open_prepare(list, FALSE) ;
}

void ojo_window_format_display_for_media () //FIXME: only hide currently-shown widgets
{
   if (ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist"))
   {
         gtk_widget_hide(GTK_WIDGET(background_image)) ;
         gtk_widget_hide(GTK_WIDGET(drawing_area)) ;
         ojo_playlist_show() ;
   }
   else
   {
      if (ojo_player_get_filetype() == AUDIO)
      {
         ojo_window_set_art_cover_image(ojo_player_get_artist(), ojo_player_get_album()) ;
         ojo_playlist_hide() ;
         gtk_widget_hide(GTK_WIDGET(drawing_area)) ;
         gtk_widget_show(GTK_WIDGET(background_image)) ;
      }
      else
      {
         ojo_playlist_hide() ;
         gtk_widget_hide(GTK_WIDGET(background_image)) ;
         gtk_widget_show(GTK_WIDGET(drawing_area)) ;
      }
   }

}

/*
 *   PLAYBACK CONTROL
 */
void on_ojo_play_pause_clicked()
{
   if(ojo_player_is_playing())
   {
      ojo_player_pause() ;
   }
   else
   {
      ojo_player_play() ;
   }
}

void on_ojo_prev_track_clicked()
{
   g_source_remove(timeout) ;
   ojo_player_prev_track() ;
}

void on_ojo_forward_clicked()
{
   ojo_player_forward() ;
}

void on_ojo_stop_clicked()
{
   ojo_player_pause() ;
   ojo_player_stop() ;
}

void on_ojo_backward_clicked()
{
   ojo_player_backward() ;
}

void on_ojo_next_track_clicked()
{
   g_source_remove(timeout) ;
   ojo_player_next_track() ;
}

void on_ojo_volume_value_changed()
{
    double volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(volume_button));
    libvlc_audio_set_volume(ojo_player_get_media_player(), (int)(100*volume)) ;
}

void on_ojo_repeat_clicked()
{
   if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 0)
      ojo_window_set_repeat(1) ;
   else if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 1)
      ojo_window_set_repeat(2) ;
   else
      ojo_window_set_repeat(0) ;
}

/*
 *   SETTINGS
 */

// FULLSCREEN
void on_ojo_fullscreen_clicked()
{
   if (ojo_settings_get_boolean(ojo_settings->gsettings, "fullscreen"))
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == TRUE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), FALSE) ;
      gtk_window_unfullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image(GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON)) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "fullscreen", FALSE) ;
      gtk_revealer_set_reveal_child(revealer_controls, TRUE) ;
      gtk_widget_show_all (GTK_WIDGET(menu_bar)) ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(window)) ;
      gtk_window_present(GTK_WINDOW(window)) ;
      gtk_button_set_image (GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-restore", GTK_ICON_SIZE_BUTTON)) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "fullscreen", TRUE) ;
      gtk_revealer_set_reveal_child(revealer_controls, FALSE) ;
      gtk_widget_hide (GTK_WIDGET(menu_bar)) ;
   }
}

void on_ojo_menu_fullscreen_toggled()
{
   on_ojo_fullscreen_clicked() ;
}

// SEEKBAR
gboolean ojo_window_seek_bar_update()
{
   double current_time = (double)ojo_player_get_current_time() ; // in ms
   double duration = (double)ojo_player_get_duration() ;         // in ms

   if (ojo_player_end_reached())
   {
      if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 1)
         ojo_player_media_play(ojo_player_get_media_index()) ;
      else if (ojo_player_get_media_index() < ojo_player_get_n_tracks()-1)
      {
         ojo_player_media_play(ojo_player_get_media_index()+1) ;
      }
      else
      {
         ojo_player_media_play(0) ;
         if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 0)
            ojo_player_pause(), ojo_player_stop() ;
      }
   }

   gtk_label_set_text(GTK_LABEL(time_label), time_to_string(current_time, duration)) ;
   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, duration) ;
   user_input = FALSE ;
   gtk_range_set_value(GTK_RANGE(seek_bar), current_time) ;

   return TRUE ;
}

void ojo_window_seek_bar_start()
{
   timeout = g_timeout_add(1000, G_SOURCE_FUNC(ojo_window_seek_bar_update), FALSE) ;
}

void on_ojo_seek_bar_value_changed()
{
   if (user_input)
   {
      if (gtk_range_get_value(GTK_RANGE(seek_bar)) != (double)ojo_player_get_current_time())
         ojo_player_set_current_time(gtk_range_get_value(GTK_RANGE(seek_bar))) ;
   }
   user_input = TRUE ;
}

void on_ojo_seek_bar_button_press_event()
{
   ojo_player_pause() ;
}

void on_ojo_seek_bar_button_release_event()
{
   ojo_player_play() ;
}


/*
 *   ABOUT
 */
void on_ojo_menu_about_activate()
{
	about_dialog_response = gtk_dialog_run(GTK_DIALOG(about)) ;
}

void on_ojo_on_about_response()
{
   gtk_widget_hide(GTK_WIDGET(about)) ;
}


/*
 *   PREFERENCES
 */
void on_ojo_menu_preferences_activate()
{
   gtk_toggle_button_set_active (preferences_dark_mode, ojo_settings_get_boolean(ojo_settings->gsettings, "dark-mode")) ;
   gtk_toggle_button_set_active (preferences_border_style, ojo_settings_get_boolean(ojo_settings->gsettings, "border-style")) ;
   gtk_toggle_button_set_active (preferences_view_playlist, ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;
   gtk_toggle_button_set_active (preferences_view_coverart, ojo_settings_get_boolean(ojo_settings->gsettings, "view-coverart")) ;
   gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;
}

void on_ojo_preferences_apply_clicked()
{
   ojo_window_set_dark_mode (gtk_toggle_button_get_active(preferences_dark_mode)) ;
   ojo_window_set_border_style (gtk_toggle_button_get_active(preferences_border_style)) ;
   ojo_window_set_view_playlist (gtk_toggle_button_get_active(preferences_view_playlist)) ;
   ojo_window_set_view_coverart (gtk_toggle_button_get_active(preferences_view_coverart)) ;
   gtk_widget_hide (GTK_WIDGET(preferences_dialog)) ;
}

void on_ojo_preferences_close_clicked()
{
    gtk_widget_hide (GTK_WIDGET(preferences_dialog)) ;
}

void ojo_window_set_dark_mode (gboolean dark_mode)
{
   g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", dark_mode, NULL) ;
   ojo_settings_set_boolean(ojo_settings->gsettings, "dark-mode", dark_mode) ;
}

void ojo_window_set_border_style (gboolean border_style)
{
   if (border_style) 
   {
      gtk_button_set_relief (playpause_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (prev_track_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (backward_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (stop_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (forward_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (next_track_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (GTK_BUTTON(volume_button), GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (fullscreen_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (playlist_button, GTK_RELIEF_NORMAL) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "border-style", border_style) ;
   }
   else
   {
      gtk_button_set_relief (playpause_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (prev_track_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (backward_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (stop_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (forward_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (next_track_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (GTK_BUTTON(volume_button), GTK_RELIEF_NONE) ;
      gtk_button_set_relief (fullscreen_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (playlist_button, GTK_RELIEF_NONE) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "border-style", border_style) ;
   }
}

void ojo_window_set_view_playlist (gboolean view_playlist)
{
   if (view_playlist)
   {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist), view_playlist) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "view-playlist", view_playlist) ;
      ojo_window_format_display_for_media() ;
   }
   else
   {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist), view_playlist) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "view-playlist", view_playlist) ;
      ojo_window_format_display_for_media() ;
   }
}

void ojo_window_set_cursor_visible(gboolean visible)
{
   GdkWindow *window ;
   GdkCursor *cursor ;

   window = gtk_widget_get_window (GTK_WIDGET(drawing_area)) ;

   if (visible)
   {
      cursor = gdk_cursor_new_from_name (gdk_display_get_default(), "default") ;
   }
   else
   {
      cursor = gdk_cursor_new_for_display (gdk_display_get_default(), GDK_BLANK_CURSOR) ;
   }
   gdk_window_set_cursor(window, cursor) ;
   g_object_unref(cursor) ;
}

void ojo_window_set_view_coverart (gboolean view_coverart) // TODO: move to settings.h
{
   ojo_settings_set_boolean(ojo_settings->gsettings, "view-coverart", view_coverart) ;
   ojo_window_set_art_cover_image(ojo_player_get_artist(), ojo_player_get_album()) ;
}

void ojo_window_set_repeat(int repeat_mode)
{
   if (repeat_mode == 0)
   {
      gtk_button_set_image(repeat_button, gtk_image_new_from_icon_name("media-repeat-none", GTK_ICON_SIZE_BUTTON)) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 0) ;
   }
   else if (repeat_mode == 1)
   {
      gtk_button_set_image(repeat_button, gtk_image_new_from_icon_name("media-repeat-track-amarok", GTK_ICON_SIZE_BUTTON)) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 1) ;
   }
   else
   {
      gtk_button_set_image(repeat_button, gtk_image_new_from_icon_name("media-repeat-all", GTK_ICON_SIZE_BUTTON)) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 2) ;
   }
}

gboolean timeout_handler(gpointer data)
{
   if (ojo_settings_get_boolean(ojo_settings->gsettings, "fullscreen"))
   {
      ojo_window_set_cursor_visible(FALSE) ;
      gtk_revealer_set_reveal_child(revealer_controls, FALSE) ;
      gtk_widget_hide(GTK_WIDGET(menu_bar)) ;
      area.timeout_tag = 0 ;
   }
	return (area.timeout_tag != 0) ;
}

gboolean on_ojo_drawing_area_motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
   const gdouble dist = sqrt(pow(event->x - area.last_motion_x, 2) + pow(event->y - area.last_motion_y, 2)) ;
   const gdouble speed = dist / (event->time - area.last_motion_time) ;

   area.last_motion_time = event->time ;
   area.last_motion_x = event->x ;
   area.last_motion_y = event->y ;

   if (speed >= 0 && ojo_settings_get_boolean(ojo_settings->gsettings, "fullscreen"))
   {
      ojo_window_set_cursor_visible(TRUE) ;
      gtk_revealer_set_reveal_child(revealer_controls, TRUE) ;
      gtk_widget_show_all(GTK_WIDGET(menu_bar)) ;
   }
   area.timeout_tag = g_timeout_add_seconds (3, timeout_handler, NULL) ;

   return TRUE ;
}


/*
 *   WINDOW SETUP
 */
void ojo_window_setup()
{
   media_already_opened = FALSE ;
   user_input = TRUE ;
   //TODO init area properly
   area.timeout_tag = 0 ;
   area.last_motion_time = 0 ;

   builder = gtk_builder_new () ;
   if (access("/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade", F_OK))
      gtk_builder_add_from_file (builder, "../data/org.github.FreaxMATE.Ojo.glade", NULL) ;
   else
      gtk_builder_add_from_file (builder, "/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade", NULL) ;

   ojo_playlist = ojo_playlist_initialize () ;
   window = GTK_WINDOW(gtk_builder_get_object(builder, "window_main")) ;
   gtk_window_set_default_size(window, ojo_settings_get_int(ojo_settings->gsettings, "width"), ojo_settings_get_int(ojo_settings->gsettings, "height")) ;
   window_width = ojo_window_get_width() ;
   window_height = ojo_window_get_height() ;

   gtk_builder_connect_signals(builder, NULL) ;
   gtk_window_set_title(window, "Ojo") ;

   drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "ojo_drawing_area")) ;
   gtk_widget_set_events (GTK_WIDGET(drawing_area), GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK) ;

   menu_bar = GTK_MENU_BAR(gtk_builder_get_object(builder, "ojo_menu")) ;
   file_menu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_menu_item")) ;
   file_submenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_submenu")) ;

   view_menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_fullscreen")) ;
   view_menu_showplaylist = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_showplaylist")) ;
   file_menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;

   controls = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_controls")) ;
   play_box = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_play_box")) ;
   seek_bar = GTK_SCALE(gtk_builder_get_object(builder, "ojo_seek_bar")) ;

   playpause_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause")) ;
   prev_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_prev_track")) ;
   backward_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_backward")) ;
   stop_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_stop")) ;
   forward_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_forward")) ;
   next_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_next_track")) ;
   volume_button = GTK_VOLUME_BUTTON(gtk_builder_get_object(builder, "ojo_volume")) ;
   fullscreen_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_fullscreen")) ;
   playlist_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_playlist")) ;
   repeat_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_repeat")) ;
   preferences_dark_mode = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_dark_mode")) ;
   preferences_border_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_border_style")) ;
   preferences_view_playlist = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_view_playlist")) ;
   preferences_view_coverart = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_view_coverart")) ;

   time_label = GTK_LABEL(gtk_builder_get_object(builder, "ojo_time_lbl")) ;
   background_image = GTK_IMAGE(gtk_builder_get_object(builder, "img_ojo_background_image")) ;

   revealer_controls = GTK_REVEALER(gtk_builder_get_object(builder, "ojo_revealer_controls")) ;
   about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_on_about")) ;
   preferences_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_preferences_dialog")) ;
   filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_filechooser_dialog")) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, 60.0) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), 0.0) ;
   ojo_window_set_dark_mode(ojo_settings_get_boolean(ojo_settings->gsettings, "dark-mode")) ;
   ojo_window_set_border_style(ojo_settings_get_boolean(ojo_settings->gsettings, "border-style")) ;
   ojo_window_set_view_playlist(ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;
   ojo_window_set_view_coverart(ojo_settings_get_boolean(ojo_settings->gsettings, "view-coverart")) ;
   ojo_window_set_repeat(ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode")) ;
   gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist), ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;

   gtk_widget_realize(GTK_WIDGET(drawing_area)) ;
   g_object_unref(builder) ;
}

int ojo_window_get_width()
{
   int width ;
   gtk_window_get_size(window, &width, NULL) ;
   return width ;
}

int ojo_window_get_height()
{
   int height ;
   gtk_window_get_size(window, NULL, &height) ;
   return height ;
}

void ojo_window_set_title(char *track_name)
{
   char *title ;
   title = calloc(strlen(track_name)+7, sizeof(char)) ;
   sprintf(title, "Ojo - %s", track_name) ;
   gtk_window_set_title(GTK_WINDOW(window), title) ;
   free(title) ;
   return ;
}

void on_window_main_size_allocate()
{
   window_width = ojo_window_get_width() ;
   window_height = ojo_window_get_height() ;
}

void on_window_main_destroy()
{
   ojo_settings_set_int(ojo_settings->gsettings, "width", window_width) ;
   ojo_settings_set_int(ojo_settings->gsettings, "height", window_height) ;
   gtk_main_quit() ;
}

void ojo_window_set_track_control_visibility(int n_tracks)
{
   if (n_tracks > 1)
   {
      gtk_widget_show(GTK_WIDGET(prev_track_button)) ;
      gtk_widget_show(GTK_WIDGET(next_track_button)) ;
   }
   else
   {
      gtk_widget_hide(GTK_WIDGET(prev_track_button)) ;
      gtk_widget_hide(GTK_WIDGET(next_track_button)) ;
   }
}


/*
 *   Other // TODO move to new file helperfunctions.c
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

