/*
See LICENSE file in root folder
*/
#ifndef ___VIDEOINSTANCE_H___
#define ___VIDEOINSTANCE_H___

#include "Module_Media.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoInstance
			: public owned_by< VideoObject >
		{
		public:
			VideoInstance( VideoObject & p_owner, Entity * p_entity );
			~VideoInstance();

			void Initialise( String const & p_videoTextureName );
			void Reset();
			void SetNoSubMaterial( uint16_t p_noSubMat );

			inline Entity * GetEntity()const
			{
				return m_entity;
			}
			inline short GetNoSubMaterial()const
			{
				return m_noSubMaterial;
			}
			inline bool IsLoadDelayed()const
			{
				return m_delayLoad;
			}

			inline void DelayLoad()
			{
				m_delayLoad = true;
			}

		private:
			Entity * m_entity;
			short m_noSubMaterial;
			String m_oldMaterialName;
			bool m_delayLoad;
		};
	}
}

#endif
