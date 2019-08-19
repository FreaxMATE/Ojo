#ifndef _vlc_player_h_
#define _vlc_player_h_

#include <vlc/vlc.h>

#include "widget.h"

libvlc_media_player_t *mediaPlayer ;
libvlc_media_t *media ;
libvlc_instance_t *vlcInst ;
int64_t duration ;

struct metaData
{
   char title[64] ;

}metaData ;

void initVlc(GtkWidget *player_widget) ;
void quitVlc(void) ;
void playerWidgetOnRealize(GtkWidget *widget) ;
void openMedia(const char* uri) ;
void onPlayPause(void) ;
void onStop(void) ;
void play(void) ;
void pausePlayer(void) ;
int64_t getDuration(void) ;
int64_t getCurrentTime() ;
void startProgressBar() ;

#endif
