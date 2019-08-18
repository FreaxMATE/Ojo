#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <vlc/vlc.h>

#define BORDER_WIDTH 6

#include "vlcPlayer.h"

GtkWidget *playpause_button;
GtkWidget *image ;

void destroy(GtkWidget *widget, gpointer data)
{
   gtk_main_quit() ;
}

void on_open(GtkWidget *widget, gpointer data)
{
   GtkWidget *dialog ;
   dialog = gtk_file_chooser_dialog_new("Choose Media", data, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel",
                                        GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL) ;
   if(gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
   {
      char *uri ;
      uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog)) ;
      open_media(uri) ;
      g_free(uri) ;
   }
   gtk_widget_destroy(dialog) ;
}

void setButtonIcon(char *iconName)
{
   if (strcmp(iconName, "media-playback-pause") == 0)
   {
      image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON) ;
      gtk_button_set_image(GTK_BUTTON(playpause_button), image) ;
   }
   else if (strcmp(iconName, "media-playback-start") == 0)
   {
      image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON) ;
      gtk_button_set_image(GTK_BUTTON(playpause_button), image) ;
   }

   return ;
}

int main( int argc, char **argv)
{
   GtkWidget *window ;
   GtkWidget *box ;
   GtkWidget *menubar ;
   GtkWidget *filemenu ;
   GtkWidget *fileitem ;
   GtkWidget *filemenu_openitem ;
   GtkWidget *player_widget ;
   GtkWidget *buttonbox ;
   GtkWidget *stop_button ;

   XInitThreads() ;
   gtk_init (&argc, &argv) ;
   // setup window
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
   gtk_window_set_default_size(GTK_WINDOW(window), 400, 300) ;
   g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL) ;
   gtk_container_set_border_width (GTK_CONTAINER (window), 0) ;
   gtk_window_set_title(GTK_WINDOW(window), "Ojo") ;

   //setup box
   box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;
   gtk_container_add(GTK_CONTAINER(window), box) ;

   //setup menu
   menubar = gtk_menu_bar_new() ;
   filemenu = gtk_menu_new() ;
   fileitem = gtk_menu_item_new_with_label ("File") ;
   filemenu_openitem = gtk_menu_item_new_with_label("Open") ;
   gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), filemenu_openitem) ;
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileitem), filemenu) ;
   gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileitem) ;
   gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0) ;
   g_signal_connect(filemenu_openitem, "activate", G_CALLBACK(on_open), window) ;

   //setup player widget
   player_widget = gtk_drawing_area_new() ;
   gtk_box_pack_start(GTK_BOX(box), player_widget, TRUE, TRUE, 0) ;

   //setup controls
   playpause_button = gtk_button_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON) ;
   stop_button = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON) ;
   g_signal_connect(playpause_button, "clicked", G_CALLBACK(on_playpause), NULL) ;
   g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop), NULL) ;
   buttonbox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
   gtk_container_set_border_width(GTK_CONTAINER(buttonbox), BORDER_WIDTH) ;
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonbox), GTK_BUTTONBOX_START) ;
   gtk_box_pack_start(GTK_BOX(buttonbox), playpause_button, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(buttonbox), stop_button, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(box), buttonbox, FALSE, FALSE, 0) ;

   initVlc(player_widget) ;

   gtk_widget_show_all(window) ;
   gtk_main () ;

   quitVlc() ;

   return 0 ;
}
