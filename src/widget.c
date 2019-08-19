#include <string.h>
#include <gtk/gtk.h>

#include "widget.h"

void setupWidgets()
{
   playerWidget = gtk_drawing_area_new() ;
   gtk_box_pack_start(GTK_BOX(box), playerWidget, TRUE, TRUE, 0) ;

   //setup progressbar
   progressBar = gtk_progress_bar_new() ;

   //setup buttons
   playpauseButton = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (playpauseButton, "Play");
   gtk_button_set_relief(GTK_BUTTON(playpauseButton), GTK_RELIEF_NONE) ;
   stopButton = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (stopButton, "Stop");
   gtk_button_set_relief(GTK_BUTTON(stopButton), GTK_RELIEF_NONE) ;
   g_signal_connect(playpauseButton, "clicked", G_CALLBACK(onPlayPause), NULL) ;
   g_signal_connect(stopButton, "clicked", G_CALLBACK(onStop), NULL) ;

   timeLabel = gtk_label_new ("") ;
   controlBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;
   gtk_container_set_border_width(GTK_CONTAINER(controlBox), 6) ;
   buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_START) ;
   gtk_container_set_border_width(GTK_CONTAINER(buttonBox), 0) ;
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_START) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), playpauseButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), stopButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(controlBox), buttonBox, FALSE, FALSE, 0) ;

   gtk_box_pack_end(GTK_BOX(controlBox), timeLabel, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(box), controlBox, FALSE, FALSE, 0) ;

   barBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0) ;
   gtk_box_pack_start(GTK_BOX(barBox), progressBar, TRUE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(box), barBox, FALSE, FALSE, 0) ;

   return ;
}

void setButtonIcon(char *iconName)
{
   if (strcmp(iconName, "media-playback-pause") == 0)
   {
      image = gtk_image_new_from_icon_name("media-playback-pause", GTK_ICON_SIZE_BUTTON) ;
      gtk_button_set_image(GTK_BUTTON(playpauseButton), image) ;
      gtk_widget_set_tooltip_text(playpauseButton, "Pause") ;
   }
   else if (strcmp(iconName, "media-playback-start") == 0)
   {
      image = gtk_image_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON) ;
      gtk_button_set_image(GTK_BUTTON(playpauseButton), image) ;
      gtk_widget_set_tooltip_text(playpauseButton, "Play") ;
   }

   return ;
}

GtkWidget *getPlayerWidget()
{
   return playerWidget ;
}

gboolean updateBar()
{
   double currentTime = (double)getCurrentTime() ;
   double duration = (double)getDuration() ;

   gtk_label_set_text(GTK_LABEL(timeLabel), timeToString(currentTime, duration)) ;

   gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressBar), currentTime/duration) ;
   if (gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressBar)) == 1)
      return FALSE ;
   return TRUE ;
}

void startProgressBar()
{
   g_timeout_add_seconds(1, G_SOURCE_FUNC(updateBar), NULL) ;
}

char *timeToString(double currentTime, double duration)
{
   currentTime /= 1000 ;
   duration /= 1000 ;
   int curMinutes = ((int)currentTime)/60 ; int allMinutes = ((int)duration)/60 ; 
   int curSeconds = ((int)currentTime)%60 ; int allSeconds = ((int)duration)%60 ;

   sprintf(string, "%02d:%02d / %02d:%02d", curMinutes, curSeconds, allMinutes, allSeconds) ;
   return string ;
}


