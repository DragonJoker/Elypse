
#ifndef ___TROLL_INSTANCE_H___
#define ___TROLL_INSTANCE_H___

#include <EMuseInstance.h>

namespace Troll
{
	namespace Main
	{
		class TrollInstance
			: public EMuseInstance
		{
		public:
			TrollInstance( const String & p_installPath , EMusePlugin * p_plugin );
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
