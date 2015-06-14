/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___VERSION_H___
#define ___VERSION_H___

#define ELYPSE_VERSION_MAIN			0
#define ELYPSE_VERSION_SUB			9
#define ELYPSE_VERSION_REVISION		4
#define ELYPSE_VERSION_PATCH		0
#define ELYPSE_VERSION ((ELYPSE_VERSION_MAIN * 1000000) + (ELYPSE_VERSION_SUB * 10000) + (ELYPSE_VERSION_REVISION * 100) + (ELYPSE_VERSION_PATCH))

#define CHECK_ELYPSE_VERSION( X) ((X) <= (ELYPSE_VERSION))

#endif
