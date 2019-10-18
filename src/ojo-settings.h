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

#ifndef _settings_h_
#define _settings_h_

#include <gio/gio.h>

#include "ojo.h"
#include "ojo-window.h"

typedef struct _OjoSettings
{
   GSettings *gsettings ;
  
} OjoSettings ;

OjoSettings* ojo_settings_initialize() ;
int ojo_settings_get_int(GSettings *gsettings, const gchar *key) ;
gboolean ojo_settings_get_boolean(GSettings *gsettings, const gchar *key) ;
void ojo_settings_set_int(GSettings *gsettings, const gchar *key, int value) ;
void ojo_settings_set_boolean(GSettings *gsettings, const gchar *key, gboolean value) ;

#endif /* _settings_h_ */

