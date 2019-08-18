#ifndef _widget_h_
#define _widget_h_

#include "window.h"
#include "vlcPlayer.h"

#define BORDER_WIDTH 6

GtkWidget *playpause_button;
GtkWidget *image ;
GtkWidget *player_widget ;
GtkWidget *buttonbox ;
GtkWidget *stop_button ;

void setupWidgets(void) ;
void setButtonIcon(char *iconName) ;
GtkWidget *getPlayerWidget() ;

#endif
