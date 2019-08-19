#ifndef _widget_h_
#define _widget_h_

#include <math.h>

#include "window.h"
#include "vlcPlayer.h"

GtkWidget *playpauseButton;
GtkWidget *image ;
GtkWidget *playerWidget ;
GtkWidget *buttonBox ;
GtkWidget *progressBox ;
GtkWidget *barBox ;
GtkWidget *stopButton ;
GtkWidget *progressBar ;

void setupWidgets(void) ;
void setButtonIcon(char *iconName) ;
GtkWidget *getPlayerWidget() ;
gboolean updateBar(void) ;

#endif
