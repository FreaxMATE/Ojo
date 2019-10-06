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

#ifndef _window_h_
#define _window_h_

#include "vlcPlayer.h"

GtkBuilder        *builder; 
GtkWindow         *window ;
GtkDrawingArea    *player_widget ;
GtkMenuBar        *menu_bar ;
GtkMenuItem       *file_menu,
                  *file_submenu ;
GtkWidget         *file_menu_open,
                  *view_menu_fullscreen,
                  *view_menu_showplaylist ;
GtkToggleButton   *preferences_dark_mode ,
                  *preferences_border_style,
                  *preferences_view_playlist ;
GtkScale          *seek_bar ;
GtkLabel          *time_label ;
GtkVolumeButton   *volume_button ;
GtkButton         *playpause_button,
                  *prev_track_button,
                  *prev_button,
                  *stop_button,
                  *forw_button,
                  *next_track_button,
                  *fullscreen_button,
                  *playlist_button ;
GtkBox            *main_box ;
GtkListBox        *playlist_box ;
GtkDialog         *about ;
GtkDialog         *preferences_dialog ;
GtkDialog         *filechooser_dialog ;
GtkWidget         **playlist_widgets ;

typedef struct _settings
{
   gboolean fullscreen ;
   gboolean dark_mode ;
   gboolean border_style ;
   gboolean view_playlist ;
} Settings ;

Settings *settings ;

GSList *list ;
int n_tracks ;
gboolean about_dialog_response, media_already_opened ;
char time_string[32] ;
int timeout ;

void on_ojo_filechooser_add_clicked(void) ;
void on_ojo_filechooser_open_clicked(void) ;
gboolean update_bar() ;
void start_seek_bar() ;
void set_playlist_item_title() ;
void set_dark_mode (gboolean dark_mode) ;
void set_border_style (gboolean border_style) ;
void set_view_playlist(gboolean view_playlist) ;
void set_title(char *trackName) ;
void setup_window(void) ;
void initialize_gtk_playlist(void) ;
int window_get_width(void) ;
int window_get_height(void) ;
char *time_to_string(double current_time, double duration) ;

#endif /* _window_h_ */

