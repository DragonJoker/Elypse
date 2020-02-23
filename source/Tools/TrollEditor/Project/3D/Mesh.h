/*
See LICENSE file in root folder
*/
#ifndef _TrollMesh_H_
#define _TrollMesh_H_

#include "Module_3D.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollMesh
			{
			public:
				TrollMesh( wxString const & p_meshName );
				~TrollMesh();

				void AddSubMaterial( wxString const & p_name );
				void SetSubMaterial( wxString const & p_name, uint32_t p_index );
				wxString GetMaterial( uint32_t p_index );

				inline wxString const & GetName()const
				{
					return m_meshName;
				}
				inline uint32_t GetNumSubMaterials()const
				{
					return uint32_t( m_subMaterials.size() );
				}

			private:
				wxString m_meshName;
				wxStringUIntMap m_subMaterials;
			};
		}
	}
}

#endif
