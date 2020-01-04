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

#include "ojo-playlist.h"

/*
 *   PLAYLIST
 */

OjoPlaylist *ojo_playlist_initialize(GtkBuilder *builder)
{
   OjoPlaylist *new ;
   new = malloc (sizeof(OjoPlaylist)) ;
   new->playlist_box = GTK_BOX(gtk_builder_get_object(builder, "ojo_playlist_box")) ;
   new->playlist_listbox = GTK_LIST_BOX(gtk_builder_get_object(builder, "ojo_playlist_listbox")) ;
   return new ;
}

void on_ojo_playlist_listbox_row_activated(GtkListBox *box, GtkListBoxRow *row, gpointer user_data)
{
   ojo_player_media_play(gtk_list_box_row_get_index(row)) ;
}

void on_ojo_menu_showplaylist_toggled()
{
   ojo_window_set_view_playlist(!ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;
}

void on_ojo_playlist_clicked()
{
   ojo_window_set_view_playlist(!ojo_settings_get_boolean(ojo_settings->gsettings, "view-playlist")) ;
}

void ojo_playlist_gtk_initialize()
{
   GtkWidget *playlist_widgets[ojo_player_get_n_tracks()] ;

   for (int i = 0; i < ojo_player_get_n_tracks(); ++i)
   {
      playlist_widgets[i] = gtk_label_new(ojo_player_get_title_by_index(i)) ;
      gtk_label_set_xalign(GTK_LABEL(playlist_widgets[i]), 0.0) ;
      gtk_widget_show(playlist_widgets[i]) ;
      gtk_list_box_insert(ojo_playlist->playlist_listbox, playlist_widgets[i], i) ;
   }
}

void ojo_playlist_entries_remove()
{
   GList *children, *iter;

   children = gtk_container_get_children(GTK_CONTAINER(ojo_playlist->playlist_listbox)) ;
   for (iter = children; iter != NULL; iter = g_list_next(iter))
      gtk_widget_destroy(GTK_WIDGET(iter->data)) ;
   g_list_free(children) ;
}

void ojo_playlist_show()
{
   if (!gtk_widget_is_visible(GTK_WIDGET(ojo_playlist->playlist_box)))
      gtk_widget_show_all(GTK_WIDGET(ojo_playlist->playlist_box)) ;
}

void ojo_playlist_hide()
{
   if (gtk_widget_is_visible(GTK_WIDGET(ojo_playlist->playlist_box)))
      gtk_widget_hide(GTK_WIDGET(ojo_playlist->playlist_box)) ;
}

void ojo_playlist_select_row(int index)
{
   gtk_list_box_select_row (ojo_playlist->playlist_listbox,
                            gtk_list_box_get_row_at_index(ojo_playlist->playlist_listbox, index)) ;
}




