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

void on_ojo_filechooser_add_clicked(void) ;
void on_ojo_filechooser_open_clicked(void) ;
gboolean ojo_window_seek_bar_update(void) ;
void ojo_window_set_art_cover_image(char *artist, char *album) ;
void ojo_window_set_dark_mode(gboolean dark_mode) ;
void ojo_window_set_view_coverart(gboolean view_coverart) ;
void ojo_window_set_track_control_visibility(int n_tracks) ;
void ojo_window_set_cursor_visible(gboolean visible) ;
int ojo_window_get_width(void) ;
int ojo_window_get_height(void) ;
char *time_to_string(double current_time, double duration) ;


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
   ojo_controlbox_set_prev_next_track_control_visibility(n_tracks) ;
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
      ojo_controlbox_fullscreen_button_set(TRUE) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "fullscreen", FALSE) ;
      ojo_controlbox_show() ;
      gtk_widget_show_all (GTK_WIDGET(menu_bar)) ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(window)) ;
      gtk_window_present(GTK_WINDOW(window)) ;
      ojo_controlbox_fullscreen_button_set(FALSE) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "fullscreen", TRUE) ;
      ojo_controlbox_hide() ;
      gtk_widget_hide (GTK_WIDGET(menu_bar)) ;
   }
}

void on_ojo_menu_fullscreen_toggled()
{
   on_ojo_fullscreen_clicked() ;
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
   gtk_toggle_button_set_active (preferences_dark_mode,
                                 ojo_settings_get_boolean(ojo_settings->gsettings, "dark-mode")) ;
   gtk_toggle_button_set_active (preferences_border_style,
                                 ojo_settings_get_boolean(ojo_settings->gsettings, "border-style")) ;
   gtk_toggle_button_set_active (preferences_view_coverart,
                                 ojo_settings_get_boolean(ojo_settings->gsettings, "view-coverart")) ;
   gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;
}

void on_ojo_preferences_apply_clicked()
{
   ojo_window_set_dark_mode (gtk_toggle_button_get_active(preferences_dark_mode)) ;
   ojo_controlbox_set_border_style (gtk_toggle_button_get_active(preferences_border_style)) ;
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
      ojo_controlbox_repeat_button_set(repeat_mode) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 0) ;
   }
   else if (repeat_mode == 1)
   {
      ojo_controlbox_repeat_button_set(repeat_mode) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 1) ;
   }
   else
   {
      ojo_controlbox_repeat_button_set(repeat_mode) ;
      ojo_settings_set_int(ojo_settings->gsettings, "repeat-mode", 2) ;
   }
}

gboolean ojo_window_mouse_motion_handler()
{
   static int counter ;

   if (!ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist") && ojo_player_get_filetype() != AUDIO)
   {
      if (ojo_settings_get_boolean(ojo_settings->gsettings, "fullscreen"))
      {
         int new_x = ojo_player_get_mousepos_x() ;
         int new_y = ojo_player_get_mousepos_y() ;

         if (abs(new_x-old_x) > mouse_sensitivity || abs(new_y-old_y) > mouse_sensitivity)
         {
            ojo_controlbox_show() ;
            ojo_window_set_cursor_visible(TRUE) ;
            counter = 0 ;
         }
         old_x = ojo_player_get_mousepos_x() ;
         old_y = ojo_player_get_mousepos_y() ;
         if (counter == 30)                            // 3 seconds of no motion
         {
            if (new_x == old_x && new_y == old_y)
            {
               ojo_controlbox_hide() ;
               ojo_window_set_cursor_visible(FALSE) ;
            }
            counter = 0 ;
         }
         counter++ ;
      }
   }
   else
   {
      ojo_controlbox_show() ;
      ojo_window_set_cursor_visible(TRUE) ;
   }
   return TRUE ;
}

void ojo_window_start_mouse_motion_handler()
{
   libvlc_video_set_mouse_input(ojo_player->media_player, TRUE) ;
   old_x = ojo_player_get_mousepos_x() ;
   old_y = ojo_player_get_mousepos_y() ;
   timeout = g_timeout_add(100, ojo_window_mouse_motion_handler, FALSE) ;
}

/*
 *   WINDOW SETUP
 */
void ojo_window_setup()
{
   media_already_opened = FALSE ;
   user_input = TRUE ;
   mouse_sensitivity = 50 ;

   builder = gtk_builder_new () ;
   if (access("/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade", F_OK))
      gtk_builder_add_from_file (builder, "../data/org.github.FreaxMATE.Ojo.glade", NULL) ;
   else
      gtk_builder_add_from_file (builder, "/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade", NULL) ;

   ojo_playlist = ojo_playlist_initialize (builder) ;
   window = GTK_WINDOW(gtk_builder_get_object(builder, "window_main")) ;
   gtk_window_set_default_size(window, ojo_settings_get_int(ojo_settings->gsettings, "width"), ojo_settings_get_int(ojo_settings->gsettings, "height")) ;
   window_width = ojo_window_get_width() ;
   window_height = ojo_window_get_height() ;

   gtk_builder_connect_signals(builder, NULL) ;
   gtk_window_set_title(window, "Ojo") ;

   drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "ojo_drawing_area")) ;

   menu_bar = GTK_MENU_BAR(gtk_builder_get_object(builder, "ojo_menu")) ;
   file_menu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_menu_item")) ;
   file_submenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_submenu")) ;

   view_menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_fullscreen")) ;
   view_menu_showplaylist = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_showplaylist")) ;
   file_menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;

   preferences_dark_mode = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_dark_mode")) ;
   preferences_border_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_border_style")) ;
   preferences_view_coverart = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_view_coverart")) ;

   background_image = GTK_IMAGE(gtk_builder_get_object(builder, "img_ojo_background_image")) ;

   about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_on_about")) ;
   preferences_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_preferences_dialog")) ;
   filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_filechooser_dialog")) ;

   ojo_controlbox = ojo_controlbox_initialize(builder) ;

   ojo_window_set_dark_mode(ojo_settings_get_boolean(ojo_settings->gsettings, "dark-mode")) ;
   ojo_controlbox_set_border_style(ojo_settings_get_boolean(ojo_settings->gsettings, "border-style")) ;
   ojo_window_set_view_playlist(FALSE) ;
   ojo_window_set_view_coverart(ojo_settings_get_boolean(ojo_settings->gsettings, "view-coverart")) ;
   ojo_window_set_repeat(ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode")) ;
   gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist),
                                  ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;

   ojo_window_start_mouse_motion_handler() ;
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

