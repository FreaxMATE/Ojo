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
   menuBar = gtk_menu_bar_new() ;
   fileMenu = gtk_menu_new() ;
   fileItem = gtk_menu_item_new_with_label ("File") ;
   fileMenuOpenItem = gtk_menu_item_new_with_label("Open") ;
   gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileMenuOpenItem) ;
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileItem), fileMenu) ;
   gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), fileItem) ;

   helpMenu = gtk_menu_new() ;
   helpItem = gtk_menu_item_new_with_label ("Help") ;
   helpMenuAboutItem = gtk_menu_item_new_with_label("About") ;
   gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), helpMenuAboutItem) ;
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpItem), helpMenu) ;
   gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), helpItem) ;


   gtk_box_pack_start(GTK_BOX(box), menuBar, FALSE, FALSE, 0) ;
   g_signal_connect(fileMenuOpenItem, "activate", G_CALLBACK(onOpen), window) ;
   g_signal_connect(helpMenuAboutItem, "activate", G_CALLBACK(onAbout), window) ;
}







