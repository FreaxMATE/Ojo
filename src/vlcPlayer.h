#ifndef _vlc_player_h_
#define _vlc_player_h_

#include <vlc/vlc.h>

#include "widget.h"

libvlc_media_player_t *mediaPlayer ;
libvlc_instance_t *vlcInst ;

void initVlc(GtkWidget *player_widget) ;
void quitVlc(void) ;
void playerWidgetOnRealize(GtkWidget *widget) ;
void openMedia(const char* uri) ;
void onPlayPause(void) ;
void onStop(void) ;
void play(void) ;
void pausePlayer(void) ;


#endif
