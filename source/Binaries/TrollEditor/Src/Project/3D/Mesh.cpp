
#include "PrecompiledHeader.h"

#include "Project/3D/Mesh.h"

using namespace Troll::Objects3D;


TrollMesh :: TrollMesh( const wxString & p_meshName )
	:	m_meshName( p_meshName )
{
}



TrollMesh :: ~TrollMesh()
{
	m_subMaterials.clear();
}



wxString TrollMesh :: GetMaterial( unsigned int p_index )
{
	if ( p_index >= m_subMaterials.size() )
	{
		return wxString();
	}

	return m_subMaterials[p_index];
}



void TrollMesh :: AddSubMaterial( const wxString & p_name )
{
	m_subMaterials[m_subMaterials.size()] = p_name;
}



void TrollMesh :: SetSubMaterial( const wxString & p_name, unsigned int p_index )
{
	if ( p_index >= m_subMaterials.size() )
	{
		return;
	}

	m_subMaterials[p_index] = p_name;
}


