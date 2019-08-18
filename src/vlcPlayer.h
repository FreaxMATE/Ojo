#ifndef _vlc_player_h_
#define _vlc_player_h_

#include <vlc/vlc.h>

#include "widget.h"

libvlc_media_player_t *media_player ;
libvlc_instance_t *vlc_inst ;

void initVlc(GtkWidget *player_widget) ;
void quitVlc(void) ;
void player_widget_on_realize(GtkWidget *widget) ;
void open_media(const char* uri) ;
void on_playpause(void) ;
void on_stop(void) ;
void play(void) ;
void pause_player(void) ;


#endif
