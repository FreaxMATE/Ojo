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
#include "ojo-controlbox.h"

#define TIME_STRING_SIZE 128

typedef struct _OjoWindow
{
    GtkBuilder        *builder; 
    GtkWindow         *window;
    GtkDrawingArea    *drawing_area;
    GtkMenuBar        *menu_bar;
    GtkMenuItem       *file_menu,
                      *file_submenu;
    GtkWidget         *file_menu_open,
                      *view_menu_fullscreen,
                      *view_menu_showplaylist,
                      *play_box;
    GtkImage          *background_image;
    GtkToggleButton   *preferences_dark_mode,
                      *preferences_border_style,
                      *preferences_view_coverart;
    GtkBox            *main_box;
    GtkDialog         *about;
    GtkDialog         *preferences_dialog;
    GtkDialog         *filechooser_dialog;

    GSList            *list;
    int               n_tracks;
    gboolean          about_dialog_response, media_already_opened, user_input;
    char              time_string[TIME_STRING_SIZE];
    int               timeout, window_width, window_height;
    int64_t           duration;
    int               old_x, old_y, mouse_sensitivity;
} OjoWindow;

OjoWindow *ojo_window_initialize(void) ;
void ojo_window_media_open_prepare(OjoWindow *ojo_window, GSList *uri_list, gboolean add) ;
void ojo_window_seek_bar_start(OjoWindow *ojo_window) ;
void ojo_window_set_view_playlist(OjoWindow *ojo_window, gboolean view_playlist) ;
void ojo_window_set_title(OjoWindow *ojo_window, char *trackName) ;
void ojo_window_set_repeat(OjoWindow *ojo_window, int repeat_mode) ;
void ojo_window_set_random(OjoWindow *ojo_window, gboolean random) ;
void ojo_window_format_display_for_media(OjoWindow *ojo_window) ;
void ojo_window_set_prev_next_track_control_visibility(OjoWindow *ojo_window, int n_tracks) ;
void ojo_window_connect_signals(OjoWindow *ojo_window);

#endif /* _ojo_window_h_ */

