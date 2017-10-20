/*
See LICENSE file in root folder
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
