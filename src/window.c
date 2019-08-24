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

void on_ojo_open_activate()
{
   GtkWidget *dialog ;
   dialog = gtk_file_chooser_dialog_new("Choose Media", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel", GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL) ;
   if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
   {
      char *uri ;
      uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog)) ;
      openMedia(uri) ;
      g_free(uri) ;
   }
   gtk_widget_destroy(dialog) ;
}

void on_window_main_destroy()
{
   gtk_main_quit() ;
}

void setupWindow()
{
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/window_main.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    playerWidget = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_drawing_area")) ;
   menubar = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu")) ;
   filemenu = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_menu_item")) ;
   fileitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_submenu")) ;
   filemenuOpenitem = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_open")) ;
    progressBar = GTK_WIDGET(gtk_builder_get_object(builder, "ojo_progress_bar"));

    g_object_unref(builder);
}
