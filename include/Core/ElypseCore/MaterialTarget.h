/*
See LICENSE file in root folder
*/
#ifndef ___MATERIAL_TARGET_H___
#define ___MATERIAL_TARGET_H___

#include "Module_SpecialEffects.h"
#include "Module_Gui.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class MaterialTarget
		{
		public:
			virtual ~MaterialTarget() {}
		public:
			virtual void Apply( String const & p_materialName ) = 0;
		};

		class MaterialTarget_Entity : public MaterialTarget
		{
		private:
			Entity * m_entity;

		public:
			MaterialTarget_Entity( Entity * p_entity )
				: m_entity( p_entity )
			{
			}
			virtual ~MaterialTarget_Entity() {}

		public:
			virtual void Apply( String const & p_materialName );
		};

		class MaterialTarget_SubEntity : public MaterialTarget
		{
		private:
			SubEntity * m_subEntity;

		public:
			MaterialTarget_SubEntity( SubEntity * p_subEntity )
				: m_subEntity( p_subEntity )
			{
			}
			virtual ~MaterialTarget_SubEntity() {}

		public:
			virtual void Apply( String const & p_materialName );
		};

		class MaterialTarget_Overlay : public MaterialTarget
		{
		private:
			EMOverlay * m_overlay;

		public:
			MaterialTarget_Overlay( EMOverlay * p_overlay )
				: m_overlay( p_overlay )
			{
			}
			virtual ~MaterialTarget_Overlay() {}

		public:
			virtual void Apply( String const & p_materialName );
		};
	}
}

#endif
