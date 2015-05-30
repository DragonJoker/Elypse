#include "RegistryManager.h"

#ifdef WIN32

# include "RegistryKey.h"
# include "STLMacros.h"

using namespace General;
using namespace General::Computer;
/*
RegistryManager::RegistryManager()
	:	m_folder (RF_HKLM)
{
}

RegistryManager::~RegistryManager()
{
	map_delete( m_registeryKeys);
}

void RegistryManager::SetFolder( RegistryFolder p_folder)
{
	m_folder = p_folder;
	map_delete( m_registeryKeys);
}

RegistryKey * RegistryManager::_getKey( const std::string & p_keyName)
{
	return map_findOrNull( m_registeryKeys, p_keyName);
}

RegistryKey * RegistryManager::_createKey( const std::string & p_keyName)
{
	RegistryKey * l_key = new RegistryKey( m_folder, p_keyName);
	m_registeryKeys.insert( RegistryMap::value_type( p_keyName, l_key));
	return l_key;
}

RegistryKey * RegistryManager::_getOrCreateKey( const std::string & p_keyName)
{
	RegistryKey * l_key = _getKey( p_keyName);

	if (l_key == NULL)
	{
		l_key = _createKey( p_keyName);
	}

	return l_key;
}

bool RegistryManager::KeyExists( const std::string & p_keyName)
{
	return _getOrCreateKey( p_keyName)->IsValid();
}

bool RegistryManager::CreateKey( const std::string & p_keyName)
{
	return _getOrCreateKey( p_keyName)->Create();
}

RegistryKey * RegistryManager::GetKey( const std::string & p_keyName)
{
	RegistryKey * l_key = _getKey( p_keyName);

	if (l_key != NULL)
	{
		l_key->Create();
		return l_key;
	}

	l_key = _createKey( p_keyName);

	if (l_key->IsValid())
	{
		l_key->Create();
		return l_key;
	}

	return NULL;
}

bool RegistryManager::DeleteKey( const std::string & p_keyName)
{
	return _getOrCreateKey( p_keyName)->Delete();
}
*/
#endif
