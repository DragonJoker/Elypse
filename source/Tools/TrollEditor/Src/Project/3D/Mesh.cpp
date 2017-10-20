/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Mesh.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			TrollMesh::TrollMesh( wxString const & p_meshName )
				: m_meshName( p_meshName )
			{
			}

			TrollMesh::~TrollMesh()
			{
				m_subMaterials.clear();
			}

			wxString TrollMesh::GetMaterial( uint32_t p_index )
			{
				if ( p_index >= m_subMaterials.size() )
				{
					return wxString();
				}

				return m_subMaterials[p_index];
			}

			void TrollMesh::AddSubMaterial( wxString const & p_name )
			{
				m_subMaterials[uint32_t( m_subMaterials.size() )] = p_name;
			}

			void TrollMesh::SetSubMaterial( wxString const & p_name, uint32_t p_index )
			{
				if ( p_index >= m_subMaterials.size() )
				{
					return;
				}

				m_subMaterials[p_index] = p_name;
			}
		}
	}
}
