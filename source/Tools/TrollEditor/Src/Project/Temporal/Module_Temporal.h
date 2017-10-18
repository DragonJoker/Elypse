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
#ifndef ___TE_MODULE_TEMPORAL_H___
#define ___TE_MODULE_TEMPORAL_H___

#include "Project/Module_Project.h"

#include <wx/string.h>
#include <map>

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Temporal
		{
			class TemporalObject;

			class TrollAnimationGroup;
			class TrollAnimatedObject;
			class TrollAnimation;

			class TrollSequence;
			class TrollPonctualEvent;
			class TrollContinuousEvent;
			class TrollKeyFrame;

			typedef std::map< wxString, wxString > StrStrMap;
			typedef std::map< wxString, TrollAnimationGroup * > TrollAnimationGroupStrMap;
			typedef std::map< wxString, TrollAnimation * > TrollAnimationStrMap;
			typedef std::map< wxString, TrollAnimatedObject * > TrollAnimatedObjectStrMap;
			typedef std::multimap< float, TrollSequence * > TrollSequenceRealMap;
			typedef std::multimap< float, TrollPonctualEvent * > TrollPonctualEventMap;
			typedef std::multimap< float, TrollContinuousEvent * > TrollContinuousEventMap;
			typedef std::map< float, TrollKeyFrame * > TrollKeyFrameMap;
			typedef std::map< wxString, TrollSequence * > TrollSequenceStrMap;
		}
	}
}

#endif
