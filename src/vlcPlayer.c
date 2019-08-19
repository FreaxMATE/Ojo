#include <vlc/vlc.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "vlcPlayer.h"

void initVlc(GtkWidget *playerWidget)
{
   vlcInst = libvlc_new(0, NULL) ;
   mediaPlayer = libvlc_media_player_new(vlcInst) ;
   g_signal_connect(G_OBJECT(playerWidget), "realize", G_CALLBACK(playerWidgetOnRealize), NULL) ;
}

void quitVlc()
{
   libvlc_media_player_release(mediaPlayer) ;
   libvlc_release(vlcInst) ;
}

void playerWidgetOnRealize(GtkWidget *widget)
{
   libvlc_media_player_set_xwindow(mediaPlayer, GDK_WINDOW_XID(gtk_widget_get_window(widget))) ;
}

void openMedia(const char* uri)
{

   media = libvlc_media_new_location(vlcInst, uri) ;
   libvlc_media_player_set_media(mediaPlayer, media) ;
   startProgressBar() ;
   play() ;
   strcpy(metaData.title, libvlc_media_get_meta(media, libvlc_meta_Title)) ;
   setTitle(metaData.title) ;
   libvlc_media_release(media) ;
}


void onPlayPause()
{
   if(libvlc_media_player_is_playing(mediaPlayer) == 1)
   {
      pausePlayer() ;
   }
   else
   {
      play() ;
   }
}

void onStop()
{
   pausePlayer() ;
   libvlc_media_player_stop(mediaPlayer) ;
}

void play()
{
   libvlc_media_player_play(mediaPlayer) ;
   setButtonIcon("media-playback-pause") ;
}

void pausePlayer()
{
   libvlc_media_player_pause(mediaPlayer) ;
   setButtonIcon("media-playback-start") ;
}

int64_t getDuration()
{
   return libvlc_media_get_duration(media) ;
   //printf ("Duration = %ld\n", (duration/1000)/60) ;
}

int64_t getCurrentTime()
{
   return libvlc_media_player_get_time(mediaPlayer) ;
}

