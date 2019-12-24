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

#ifndef _ojo_window_h_
#define _ojo_window_h_

#include <unistd.h>

#include "ojo.h"
#include "ojo-player.h"
#include "ojo-playlist.h"

GtkBuilder        *builder; 
GtkWindow         *window ;
GtkDrawingArea    *drawing_area ;
GtkMenuBar        *menu_bar ;
GtkMenuItem       *file_menu,
                  *file_submenu ;
GtkWidget         *file_menu_open,
                  *view_menu_fullscreen,
                  *view_menu_showplaylist,
                  *play_box ;
GtkImage          *background_image ;
GtkToggleButton   *preferences_dark_mode ,
                  *preferences_border_style,
                  *preferences_view_playlist ,
                  *preferences_view_coverart ;
GtkScale          *seek_bar ;
GtkLabel          *time_label ;
GtkVolumeButton   *volume_button ;
GtkButton         *playpause_button,
                  *prev_track_button,
                  *backward_button,
                  *stop_button,
                  *forward_button,
                  *next_track_button,
                  *fullscreen_button,
                  *playlist_button ;
GtkBox            *main_box ;
GtkDialog         *about ;
GtkDialog         *preferences_dialog ;
GtkDialog         *filechooser_dialog ;

struct _area {
	guint timeout_tag;
   guint32 last_motion_time ;
   double last_motion_x ;
   double last_motion_y ;
} area ;

GSList *list ;
int n_tracks ;
gboolean about_dialog_response, media_already_opened, visible_controls ;
char time_string[32] ;
int timeout, window_width, window_height ;
gboolean on_ojo_drawing_area_motion_notify_event( GtkWidget *widget, GdkEventMotion *event ) ;
void ojo_window_media_open_prepare(GSList *uri_list, gboolean add) ;
void on_ojo_filechooser_add_clicked(void) ;
void on_ojo_filechooser_open_clicked(void) ;
void ojo_window_gtk_playlist_initialize(void) ;
gboolean ojo_window_seek_bar_update(void) ;
void ojo_window_seek_bar_start() ;
void ojo_window_set_playlist_item_title() ;
void ojo_window_set_art_cover_image(char *artist, char *album) ;
void ojo_window_set_dark_mode(gboolean dark_mode) ;
void ojo_window_set_border_style(gboolean border_style) ;
void ojo_window_set_view_playlist(gboolean view_playlist) ;
void ojo_window_set_view_coverart(gboolean view_coverart) ;
void ojo_window_set_title(char *trackName) ;
void ojo_window_setup(void) ;
void ojo_window_format_display_for_media(void) ;
void ojo_window_set_track_control_visibility(int n_tracks) ;
void ojo_window_set_cursor_visible(gboolean visible) ;
int ojo_window_get_width(void) ;
int ojo_window_get_height(void) ;
char *time_to_string(double current_time, double duration) ;

#endif /* _ojo_window_h_ */

