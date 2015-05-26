/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___MIRROR_MANAGER_H___
#define ___MIRROR_MANAGER_H___

#include "Module_SpecialEffects.h"

#include <Manager.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class d_dll_export MirrorManager : public Manager<Mirror>
		{
			String m_instanceKey;
		public:
			MirrorManager( const String & p_instanceKey );
			~MirrorManager();

		public:
			void UpdateAll()const;

			inline const String & GetInstanceKey()const
			{
				return m_instanceKey;
			}
		};
	}
}

#endif
