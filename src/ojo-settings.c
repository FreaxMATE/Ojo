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


#include "ojo-settings.h"

OjoSettings* ojo_settings_initialize()
{
   OjoSettings *new ;
   new = malloc (sizeof(OjoSettings)) ;
   new->gsettings = g_settings_new ("org.github.FreaxMATE.Ojo") ;
   return new ;
}

int ojo_settings_get_int(GSettings *gsettings, const gchar *key)
{
   return g_settings_get_int(gsettings, key) ;
}

gboolean ojo_settings_get_boolean(GSettings *gsettings, const gchar *key)
{
   return g_settings_get_boolean(gsettings, key) ;
}

void ojo_settings_set_int(GSettings *gsettings, const gchar *key, int value)
{
   g_settings_set_int(gsettings, key, value) ;
}

void ojo_settings_set_boolean(GSettings *gsettings, const gchar *key, gboolean value)
{
   g_settings_set_boolean(gsettings, key, value) ;
}


