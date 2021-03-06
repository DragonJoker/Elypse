/*
See LICENSE file in root folder
*/
#ifndef ___PUB_INSTANCE_H___
#define ___PUB_INSTANCE_H___

#include "Module_Pub.h"

#include "ZoneObject.h"

#include <OwnedBy.h>

namespace Elypse
{
	namespace Pub
	{
		class d_dll_export PubInstance
			: public ZoneObject
			, public owned_by< PubObject >
		{
		public:
			PubInstance( PubObject & p_owner, Entity * p_entity );
			~PubInstance();

		public:
			void Apply();

			virtual void AddToZone( Zone * p_zone )
			{
				m_zone = p_zone;
			}
			virtual void RemoveFromZone( Zone * p_zone );

			inline Entity * GetEntity()const
			{
				return m_entity;
			}

			inline void SetSubMat( uint32_t p_submat )
			{
				m_submat = p_submat;
			}

		private:
			Entity * m_entity;
			Zone * m_zone;
			uint32_t m_submat;
		};
	}
}

#endif
