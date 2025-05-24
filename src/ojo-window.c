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
#include <glib.h>
#include <math.h>

#include "ojo-window.h"


void on_ojo_filechooser_add_clicked(void);
void on_ojo_filechooser_open_clicked(void);
gboolean ojo_window_seek_bar_update(void);
void ojo_window_set_art_cover_image(OjoWindow *ojo_window, char *artist, char *album);
void ojo_window_set_dark_mode(OjoWindow *ojo_window, gboolean dark_mode);
void ojo_window_set_view_coverart(OjoWindow *ojo_window, gboolean view_coverart);
void ojo_window_set_track_control_visibility(OjoWindow *ojo_window, int n_tracks);
void ojo_window_set_cursor_visible(OjoWindow *ojo_window, gboolean visible);
int ojo_window_get_width(OjoWindow *ojo_window);
int ojo_window_get_height(OjoWindow *ojo_window);
char *time_to_string(double current_time, double duration);

void ojo_window_set_art_cover_image(OjoWindow *ojo_window, char *artist, char *album)
{
   char uri[1024];

   sprintf(uri, "/home/%s/.cache/vlc/art/artistalbum/%s/%s/art.jpg", getenv("USER"), artist, album);
   if (g_file_test(uri, G_FILE_TEST_EXISTS) && ojo_settings_get_boolean(ojo_settings, "view-coverart"))
      gtk_image_set_from_file(ojo_window->background_image, uri);
   else
      gtk_image_set_from_icon_name(ojo_window->background_image, "audio-x-generic", GTK_ICON_SIZE_DIALOG);
}

void ojo_window_media_open_prepare(OjoWindow *ojo_window, GSList *uri_list, gboolean add)
{
   int n_tracks = g_slist_length(uri_list);
   ojo_controlbox_set_playlist_control_visibility(ojo_controlbox, n_tracks);
   ojo_window->media_already_opened = TRUE;
   ojo_player_media_open(ojo_player, uri_list, n_tracks, add);
}

/*
 *   FILECHOOSER dialog
 */
void on_ojo_menu_open_activate()
{
   gtk_dialog_run(GTK_DIALOG(ojo_window->filechooser_dialog)) ;
}

void on_ojo_filechooser_cancel_clicked()
{
   gtk_widget_hide(GTK_WIDGET(ojo_window->filechooser_dialog)) ;
}

void on_ojo_filechooser_add_clicked()
{
   if (ojo_window->media_already_opened == FALSE)
   {
      on_ojo_filechooser_open_clicked() ;
      return ;
   }
   ojo_playlist_entries_remove(ojo_playlist) ;
   gtk_widget_hide(GTK_WIDGET(ojo_window->filechooser_dialog)) ;

   ojo_window->list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(ojo_window->filechooser_dialog)) ;
   if (ojo_window->list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_add_clicked() in window.c: no files specified\n") ;
      return ;
   }
   ojo_window_media_open_prepare(ojo_window, ojo_window->list, TRUE) ;
}

void on_ojo_filechooser_open_clicked()
{
   if (ojo_window->media_already_opened == TRUE)
   {
      ojo_playlist_entries_remove(ojo_playlist) ;
      ojo_player_tracks_free(ojo_player) ;
   }
   gtk_widget_hide(GTK_WIDGET(ojo_window->filechooser_dialog)) ;

   ojo_window->list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(ojo_window->filechooser_dialog)) ;
   if (ojo_window->list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_open_clicked() in window.c: no files specified\n") ;
      return ;
   }
   ojo_window_media_open_prepare(ojo_window, ojo_window->list, FALSE) ;
}

void ojo_window_format_display_for_media(OjoWindow *ojo_window)
{
   if (ojo_settings_get_boolean(ojo_settings, "view-playlist"))
   {
         gtk_widget_hide(GTK_WIDGET(ojo_window->background_image));
         gtk_widget_hide(GTK_WIDGET(ojo_window->drawing_area));
         ojo_playlist_show(ojo_playlist);
   }
   else
   {
      if (ojo_player_get_filetype(ojo_player) == AUDIO)
      {
         ojo_window_set_art_cover_image(ojo_window, ojo_player_get_artist(ojo_player), ojo_player_get_album(ojo_player));
         ojo_playlist_hide(ojo_playlist);
         gtk_widget_hide(GTK_WIDGET(ojo_window->drawing_area));
         gtk_widget_show(GTK_WIDGET(ojo_window->background_image));
      }
      else
      {
         ojo_playlist_hide(ojo_playlist);
         gtk_widget_hide(GTK_WIDGET(ojo_window->background_image));
         gtk_widget_show(GTK_WIDGET(ojo_window->drawing_area));
      }
   }
}


/*
 *   SETTINGS
 */

// FULLSCREEN
void on_ojo_fullscreen_clicked()
{
   if (ojo_settings_get_boolean(ojo_settings, "fullscreen"))
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_fullscreen)) == TRUE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_fullscreen), FALSE) ;
      gtk_window_unfullscreen(GTK_WINDOW(ojo_window->window)) ;
      ojo_controlbox_fullscreen_button_set(ojo_controlbox, TRUE) ;
      ojo_settings_set_boolean(ojo_settings, "fullscreen", FALSE) ;
      ojo_controlbox_show(ojo_controlbox) ;
      gtk_widget_show_all(GTK_WIDGET(ojo_window->menu_bar)) ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(ojo_window->window)) ;
      gtk_window_present(GTK_WINDOW(ojo_window->window)) ;
      ojo_controlbox_fullscreen_button_set(ojo_controlbox, FALSE) ;
      ojo_settings_set_boolean(ojo_settings, "fullscreen", TRUE) ;
      ojo_controlbox_hide(ojo_controlbox) ;
      gtk_widget_hide (GTK_WIDGET(ojo_window->menu_bar)) ;
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
	ojo_window->about_dialog_response = gtk_dialog_run(GTK_DIALOG(ojo_window->about)) ;
}

void on_ojo_on_about_response()
{
   gtk_widget_hide(GTK_WIDGET(ojo_window->about)) ;
}


/*
 *   PREFERENCES
 */
void on_ojo_menu_preferences_activate()
{
   gtk_toggle_button_set_active(ojo_window->preferences_dark_mode,
                                 ojo_settings_get_boolean(ojo_settings, "dark-mode")) ;
   gtk_toggle_button_set_active(ojo_window->preferences_border_style,
                                 ojo_settings_get_boolean(ojo_settings, "border-style")) ;
   gtk_toggle_button_set_active(ojo_window->preferences_view_coverart,
                                 ojo_settings_get_boolean(ojo_settings, "view-coverart")) ;
   gtk_dialog_run(GTK_DIALOG(ojo_window->preferences_dialog)) ;
}

void on_ojo_preferences_apply_clicked()
{
   ojo_window_set_dark_mode(ojo_window, gtk_toggle_button_get_active(ojo_window->preferences_dark_mode)) ;
   ojo_controlbox_set_border_style(ojo_controlbox, gtk_toggle_button_get_active(ojo_window->preferences_border_style)) ;
   ojo_window_set_view_coverart(ojo_window, gtk_toggle_button_get_active(ojo_window->preferences_view_coverart)) ;
   gtk_widget_hide (GTK_WIDGET(ojo_window->preferences_dialog)) ;
}

void on_ojo_preferences_close_clicked()
{
    gtk_widget_hide (GTK_WIDGET(ojo_window->preferences_dialog)) ;
}

void ojo_window_set_dark_mode(OjoWindow *ojo_window, gboolean dark_mode)
{
   g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", dark_mode, NULL);
   ojo_settings_set_boolean(ojo_settings, "dark-mode", dark_mode);
}

void ojo_window_set_view_playlist(OjoWindow *ojo_window, gboolean view_playlist)
{
   if (view_playlist)
   {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_showplaylist), view_playlist);
      ojo_settings_set_boolean(ojo_settings, "view-playlist", view_playlist);
      ojo_window_format_display_for_media(ojo_window);
   }
   else
   {
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(ojo_window->view_menu_showplaylist), view_playlist);
      ojo_settings_set_boolean(ojo_settings, "view-playlist", view_playlist);
      ojo_window_format_display_for_media(ojo_window);
   }
}

void ojo_window_set_cursor_visible(OjoWindow *ojo_window, gboolean visible)
{
   GdkWindow *window;
   GdkCursor *cursor;

   window = gtk_widget_get_window(GTK_WIDGET(ojo_window->drawing_area));

   if (visible)
   {
      cursor = gdk_cursor_new_from_name(gdk_display_get_default(), "default");
   }
   else
   {
      cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_BLANK_CURSOR);
   }
   gdk_window_set_cursor(window, cursor);
   g_object_unref(cursor);
}

void ojo_window_set_view_coverart(OjoWindow *ojo_window, gboolean view_coverart)
{
   ojo_settings_set_boolean(ojo_settings, "view-coverart", view_coverart);
   ojo_window_set_art_cover_image(ojo_window, ojo_player_get_artist(ojo_player), ojo_player_get_album(ojo_player));
}

void ojo_window_set_repeat(OjoWindow *ojo_window, int repeat_mode)
{
   ojo_controlbox_repeat_button_set(ojo_controlbox, repeat_mode);
   ojo_settings_set_int(ojo_settings, "repeat-mode", repeat_mode);
}

void ojo_window_set_random(OjoWindow *ojo_window, gboolean random)
{
   ojo_controlbox_random_button_set(ojo_controlbox, random);
   ojo_settings_set_boolean(ojo_settings, "random-playback", random);
}

gboolean ojo_window_mouse_motion_handler(OjoWindow *ojo_window)
{
   static int counter;
   double distance;

   if (!ojo_settings_get_boolean(ojo_settings, "view-playlist")
       && ojo_player_get_filetype(ojo_player) != AUDIO
       && ojo_player_get_n_tracks(ojo_player) >= 1)
   {
      if (ojo_settings_get_boolean(ojo_settings, "fullscreen"))
      {
         int new_x = ojo_player_get_mousepos_x(ojo_player);
         int new_y = ojo_player_get_mousepos_y(ojo_player);
         distance = sqrt(pow(abs(new_x - ojo_window->old_x), 2) + pow(abs(new_y - ojo_window->old_y), 2));
         if (distance > ojo_window->mouse_sensitivity)
         {
            ojo_controlbox_show(ojo_controlbox);
            ojo_window_set_cursor_visible(ojo_window, TRUE);
            counter = 0;
         }
         ojo_window->old_x = ojo_player_get_mousepos_x(ojo_player);
         ojo_window->old_y = ojo_player_get_mousepos_y(ojo_player);
         if (counter == 30) // 3 seconds of no motion
         {
            if (new_x == ojo_window->old_x && new_y == ojo_window->old_y)
            {
               if (new_y < ojo_player_get_size_y(ojo_player) - 4)
               {
                  ojo_controlbox_hide(ojo_controlbox);
                  ojo_window_set_cursor_visible(ojo_window, FALSE);
               }
            }
            counter = 0;
         }
         counter++;
      }
   }
   else
   {
      ojo_controlbox_show(ojo_controlbox);
      ojo_window_set_cursor_visible(ojo_window, TRUE);
   }
   return TRUE;
}

void ojo_window_start_mouse_motion_handler(OjoWindow *ojo_window)
{
   libvlc_video_set_mouse_input(ojo_player->media_player, TRUE);
   ojo_window->old_x = ojo_player_get_mousepos_x(ojo_player);
   ojo_window->old_y = ojo_player_get_mousepos_y(ojo_player);
   ojo_window->timeout = g_timeout_add(100, (GSourceFunc)ojo_window_mouse_motion_handler, ojo_window);
}

/*
 *   WINDOW SETUP
 */
OjoWindow *ojo_window_initialize()
{
   OjoWindow *new ;
   new = malloc (sizeof(OjoWindow)) ;

   new->media_already_opened = FALSE ;
   new->user_input = TRUE ;
   new->mouse_sensitivity = 50 ;

   if (access("/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade", F_OK))
      new->builder = gtk_builder_new_from_file ("../data/org.github.FreaxMATE.Ojo.glade") ;
   else
      new->builder = gtk_builder_new_from_file ("/usr/local/share/ojo/org.github.FreaxMATE.Ojo.glade") ;

   ojo_playlist = ojo_playlist_initialize (new->builder) ;
   new->window_width = ojo_settings_get_int(ojo_settings, "width") ;
   new->window_height = ojo_settings_get_int(ojo_settings, "height") ;
   new->window = GTK_WINDOW(gtk_builder_get_object(new->builder, "window_main")) ;
   gtk_window_set_default_size(new->window, new->window_width, new->window_height) ;

   gtk_window_set_title(new->window, "Ojo") ;

   new->drawing_area = GTK_DRAWING_AREA(gtk_builder_get_object(new->builder, "ojo_drawing_area")) ;

   new->menu_bar = GTK_MENU_BAR(gtk_builder_get_object(new->builder, "ojo_menu")) ;
   new->file_menu = GTK_MENU_ITEM(gtk_builder_get_object(new->builder, "ojo_menu_item")) ;
   new->file_submenu = GTK_MENU_ITEM(gtk_builder_get_object(new->builder, "ojo_submenu")) ;

   new->view_menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(new->builder, "ojo_menu_fullscreen")) ;
   new->view_menu_showplaylist = GTK_WIDGET(gtk_builder_get_object(new->builder, "ojo_menu_showplaylist")) ;
   new->file_menu_open = GTK_WIDGET(gtk_builder_get_object(new->builder, "ojo_open")) ;

   new->preferences_dark_mode = GTK_TOGGLE_BUTTON(gtk_builder_get_object(new->builder, "ojo_preferences_dark_mode")) ;
   new->preferences_border_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(new->builder,
                                                "ojo_preferences_border_style")) ;
                                                new->preferences_view_coverart = GTK_TOGGLE_BUTTON(gtk_builder_get_object(new->builder,
                                                 "ojo_preferences_view_coverart")) ;

                                                 new->background_image = GTK_IMAGE(gtk_builder_get_object(new->builder, "img_ojo_background_image")) ;

                                                 new->about = GTK_DIALOG(gtk_builder_get_object(new->builder, "ojo_on_about")) ;
                                                 new-> preferences_dialog = GTK_DIALOG(gtk_builder_get_object(new->builder, "ojo_preferences_dialog")) ;
                                                 new->filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(new->builder, "ojo_filechooser_dialog")) ;

   ojo_controlbox = ojo_controlbox_initialize(new->builder) ;

   ojo_window_set_dark_mode(new, ojo_settings_get_boolean(ojo_settings, "dark-mode")) ;
   ojo_controlbox_set_border_style(ojo_controlbox, ojo_settings_get_boolean(ojo_settings, "border-style")) ;
   ojo_window_set_view_playlist(new, FALSE) ;
   ojo_window_set_view_coverart(new, ojo_settings_get_boolean(ojo_settings, "view-coverart")) ;
   ojo_window_set_repeat(new, ojo_settings_get_int(ojo_settings, "repeat-mode")) ;
   ojo_window_set_random(new, ojo_settings_get_int(ojo_settings, "repeat-mode")) ;
   gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(new->view_menu_showplaylist),
                                  ojo_settings_get_boolean(ojo_settings, "view-playlist")) ;

   ojo_window_start_mouse_motion_handler(new) ;
   gtk_widget_realize(GTK_WIDGET(new->drawing_area)) ;
   return new;
}

void ojo_window_connect_signals(OjoWindow *ojo_window)
{
   gtk_builder_connect_signals(ojo_window->builder, NULL);
   g_object_unref(ojo_window->builder) ;
}

int ojo_window_get_width(OjoWindow *ojo_window)
{
   int width;
   gtk_window_get_size(GTK_WINDOW(ojo_window->window), &width, NULL);
   return width;
}

int ojo_window_get_height(OjoWindow *ojo_window)
{
   int height;
   gtk_window_get_size(GTK_WINDOW(ojo_window->window), NULL, &height);
   return height;
}

void ojo_window_set_title(OjoWindow *ojo_window, char *track_name)
{
   char *title;
   title = calloc(strlen(track_name) + 7, sizeof(char));
   sprintf(title, "Ojo - %s", track_name);
   gtk_window_set_title(GTK_WINDOW(ojo_window->window), title);
   free(title);
}

void on_window_main_size_allocate()
{
   ojo_window->window_width = ojo_window_get_width(ojo_window) ;
   ojo_window->window_height = ojo_window_get_height(ojo_window) ;
}

void on_window_main_destroy()
{
   ojo_settings_set_int(ojo_settings, "width", ojo_window->window_width) ;
   ojo_settings_set_int(ojo_settings, "height", ojo_window->window_height) ;
   gtk_main_quit() ;
}

