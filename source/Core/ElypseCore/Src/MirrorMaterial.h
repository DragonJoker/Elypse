/*
See LICENSE file in root folder
*/
#ifndef ___MIRROR_MATERIAL_H___
#define ___MIRROR_MATERIAL_H___

#include "Module_SpecialEffects.h"

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export MirrorMaterial
			: public named
			, public owned_by< Mirror >
		{
		public:
			MirrorMaterial( String const & p_name, Mirror & p_owner );
			~MirrorMaterial();

		public:
			void Apply();
			void Remove();

		private:
			void _setOpacity();

		public:
			inline void SetOpacity( Real p_opacity )
			{
				m_opacity = p_opacity;
				_setOpacity();
			}

		private:
			Real m_opacity;
			TextureUnitState * m_tus;
		};
	}
}

#endif
