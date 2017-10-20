/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_INSTANCE_H___
#define ___TROLL_INSTANCE_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <ElypseInstance.h>

namespace Troll
{
	namespace ElypseRW
	{
		class TrollInstance
			: public ElypseInstance
		{
		public:
			TrollInstance( String const & p_installPath, ElypsePlugin * p_plugin );
			virtual ~TrollInstance();

			virtual void CreateScene();

			inline bool IsSceneCreated()const
			{
				return m_sceneCreated;
			}

		private:
			bool m_sceneCreated;
		};
	}
}

#endif
