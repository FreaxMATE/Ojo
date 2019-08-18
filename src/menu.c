#include <gtk/gtk.h>

#include "menu.h"

void setupMenu(GtkWidget *window, GtkWidget *box)
{
   menubar = gtk_menu_bar_new() ;
   filemenu = gtk_menu_new() ;
   fileitem = gtk_menu_item_new_with_label ("File") ;
   filemenu_openitem = gtk_menu_item_new_with_label("Open") ;
   gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), filemenu_openitem) ;
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileitem), filemenu) ;
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileitem) ;
   gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0) ;
   g_signal_connect(filemenu_openitem, "activate", G_CALLBACK(on_open), window) ;
}
