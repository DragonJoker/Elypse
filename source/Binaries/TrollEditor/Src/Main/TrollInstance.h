
#ifndef ___TROLL_INSTANCE_H___
#define ___TROLL_INSTANCE_H___

#include <EMuseInstance.h>

namespace Troll
{
	namespace Main
	{
		class TrollInstance : public EMuseInstance
		{
		private:
			bool m_sceneCreated;
		public:
			TrollInstance( const String & p_installPath , EMusePlugin * p_plugin );
			virtual ~TrollInstance();

		public:
			virtual void CreateScene();

		public:
			inline bool IsSceneCreated()const
			{
				return m_sceneCreated;
			}
		};
	}
}

#endif
