#ifndef _window_h_
#define _window_h_

#include "vlcPlayer.h"

GtkWidget *window ;
GtkWidget *box ;

void setupWindow(void) ;
void destroy(void) ;
void on_open(GtkWidget *widget, gpointer data) ;
GtkWidget *getWindow(void) ;
GtkWidget *getBox(void) ;

#endif
