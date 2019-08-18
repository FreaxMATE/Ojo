#include <vlc/vlc.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>

#include "vlcPlayer.h"

void initVlc(GtkWidget *player_widget)
{
   vlc_inst = libvlc_new(0, NULL) ;
   media_player = libvlc_media_player_new(vlc_inst) ;
   g_signal_connect(G_OBJECT(player_widget), "realize", G_CALLBACK(player_widget_on_realize), NULL) ;
}

void quitVlc()
{
   libvlc_media_player_release(media_player) ;
   libvlc_release(vlc_inst) ;
}

void player_widget_on_realize(GtkWidget *widget)
{
   libvlc_media_player_set_xwindow(media_player, GDK_WINDOW_XID(gtk_widget_get_window(widget))) ;
}

void open_media(const char* uri)
{
   libvlc_media_t *media ;
   media = libvlc_media_new_location(vlc_inst, uri) ;
   libvlc_media_player_set_media(media_player, media) ;
   play() ;
   libvlc_media_release(media) ;
}


void on_playpause(void)
{
   if(libvlc_media_player_is_playing(media_player) == 1)
   {
      pause_player() ;
   }
   else
   {
      play() ;
   }
}

void on_stop()
{
   pause_player() ;
   libvlc_media_player_stop(media_player) ;
}

void play(void)
{
   libvlc_media_player_play(media_player) ;
   setButtonIcon("media-playback-pause") ;
}

void pause_player(void)
{
   libvlc_media_player_pause(media_player) ;
   setButtonIcon("media-playback-start") ;
}



