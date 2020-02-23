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
			TrollInstance( Path const & p_installPath, ElypsePlugin * p_plugin );
			virtual ~TrollInstance();

			void CreateScene()override;

			inline bool IsSceneCreated()const
			{
				return m_sceneCreated;
			}

		protected:
			void _createNewFramelistener()override;
			void _createLinkedFramelistener()override;

		private:
			bool m_sceneCreated;
		};
	}
}

#endif
