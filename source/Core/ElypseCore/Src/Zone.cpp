/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Zone.h"
#include "ZoneObject.h"
#include "Portal.h"

#include "ScriptNode.h"
#include "DataFile.h"
#include "ConfigFile.h"

#include "ElypseLogs.h"

Zone::Zone( String const & p_name, Universe & p_parent )
	: named( p_name )
	, owned_by< Universe >( p_parent )
	, m_loaded( false )
	, m_firstInit( true )
{
	//std::cout << "Zone created : " << m_name << std::endl;
}

Zone::~Zone()
{
	Unload();
	General::Utils::map::deleteAll( m_portals );

	for ( size_t i = 0 ; i < m_loadScriptNodes.size() ;  i ++ )
	{
		if ( m_loadScriptNodes[i] != NULL )
		{
			m_loadScriptNodes[i]->Delete();
		}
	}

	for ( size_t i = 0 ; i < m_unloadScriptNodes.size() ;  i ++ )
	{
		if ( m_unloadScriptNodes[i] != NULL )
		{
			m_unloadScriptNodes[i]->Delete();
		}
	}
}

void Zone::AddPortal( Portal * p_portal )
{
	genlib_assert( p_portal != NULL );
	m_portals.insert( std::make_pair( p_portal->GetName(), p_portal ) );
}

void Zone::ClearPortals()
{
	General::Utils::map::deleteAll( m_portals );
}

void Zone::Load()
{
	EMUSE_MESSAGE_DEBUG( "Zone::Load -> " + m_name );

	if ( ! m_loaded )
	{
		m_loaded = true;
		General::Utils::vector::cycle( m_dataFiles, & DataFile::Use );
		General::Utils::vector::cycle( m_sceneFiles, & ConfigFile::Use );
		General::Utils::vector::cycle( m_loadScriptFiles, & ConfigFile::Use );
		General::Utils::vector::cycle( m_unloadScriptFiles, & ConfigFile::Use );
	}
}

void Zone::Unload()
{
	EMUSE_MESSAGE_DEBUG( "Zone::Unload -> " + m_name );

	if ( m_loaded )
	{
		m_loaded = false;
		General::Utils::vector::cycle( m_dataFiles, & DataFile::Release );
		General::Utils::vector::cycle( m_sceneFiles, & ConfigFile::Release );
		General::Utils::vector::cycle( m_loadScriptFiles, & ConfigFile::Release );
		General::Utils::vector::cycle( m_unloadScriptFiles, & ConfigFile::Release );
		General::Utils::set::cycle( m_objects, & ZoneObject::RemoveFromZone, this );
		m_objects.clear();
	}
}

void Zone::AddZoneObject( ZoneObject * p_object )
{
	genlib_assert( p_object != NULL );
	m_objects.insert( p_object );
	p_object->AddToZone( this );
}

void Zone::RemoveZoneObject( ZoneObject * p_object )
{
	genlib_assert( p_object != NULL );
	General::Utils::set::eraseValue( m_objects, p_object );
}
