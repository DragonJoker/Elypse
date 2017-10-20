/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_GUI_TIME_H___
#define ___MODULE_GUI_TIME_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Temporal/Module_Temporal.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class ArrowPanel;
			class ContinuousEventPanel;
			class LinePanel;
			class PonctualEventPanel;
			class SequencePanel;
			class TimeLineContainer;
			class TimeLinePanel;
			class TimePanel;
			class TimeTick;

			typedef std::multimap< int, int > IntIMultiMap;
			typedef std::map< int, IntIMultiMap > DimensionMap;
		}
	}
}

#endif
