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

#include "menu.h"

void setupMenu(GtkWidget *window, GtkWidget *box)
{
   menubar = gtk_menu_bar_new() ;
   filemenu = gtk_menu_new() ;
   fileitem = gtk_menu_item_new_with_label ("File") ;
   filemenuOpenitem = gtk_menu_item_new_with_label("Open") ;
   gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), filemenuOpenitem) ;
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileitem), filemenu) ;
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileitem) ;
   gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0) ;
   g_signal_connect(filemenuOpenitem, "activate", G_CALLBACK(onOpen), window) ;
}
