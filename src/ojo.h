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

#ifndef _ojo_h_
#define _ojo_h_

typedef struct _OjoSettings OjoSettings ;
typedef struct _OjoPlayer OjoPlayer ;
typedef struct _OjoPlaylist OjoPlaylist ;
typedef struct _OjoTrack OjoTrack ;
enum _FileType {
   AUDIO,
   VIDEO,
   UNKNOWN,
} ;
typedef enum _FileType FileType ;

OjoSettings *ojo_settings ;
OjoPlayer   *ojo_player ;
OjoPlaylist *ojo_playlist ;

#endif /* _ojo_h_ */

