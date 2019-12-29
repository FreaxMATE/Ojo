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

#ifndef _ojo_controlbox_h_
#define _ojo_controlbox_h_

#include <gtk/gtk.h>

#include "ojo.h"
#include "ojo-player.h"
#include "ojo-window.h"

typedef struct _OjoControlBox
{
   GtkRevealer     *revealer_controls ;
   GtkVolumeButton *volume_button ;
   GtkScale        *seek_bar ;
   GtkBox          *controlbox,
                   *controlbutton_box ;
   GtkLabel        *time_label ;
   GtkButton       *playpause_button,
                   *prev_track_button,
                   *backward_button,
                   *stop_button,
                   *forward_button,
                   *next_track_button,
                   *fullscreen_button,
                   *playlist_button,
                   *repeat_button ;
} OjoControlBox ;

OjoControlBox *ojo_controlbox_initialize(GtkBuilder *builder) ;
void ojo_controlbox_show() ;
void ojo_controlbox_hide() ;
void ojo_controlbox_free(OjoControlBox *controlbox) ;
void ojo_controlbox_set_prev_next_track_control_visibility(int n_tracks) ;
void ojo_controlbox_seek_bar_start() ;
void ojo_controlbox_set_border_style (gboolean border_style) ;
void ojo_controlbox_fullscreen_button_set(gboolean fullscreen_mode) ;
void ojo_controlbox_repeat_button_set(int repeat_mode) ;

#endif /* _ojo_controlbox_h_ */

