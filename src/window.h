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
                  *fullscreen_button ;
GtkListBox        *playlist_box ;
GtkScrolledWindow *scrolled_window_playlist ;
GtkDialog         *about ;
GtkDialog         *preferences_dialog ;
GtkDialog         *filechooser_dialog ;

typedef struct _settings
{
   int fullscreen ;
   int dark_mode ;
   int border_style ;
   int view_playlist ;
} Settings ;

typedef struct _playlist
{
   int n_items ;
   char **uri ;
   GtkWidget **playlist_item_info ;
} Playlist ;

Settings *settings ;
Playlist playlist ;

int about_dialog_response, media_already_opened ;
char time_string[32] ;
int timeout ;

int update_bar() ;
void start_seek_bar() ;
void set_playlist_item_title() ;
void set_dark_mode (int dark_mode) ;
void set_border_style (int border_style) ;
void set_view_playlist(int view_playlist) ;
void set_title(char *trackName) ;
void setup_window(void) ;
char *time_to_string(double current_time, double duration) ;

#endif /* _window_h_ */

