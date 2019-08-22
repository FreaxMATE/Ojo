/*
 * Copyright (c) 2019 FreaxMATE
 *
 * This file is part of Ojo.
 *
 * Ojo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Ojo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Ojo.  If not, see <http://www.gnu.org/licenses/>.
 */

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

   seekBackwardButton = gtk_button_new_from_icon_name("media-seek-backward", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (seekBackwardButton, "Rewind");
   gtk_button_set_relief(GTK_BUTTON(seekBackwardButton), GTK_RELIEF_NONE) ;

   playpauseButton = gtk_button_new_from_icon_name("media-playback-start", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (playpauseButton, "Play");
   gtk_button_set_relief(GTK_BUTTON(playpauseButton), GTK_RELIEF_NONE) ;

   stopButton = gtk_button_new_from_icon_name("media-playback-stop", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (stopButton, "Stop");
   gtk_button_set_relief(GTK_BUTTON(stopButton), GTK_RELIEF_NONE) ;

   seekForwardButton = gtk_button_new_from_icon_name("media-seek-forward", GTK_ICON_SIZE_BUTTON) ;
   gtk_widget_set_tooltip_text (seekForwardButton, "Forward");
   gtk_button_set_relief(GTK_BUTTON(seekForwardButton), GTK_RELIEF_NONE) ;

   g_signal_connect(seekBackwardButton, "clicked", G_CALLBACK(onSeekBackward), NULL) ;
   g_signal_connect(playpauseButton, "clicked", G_CALLBACK(onPlayPause), NULL) ;
   g_signal_connect(stopButton, "clicked", G_CALLBACK(onStop), NULL) ;
   g_signal_connect(seekForwardButton, "clicked", G_CALLBACK(onSeekForward), NULL) ;


   controlBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0) ;
   gtk_container_set_border_width(GTK_CONTAINER(controlBox), 0) ;
   buttonBox = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL) ;
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_START) ;
   gtk_container_set_border_width(GTK_CONTAINER(buttonBox), 0) ;
   gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonBox), GTK_BUTTONBOX_START) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), seekBackwardButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), playpauseButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), stopButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(buttonBox), seekForwardButton, FALSE, FALSE, 0) ;
   gtk_box_pack_start(GTK_BOX(controlBox), buttonBox, FALSE, FALSE, 0) ;

   volumeButton = gtk_volume_button_new() ;
   gtk_scale_button_set_value(GTK_SCALE_BUTTON(volumeButton), 1) ;
   g_signal_connect(GTK_VOLUME_BUTTON(volumeButton), "value-changed", G_CALLBACK(onVolumeChanged), NULL) ;

   gtk_box_pack_end(GTK_BOX(controlBox), volumeButton, FALSE, FALSE, 0) ;
   timeLabel = gtk_label_new ("00:00  ") ;
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

   if (curMinutes > 59 || allMinutes > 59)
   {
       int curHours = ((int)curMinutes)/60 ; int allHours = ((int)allMinutes)/60 ;
       sprintf(string, "%02d:%02d:%02d / %02d:%02d:%02d  ", curHours, curMinutes-(curHours*60),
               curSeconds, allHours, allMinutes-(allHours*60), allSeconds) ;
   }
   else
   {
      sprintf(string, "%02d:%02d / %02d:%02d  ", curMinutes, curSeconds, allMinutes, allSeconds) ;
   }

   return string ;
}

double getVolumeLevel()
{
   return gtk_scale_button_get_value(GTK_SCALE_BUTTON(volumeButton)) ;
}


