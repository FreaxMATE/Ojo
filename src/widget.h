#ifndef _widget_h_
#define _widget_h_

#include "window.h"
#include "vlcPlayer.h"

#define BORDER_WIDTH 6

GtkWidget *playpauseButton;
GtkWidget *image ;
GtkWidget *playerWidget ;
GtkWidget *buttonbox ;
GtkWidget *stopButton ;

void setupWidgets(void) ;
void setButtonIcon(char *iconName) ;
GtkWidget *getPlayerWidget() ;

#endif
