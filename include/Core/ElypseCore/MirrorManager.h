/*
See LICENSE file in root folder
*/
#ifndef ___MIRROR_MANAGER_H___
#define ___MIRROR_MANAGER_H___

#include "Module_SpecialEffects.h"

#include <Manager.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export MirrorManager
			: public Manager< Mirror >
		{
			String m_instanceKey;
		public:
			MirrorManager( String const & p_instanceKey );
			~MirrorManager();

		public:
			void UpdateAll()const;

			inline String const & GetInstanceKey()const
			{
				return m_instanceKey;
			}
		};
	}
}

#endif
