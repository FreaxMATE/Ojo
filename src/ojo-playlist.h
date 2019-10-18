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

#ifndef _playlist_h_
#define _playlist_h_

#include <gtk/gtk.h>

#include "ojo.h"
#include "ojo-player.h"

typedef struct _OjoPlaylist
{
   GtkListBox *playlist_box ;

} OjoPlaylist ;

OjoPlaylist *ojo_playlist_initialize() ;
void ojo_playlist_gtk_initialize() ;
void ojo_playlist_entries_remove() ;
void ojo_playlist_show() ;
void ojo_playlist_hide() ;
void ojo_playlist_select_row(int index) ;

#endif /* _playlist_h_ */

