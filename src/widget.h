#ifndef _widget_h_
#define _widget_h_

#include <math.h>

#include "window.h"
#include "vlcPlayer.h"

GtkWidget *playpauseButton;
GtkWidget *image ;
GtkWidget *playerWidget ;
GtkWidget *buttonBox ;
GtkWidget *controlBox ;
GtkWidget *progressBox ;
GtkWidget *barBox ;
GtkWidget *stopButton ;
GtkWidget *progressBar ;
GtkWidget *timeLabel ;
char string[16] ;

void setupWidgets(void) ;
void setButtonIcon(char *iconName) ;
GtkWidget *getPlayerWidget() ;
gboolean updateBar(void) ;
char *timeToString(double currentTime, double duration) ;

#endif
