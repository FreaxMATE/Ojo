#include <gtk/gtk.h>

#include "window.h"

void setupWindow()
{
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
   gtk_window_set_default_size(GTK_WINDOW(window), 400, 300) ;
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

GtkWidget *getWindow()
{
   return window ;
}

GtkWidget *getBox()
{
   return box ;
}










