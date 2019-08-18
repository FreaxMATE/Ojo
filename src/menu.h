#ifndef _menu_h_
#define _menu_h_

#include "window.h"

GtkWidget *menubar ;
GtkWidget *filemenu ;
GtkWidget *fileitem ;
GtkWidget *filemenuOpenitem ;

void setupMenu(GtkWidget *window, GtkWidget *box) ;

#endif
