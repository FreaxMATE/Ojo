#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <vlc/vlc.h>

#include "vlcPlayer.h"
#include "menu.h"
#include "window.h"
#include "widget.h"

int main( int argc, char **argv)
{
   XInitThreads() ;
   gtk_init (&argc, &argv) ;

   setupWindow() ;
   setupMenu(getWindow(), getBox()) ;
   setupWidgets() ;
   initVlc(playerWidget) ;

   gtk_widget_show_all(window) ;
   gtk_main () ;
   quitVlc() ;

   return 0 ;
}
