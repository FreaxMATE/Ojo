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

#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <vlc/vlc.h>

#include "ojo.h"
#include "ojo-settings.h"
#include "ojo-player.h"
#include "ojo-playlist.h"
#include "ojo-window.h"
#include "ojo-track.h"

void ojo_main_print_usage()
{
   printf ("Ojo Usage:\n") ;
   printf ("ojo [FILEPATH]\n") ;
}

int main(int argc, char **argv)
{
   GSList *list = NULL ;
   if (argc > 1)
   {
      if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
      {
         ojo_main_print_usage() ;
         return 0 ;
      }
   }
   XInitThreads() ;
   gtk_init (&argc, &argv) ;

   ojo_settings = ojo_settings_initialize() ;
   ojo_player = ojo_player_initialize() ;
   ojo_window_setup() ;
   gtk_widget_show(GTK_WIDGET(window)) ;
   if (argc > 1)
   {
      for (int i = 1; i < argc; ++i)
      {
         list = g_slist_append(list, argv[i]) ;
      }
      ojo_window_media_open_prepare(list, FALSE) ;
   }
   gtk_main() ;
   g_settings_sync() ;
   ojo_player_quit() ;

   return 0 ;
}
