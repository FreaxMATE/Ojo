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
                  *play_box,
                  *controls ;
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
                  *playlist_button,
                  *repeat_button ;
GtkBox            *main_box ;
GtkDialog         *about ;
GtkDialog         *preferences_dialog ;
GtkDialog         *filechooser_dialog ;
GtkRevealer       *revealer_controls ;

GSList *list ;
int n_tracks ;
gboolean about_dialog_response, media_already_opened, user_input ;
char time_string[32] ;
int timeout, window_width, window_height ;
int64_t duration ;
int old_x, old_y, mouse_sensitivity ;

void ojo_window_media_open_prepare(GSList *uri_list, gboolean add) ;
void ojo_window_seek_bar_start() ;
void ojo_window_set_view_playlist(gboolean view_playlist) ;
void ojo_window_set_title(char *trackName) ;
void ojo_window_setup(void) ;
void ojo_window_format_display_for_media(void) ;

#endif /* _ojo_window_h_ */

