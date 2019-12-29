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

#include "ojo-controlbox.h"

char *time_to_string(double current_time, double duration) ;

OjoControlBox *ojo_controlbox_initialize(GtkBuilder *builder)
{
   OjoControlBox *new = malloc(sizeof(OjoControlBox)) ;

   new->controlbox = GTK_BOX(gtk_builder_get_object(builder, "ojo_controlbox")) ;
   new->controlbutton_box = GTK_BOX(gtk_builder_get_object(builder, "ojo_controlbutton_box")) ;
   new->seek_bar = GTK_SCALE(gtk_builder_get_object(builder, "ojo_seek_bar")) ;
   new->revealer_controls = GTK_REVEALER(gtk_builder_get_object(builder, "ojo_revealer_controls")) ;

   new->playpause_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_play_pause")) ;
   new->prev_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_prev_track")) ;
   new->backward_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_backward")) ;
   new->stop_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_stop")) ;
   new->forward_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_forward")) ;
   new->next_track_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_next_track")) ;
   new->volume_button = GTK_VOLUME_BUTTON(gtk_builder_get_object(builder, "ojo_volume")) ;
   new->fullscreen_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_fullscreen")) ;
   new->playlist_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_playlist")) ;
   new->repeat_button = GTK_BUTTON(gtk_builder_get_object(builder, "ojo_repeat")) ;

   new->time_label = GTK_LABEL(gtk_builder_get_object(builder, "ojo_time_lbl")) ;
   gtk_range_set_range(GTK_RANGE(new->seek_bar), 0.0, 60.0) ;
   gtk_range_set_value(GTK_RANGE(new->seek_bar), 0.0) ;
   return new ;
}

void ojo_controlbox_show()
{
   gtk_revealer_set_reveal_child(ojo_controlbox->revealer_controls, TRUE) ;
   ojo_controlbox_set_prev_next_track_control_visibility(ojo_player_get_n_tracks()) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->controlbox)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->controlbutton_box)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->seek_bar)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->playpause_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->backward_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->stop_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->forward_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->volume_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->fullscreen_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->playlist_button)) ;
   gtk_widget_show(GTK_WIDGET(ojo_controlbox->repeat_button)) ;
}

void ojo_controlbox_hide()
{
   gtk_revealer_set_reveal_child(ojo_controlbox->revealer_controls, FALSE) ;
}

void on_ojo_play_pause_clicked()
{
   ojo_player_is_playing() ? ojo_player_pause() : ojo_player_play() ;
}

void on_ojo_prev_track_clicked()
{
   g_source_remove(timeout) ;
   ojo_player_prev_track() ;
}

void on_ojo_forward_clicked()
{
   ojo_player_forward() ;
}

void on_ojo_stop_clicked()
{
   ojo_player_pause() ;
   ojo_player_stop() ;
}

void on_ojo_backward_clicked()
{
   ojo_player_backward() ;
}

void on_ojo_next_track_clicked()
{
   g_source_remove(timeout) ;
   ojo_player_next_track() ;
}

void on_ojo_volume_value_changed()
{
   ojo_player_set_volume(gtk_scale_button_get_value(GTK_SCALE_BUTTON(ojo_controlbox->volume_button))) ;
}

void on_ojo_repeat_clicked()
{
   if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 0)
      ojo_window_set_repeat(1) ;
   else if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 1)
      ojo_window_set_repeat(2) ;
   else
      ojo_window_set_repeat(0) ;
}

// SEEKBAR
gboolean ojo_controlbox_seek_bar_update()
{
   double current_time = (double)ojo_player_get_current_time() ; // in ms
   double duration = (double)ojo_player_get_duration() ;         // in ms

   if (ojo_player_end_reached())
   {
      if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 1)
         ojo_player_media_play(ojo_player_get_media_index()) ;
      else if (ojo_player_get_media_index() < ojo_player_get_n_tracks()-1)
      {
         ojo_player_media_play(ojo_player_get_media_index()+1) ;
      }
      else
      {
         ojo_player_media_play(0) ;
         if (ojo_settings_get_int(ojo_settings->gsettings, "repeat-mode") == 0)
            ojo_player_pause(), ojo_player_stop() ;
      }
   }

   gtk_label_set_text(GTK_LABEL(ojo_controlbox->time_label), time_to_string(current_time, duration)) ;
   gtk_range_set_range(GTK_RANGE(ojo_controlbox->seek_bar), 0.0, duration) ;
   user_input = FALSE ;
   gtk_range_set_value(GTK_RANGE(ojo_controlbox->seek_bar), current_time) ;

   return TRUE ;
}

void ojo_controlbox_seek_bar_start()
{
   timeout = g_timeout_add(1000, ojo_controlbox_seek_bar_update, FALSE) ;
}

void on_ojo_seek_bar_value_changed()
{
   if (user_input)
   {
      ojo_player_set_current_time(gtk_range_get_value(GTK_RANGE(ojo_controlbox->seek_bar))) ;
   }
   user_input = TRUE ;
}

void on_ojo_seek_bar_button_press_event()
{
   ojo_player_pause() ;
}

void on_ojo_seek_bar_button_release_event()
{
   ojo_player_play() ;
}


void ojo_controlbox_set_border_style (gboolean border_style)
{
   if (border_style) 
   {
      gtk_button_set_relief (ojo_controlbox->playpause_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->prev_track_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->backward_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->stop_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->forward_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->next_track_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (GTK_BUTTON(ojo_controlbox->volume_button), GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->fullscreen_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->playlist_button, GTK_RELIEF_NORMAL) ;
      gtk_button_set_relief (ojo_controlbox->repeat_button, GTK_RELIEF_NORMAL) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "border-style", border_style) ;
   }
   else
   {
      gtk_button_set_relief (ojo_controlbox->playpause_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->prev_track_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->backward_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->stop_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->forward_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->next_track_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (GTK_BUTTON(ojo_controlbox->volume_button), GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->fullscreen_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->playlist_button, GTK_RELIEF_NONE) ;
      gtk_button_set_relief (ojo_controlbox->repeat_button, GTK_RELIEF_NONE) ;
      ojo_settings_set_boolean(ojo_settings->gsettings, "border-style", border_style) ;
   }
}


void ojo_controlbox_set_prev_next_track_control_visibility(int n_tracks)
{
   if (n_tracks > 1)
   {
      gtk_widget_show(GTK_WIDGET(ojo_controlbox->prev_track_button)) ;
      gtk_widget_show(GTK_WIDGET(ojo_controlbox->next_track_button)) ;
   }
   else
   {
      gtk_widget_hide(GTK_WIDGET(ojo_controlbox->prev_track_button)) ;
      gtk_widget_hide(GTK_WIDGET(ojo_controlbox->next_track_button)) ;
   }
}

void ojo_controlbox_fullscreen_button_set(gboolean fullscreen_mode)
{
   fullscreen_mode ?
   gtk_button_set_image(GTK_BUTTON(ojo_controlbox->fullscreen_button),
                        gtk_image_new_from_icon_name("view-fullscreen", GTK_ICON_SIZE_BUTTON))
   :
   gtk_button_set_image (GTK_BUTTON(ojo_controlbox->fullscreen_button),
                         gtk_image_new_from_icon_name("view-restore", GTK_ICON_SIZE_BUTTON)) ;
}

void ojo_controlbox_repeat_button_set(int repeat_mode)
{
   if (repeat_mode == 0)
      gtk_button_set_image(ojo_controlbox->repeat_button,
                           gtk_image_new_from_icon_name("media-repeat-none", GTK_ICON_SIZE_BUTTON)) ;
   else if (repeat_mode == 1)
      gtk_button_set_image(ojo_controlbox->repeat_button,
                           gtk_image_new_from_icon_name("media-repeat-track-amarok", GTK_ICON_SIZE_BUTTON)) ;
   else
      gtk_button_set_image(ojo_controlbox->repeat_button,
                           gtk_image_new_from_icon_name("media-repeat-all", GTK_ICON_SIZE_BUTTON)) ;
}

char *time_to_string(double current_time, double duration)
{
   current_time /= 1000 ;
   duration /= 1000 ;

   int cur_minutes = ((int)current_time)/60 ; int all_minutes = ((int)duration)/60 ; 
   int cur_seconds = ((int)current_time)%60 ; int all_seconds = ((int)duration)%60 ;

   if (cur_minutes > 59 || all_minutes > 59)
   {
       int cur_hours = ((int)cur_minutes)/60 ; int all_hours = ((int)all_minutes)/60 ;
       sprintf(time_string, "%02d:%02d:%02d / %02d:%02d:%02d  ", cur_hours, cur_minutes-(cur_hours*60), cur_seconds, all_hours, all_minutes-(all_hours*60), all_seconds) ;
   }
   else
   {
      sprintf(time_string, "%02d:%02d / %02d:%02d  ", cur_minutes, cur_seconds, all_minutes, all_seconds) ;
   }

   return time_string ;
}


