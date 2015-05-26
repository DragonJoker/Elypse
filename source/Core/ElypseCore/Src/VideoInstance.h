#ifndef ___VIDEOINSTANCE_H___
#define ___VIDEOINSTANCE_H___

#include "Module_Media.h"

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoInstance : public owned_by <VideoObject>
		{
		private:
			Entity * m_entity;
			short m_noSubMaterial;
			String m_oldMaterialName;
			bool m_delayLoad;

		public:
			VideoInstance( VideoObject * p_owner, Entity * p_entity );
			~VideoInstance();
			void Initialise( const String & p_videoTextureName );
			void Reset();
			void SetNoSubMaterial( unsigned short p_noSubMat );

		public:
			inline Entity 	*	GetEntity()const
			{
				return m_entity;
			}
			inline short		GetNoSubMaterial()const
			{
				return m_noSubMaterial;
			}
			inline bool			IsLoadDelayed()const
			{
				return m_delayLoad;
			}

			inline void		DelayLoad()
			{
				m_delayLoad = true;
			}

		};
	}
}

#endif
