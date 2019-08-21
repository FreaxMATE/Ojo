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

void setupWindow()
{
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
   gtk_window_set_default_size(GTK_WINDOW(window), 960, 540) ;
   g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL) ;
   gtk_container_set_border_width (GTK_CONTAINER (window), 0) ;
   gtk_window_set_title(GTK_WINDOW(window), "Ojo") ;

   //setup box
   box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;
   gtk_container_add(GTK_CONTAINER(window), box) ;
}

void destroy()
{
   gtk_main_quit() ;
}

void onOpen(GtkWidget *widget, gpointer data)
{
   GtkWidget *dialog ;
   dialog = gtk_file_chooser_dialog_new("Choose Media", data, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel",
                                        GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL) ;
   if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
   {
      char *uri ;
      uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog)) ;
      openMedia(uri) ;
      g_free(uri) ;
   }
   gtk_widget_destroy(dialog) ;
}

GtkWidget *getWindow()
{
   return window ;
}

GtkWidget *getBox()
{
   return box ;
}

void setTitle(char *trackName)
{
   char title[64] ;
   sprintf(title, "Ojo - %s", trackName) ;
   gtk_window_set_title(GTK_WINDOW(window), title) ;
   return ;
}








