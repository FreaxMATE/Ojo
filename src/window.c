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

/*
 *   PLAYLIST
 */

void on_ojo_playlist_box_row_activated(GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
   play_media(gtk_list_box_row_get_index(row)) ;
}

void on_ojo_menu_showplaylist_toggled()
{
   set_view_playlist(gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist))) ;
}

void initialize_gtk_playlist()
{
   playlist_widgets = calloc (vlc->n_tracks, sizeof(GtkLabel *)) ;

   for (int i = 0; i < vlc->n_tracks; ++i)
   {
      playlist_widgets[i] = gtk_label_new(vlc->tracks[i]->title) ;
      gtk_label_set_xalign(GTK_LABEL(playlist_widgets[i]), 0.0) ;
      gtk_widget_show(playlist_widgets[i]) ;
      gtk_list_box_insert(playlist_box, playlist_widgets[i], i) ;
   }
   gtk_paned_set_position (paned, window_get_width()/2) ;
}

void remove_playlist_entries()
{
   for (int i = 0; i < vlc->n_tracks; ++i)
   {
      gtk_widget_destroy(playlist_widgets[i]) ;
   }
   free(playlist_widgets) ;
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
   remove_playlist_entries() ;
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;
   gtk_widget_hide(GTK_WIDGET(scrolled_window_playlist)) ;

   list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser_dialog)) ;
   if (list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_add_clicked() in window.c: no files specified\n") ;
      return ;
   }
   n_tracks = g_slist_length(list) ;
   gtk_widget_show(GTK_WIDGET(prev_track_button)) ;
   gtk_widget_show(GTK_WIDGET(next_track_button)) ;
   set_view_playlist(TRUE) ;

   media_already_opened = TRUE ;
   open_media(list, n_tracks, TRUE) ;
}

void on_ojo_filechooser_open_clicked()
{
   if (media_already_opened == TRUE)
      remove_playlist_entries() ;
   gtk_widget_hide(GTK_WIDGET(filechooser_dialog)) ;
   gtk_widget_hide(GTK_WIDGET(scrolled_window_playlist)) ;

   list = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(filechooser_dialog)) ;
   if (list == NULL)
   {
      fprintf (stderr, "WARNING: on_ojo_filechooser_open_clicked() in window.c: no files specified\n") ;
      return ;
   }
   n_tracks = g_slist_length(list) ;
   if (n_tracks > 1)
   {
      gtk_widget_show(GTK_WIDGET(prev_track_button)) ;
      gtk_widget_show(GTK_WIDGET(next_track_button)) ;
      set_view_playlist(TRUE) ;
   }
   else
   {
      gtk_widget_hide(GTK_WIDGET(prev_track_button)) ;
      gtk_widget_hide(GTK_WIDGET(next_track_button)) ;
      set_view_playlist(FALSE) ;
   }
   media_already_opened = TRUE ;
   open_media(list, n_tracks, FALSE) ;
}


/*
 *   PLAYBACK CONTROL
 */
void on_ojo_play_pause_clicked()
{
   if(libvlc_media_player_is_playing(vlc->media_player) == 1)
   {
      pause_player() ;
   }
   else
   {
      play_player() ;
   }
}

void on_ojo_prev_track_clicked()
{
   g_source_remove(timeout) ;
   vlc->media_index-1 < 0 ? play_media(0) : play_media(vlc->media_index-1) ;
}

void on_ojo_forw_clicked()
{
   libvlc_media_player_set_position(vlc->media_player, libvlc_media_player_get_position(vlc->media_player)+0.05) ;
}

void on_ojo_stop_clicked()
{
   pause_player() ;
   libvlc_media_player_stop(vlc->media_player) ;
}

void on_ojo_prev_clicked()
{
   libvlc_media_player_set_position(vlc->media_player, libvlc_media_player_get_position(vlc->media_player)-0.05) ;
}

void on_ojo_next_track_clicked()
{
   g_source_remove(timeout) ;
   vlc->media_index+1 < vlc->n_tracks ? play_media(vlc->media_index+1) : play_media(0) ;
}

void on_ojo_volume_value_changed()
{
    double volume = gtk_scale_button_get_value(GTK_SCALE_BUTTON(volume_button));
    libvlc_audio_set_volume(vlc->media_player, (int)(100*volume)) ;
}


/*
 *   SETTINGS
 */

// FULLSCREEN
void on_ojo_fullscreen_clicked()
{
   if (settings->fullscreen == TRUE)
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == TRUE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), FALSE) ;
      gtk_window_unfullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image(GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON)) ;
      settings->fullscreen = FALSE ;
   }
   else
   {
      if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen)) == FALSE)
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_fullscreen), TRUE) ;
      gtk_window_fullscreen(GTK_WINDOW(window)) ;
      gtk_button_set_image (GTK_BUTTON(fullscreen_button), gtk_image_new_from_icon_name("view-restore", GTK_ICON_SIZE_BUTTON)) ;
      settings->fullscreen = TRUE ;
   }
}

void on_ojo_menu_fullscreen_toggled()
{
   on_ojo_fullscreen_clicked() ;
}

// SEEKBAR
int update_bar()
{
   double current_time = (double)get_current_time() ; // in ms
   double duration = (double)get_duration() ;         // in ms

   if (libvlc_media_player_get_state(vlc->media_player) == libvlc_Ended)
   {
      if (vlc->media_index < vlc->n_tracks-1)
      {
         play_media(vlc->media_index+1) ;
      }
      else
      {
         play_media(0) ;
      }
   }

   gtk_label_set_text(GTK_LABEL(time_label), time_to_string(current_time, duration)) ;
   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, duration) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), current_time) ;

   return TRUE ;
}

void start_seek_bar()
{
   timeout = g_timeout_add(100, update_bar, FALSE) ;
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

Settings *initialise_settings()
{
   Settings *new ;
   new = (Settings*) malloc(sizeof(Settings)) ;
   new->fullscreen = FALSE ;
   new->dark_mode = FALSE ;
   new->border_style = FALSE ;
   new->view_playlist = FALSE ;
   return new ;
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
   gtk_toggle_button_set_active (preferences_dark_mode, settings->dark_mode) ;
   gtk_toggle_button_set_active (preferences_border_style, settings->border_style) ;
   gtk_toggle_button_set_active (preferences_view_playlist, settings->view_playlist) ;
   gtk_dialog_run(GTK_DIALOG(preferences_dialog)) ;
}

void on_ojo_preferences_apply_clicked()
{
   set_dark_mode (gtk_toggle_button_get_active(preferences_dark_mode)) ;
   set_border_style (gtk_toggle_button_get_active(preferences_border_style)) ;
   set_view_playlist (gtk_toggle_button_get_active(preferences_view_playlist)) ;
   gtk_widget_hide (GTK_WIDGET(preferences_dialog)) ;
}

void on_ojo_preferences_close_clicked()
{
    gtk_toggle_button_set_active (preferences_dark_mode, settings->dark_mode) ;
    gtk_toggle_button_set_active (preferences_border_style, settings->border_style) ;
    gtk_toggle_button_set_active (preferences_view_playlist, settings->view_playlist) ;
    gtk_widget_hide (GTK_WIDGET(preferences_dialog)) ;
}

void set_dark_mode (int dark_mode)
{
    if (settings->dark_mode != dark_mode)
    {
        settings->dark_mode = dark_mode ;
        g_object_set (gtk_settings_get_default (), "gtk-application-prefer-dark-theme", dark_mode, NULL) ;
        settings->dark_mode = dark_mode ;
    }
}

void set_border_style (int border_style)
{
    if (settings->border_style != border_style)
    {
        if (border_style) 
        {
                gtk_button_set_relief (playpause_button, GTK_RELIEF_NORMAL) ;
                gtk_button_set_relief (prev_button, GTK_RELIEF_NORMAL) ;
                gtk_button_set_relief (stop_button, GTK_RELIEF_NORMAL) ;
                gtk_button_set_relief (forw_button, GTK_RELIEF_NORMAL) ;
                gtk_button_set_relief (GTK_BUTTON(volume_button), GTK_RELIEF_NORMAL) ;
                gtk_button_set_relief (fullscreen_button, GTK_RELIEF_NORMAL) ;
                settings->border_style = border_style ;
        }
        else
        {
                gtk_button_set_relief (playpause_button, GTK_RELIEF_NONE) ;
                gtk_button_set_relief (prev_button, GTK_RELIEF_NONE) ;
                gtk_button_set_relief (stop_button, GTK_RELIEF_NONE) ;
                gtk_button_set_relief (forw_button, GTK_RELIEF_NONE) ;
                gtk_button_set_relief (GTK_BUTTON(volume_button), GTK_RELIEF_NONE) ;
                gtk_button_set_relief (fullscreen_button, GTK_RELIEF_NONE) ;
                settings->border_style = border_style ;
        }
    }
}

void set_view_playlist(int view_playlist)
{
   if (view_playlist)
   {
      gtk_widget_show(GTK_WIDGET(scrolled_window_playlist)) ;
      settings->view_playlist = view_playlist ;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist), TRUE) ;
   }
   else
   {
      gtk_widget_hide(GTK_WIDGET(scrolled_window_playlist)) ;
      settings->view_playlist = view_playlist ;
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(view_menu_showplaylist), FALSE) ;
   }
}


/*
 *   WINDOW SETUP
 */
void setup_window()
{
   media_already_opened = FALSE ;
   settings = initialise_settings();
   builder = gtk_builder_new();
   gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

   window = GTK_WINDOW(gtk_builder_get_object(builder, "window_main"));
   gtk_builder_connect_signals(builder, NULL);

   player_widget = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "ojo_drawing_area")) ;

   playlist_box = GTK_LIST_BOX(gtk_builder_get_object(builder, "ojo_playlist_box")) ;

   menu_bar = GTK_MENU_BAR(gtk_builder_get_object(builder, "ojo_menu")) ;
   file_menu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_menu_item")) ;
   file_submenu = GTK_MENU_ITEM(gtk_builder_get_object(builder, "ojo_submenu")) ;

   view_menu_fullscreen = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_fullscreen")) ;
   view_menu_showplaylist = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_showplaylist")) ;
   file_menu_open = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;

   seek_bar = GTK_SCALE(gtk_builder_get_object(builder, "ojo_seek_bar")) ;

   playpause_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause")) ;
   prev_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_prev_track")) ;
   prev_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_prev")) ;
   stop_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_stop")) ;
   forw_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_forw")) ;
   next_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_next_track")) ;
   volume_button = GTK_VOLUME_BUTTON(gtk_builder_get_object(builder, "ojo_volume")) ;
   fullscreen_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_fullscreen")) ;
   preferences_dark_mode = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_dark_mode")) ;
   preferences_border_style = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_border_style")) ;
   preferences_view_playlist = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "ojo_preferences_view_playlist")) ;

   time_label = GTK_LABEL(gtk_builder_get_object(builder, "ojo_time_lbl")) ;
   scrolled_window_playlist = GTK_SCROLLED_WINDOW(gtk_builder_get_object(builder, "ojo_scrolled_window_playlist")) ;
   paned = GTK_PANED(gtk_builder_get_object(builder, "ojo_paned")) ;

   about = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_on_about")) ;
   preferences_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_preferences_dialog")) ;
   filechooser_dialog = GTK_DIALOG(gtk_builder_get_object(builder, "ojo_filechooser_dialog")) ;

   gtk_range_set_range(GTK_RANGE(seek_bar), 0.0, 60.0) ;
   gtk_range_set_value(GTK_RANGE(seek_bar), 0.0) ;
   gtk_widget_hide(GTK_WIDGET(scrolled_window_playlist)) ;
   gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM(view_menu_showplaylist), settings->view_playlist) ;
   g_object_unref(builder) ;
}

int window_get_width()
{
   int width ;
   gtk_window_get_size(window, &width, NULL) ;
   return width ;
}

int window_get_height()
{
   int height ;
   gtk_window_get_size(window, &height, NULL) ;
   return height ;
}

void set_title(char *track_name)
{
   char *title ;
   title = calloc(strlen(track_name)+6, sizeof(char)) ;
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

