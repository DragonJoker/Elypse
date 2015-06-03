#include <wx/string.h>
#include <map>

namespace Troll
{
	class TrollObject;
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
	}
}

typedef std::map <wxString,		wxString>									StrStrMap;
typedef std::map <wxString,		Troll::Temporal::TrollAnimationGroup *>		TrollAnimationGroupStrMap;
typedef std::map <wxString,		Troll::Temporal::TrollAnimation *>			TrollAnimationStrMap;
typedef std::map <wxString,		Troll::Temporal::TrollAnimatedObject *>		TrollAnimatedObjectStrMap;
typedef std::multimap <float,	Troll::Temporal::TrollSequence *>			TrollSequenceRealMap;
typedef std::multimap <float,	Troll::Temporal::TrollPonctualEvent *>		TrollPonctualEventMap;
typedef std::multimap <float,	Troll::Temporal::TrollContinuousEvent *>	TrollContinuousEventMap;
typedef std::map <float,		Troll::Temporal::TrollKeyFrame *>			TrollKeyFrameMap;
typedef std::map <wxString,		Troll::Temporal::TrollSequence *>			TrollSequenceStrMap;