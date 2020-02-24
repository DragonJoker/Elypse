/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SoundManager.h"
#include "SoundObject.h"
#include "SoundPlaylist.h"
#include "SoundInstance.h"

#include "ElypseLogs.h"

#include <OgreEntity.h>
#include <OgreLog.h>
#include <StringConverter.h>

GENLIB_INIT_SINGLETON( SoundManager );

namespace
{
	template< typename T, size_t N >
	size_t countOf( T const ( &value )[N] )
	{
		return N;
	}

	void normalize( FMOD_VECTOR  & p_vector )
	{
		auto norm = p_vector.x * p_vector.x
			+ p_vector.y * p_vector.y
			+ p_vector.z * p_vector.z;
		norm = sqrt( norm );

		if ( norm )
		{
			p_vector.x /= norm;
			p_vector.y /= norm;
			p_vector.z /= norm;
		}
	}
}

SoundManager::SoundManager( const String  & p_basePath )
	: m_basePath( p_basePath )
	, m_musicVolume( 0.0 )
	, m_SFXVolume( 0.0 )
	, m_musicMuted( false )
	, m_SFXMuted( false )
	, m_initialised( false )
	, m_musicGroup( nullptr )
	, m_SFXGroup( nullptr )
	, m_system( nullptr )
{
	GENLIB_SET_SINGLETON();
	uint32_t l_version;
	srand( uint32_t( time( nullptr ) ) );
	FMOD_VECTOR l_up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR l_forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
	m_listenerPos.x = 0.0f;
	m_listenerPos.y = 0.0f;
	m_listenerPos.z = 0.0f;
	m_lastListenerPos.x = 0.0f;
	m_lastListenerPos.y = 0.0f;
	m_lastListenerPos.z = 0.0f;

	try
	{
		FMOD_RESULT l_res = FMOD::System_Create( & m_system );

		if ( l_res != FMOD_OK )
		{
			return;
		}

		l_res = m_system->getVersion( & l_version );

		if ( l_res != FMOD_OK )
		{
			return;
		}
	}
	catch ( ... )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "FMOD::System_Create ou m_system->getVersion -> exception" );
		return;
	}

	if ( l_version < FMOD_VERSION )
	{
		size_t l_len = 0;
		_logMessage( String( "Warning! You are using an old version of FMOD [0x" )
					 + uitoa( l_version, l_len, 16 ) + "].  This program requires [0x"
					 + uitoa( FMOD_VERSION, l_len, 16 ) + "]" );
	}

#if ELYPSE_LINUX
	FMOD_OUTPUTTYPE l_outputMode[5] =
	{
		FMOD_OUTPUTTYPE_AUTODETECT,
		FMOD_OUTPUTTYPE_ALSA,
		FMOD_OUTPUTTYPE_OSS,
		FMOD_OUTPUTTYPE_ESD,
		FMOD_OUTPUTTYPE_NOSOUND
	};
#elif ELYPSE_WINDOWS
	FMOD_OUTPUTTYPE l_outputMode[7] =
	{
		FMOD_OUTPUTTYPE_AUTODETECT,
		//FMOD_OUTPUTTYPE_OPENAL,
		FMOD_OUTPUTTYPE_WASAPI,
		FMOD_OUTPUTTYPE_ASIO,
		FMOD_OUTPUTTYPE_NOSOUND
	};
#endif

	for ( size_t i = 0 ; i < countOf( l_outputMode ); i ++ )
	{
		CHECKFMODERROR( m_system->setOutput( l_outputMode[i] ) );

		if ( CHECKFMODERROR( m_system->init( MAXCHANNEL, FMOD_INIT_NORMAL, 0 ) ) )
		{
			_logMessage( "FMOD is initialized with Output type : [" + SoundManager::OutputTypeToStr( l_outputMode[i] ) + "]" );
			break;
		}
	}

	CHECKFMODERROR( m_system->createChannelGroup( "Music", & m_musicGroup ) );
	CHECKFMODERROR( m_system->createChannelGroup( "SFX", & m_SFXGroup ) );
	normalize( l_forward );
	normalize( l_up );
	CHECKFMODERROR( m_system->set3DListenerAttributes( 0, & m_listenerPos, & l_velocity, & l_forward, & l_up ) );
}

SoundManager::~SoundManager()
{
	Cleanup();

	if ( m_musicGroup != nullptr )
	{
		CHECKFMODERROR( m_musicGroup->stop() );
		CHECKFMODERROR( m_musicGroup->release() );
		CHECKFMODERROR( m_SFXGroup->stop() );
		CHECKFMODERROR( m_SFXGroup->release() );
	}

	if ( m_system != nullptr )
	{
		CHECKFMODERROR( m_system->close() );
		CHECKFMODERROR( m_system->release() );
	}
}

void SoundManager::AddPlaylist( SoundPlaylist * p_playlist )
{
	if ( ! m_initialised )
	{
		return;
	}

	if ( p_playlist == nullptr )
	{
		return;
	}

	if ( General::Utils::map::has( m_playlists, p_playlist->GetName() ) )
	{
		_logMessage( "SoundManager::AddPlaylist - Can't add the playlist " + p_playlist->GetName() + " because it already exists" );
		return;
	}

	m_playlists.insert( std::make_pair( p_playlist->GetName(), p_playlist ) );
}

SoundObject * SoundManager::CreateSound( String const & p_name )
{
	if ( ! m_initialised )
	{
		return nullptr;
	}

	SoundObject * l_sound = General::Utils::map::findOrNull( m_sounds, p_name );

	if ( l_sound != nullptr )
	{
		_logMessage( "SoundManager::AddPlaylist - Can't add the sound " + p_name + " because it already exists" );
		return l_sound;
	}

	l_sound = new SoundObject( p_name );
	m_sounds.insert( std::make_pair( p_name, l_sound ) );
	return l_sound;
}

void SoundManager::RemoveSoundObject( String const & p_name )
{
	if ( ! m_initialised )
	{
		return;
	}

	General::Utils::map::deleteValue( m_sounds, p_name );
}

void SoundManager::RegisterInstance( SoundInstance * p_instance )
{
	if ( ! m_initialised )
	{
		return;
	}

	if ( p_instance == nullptr )
	{
		return;
	}

	SceneNode * l_node = p_instance->GetNode();

	if ( l_node == nullptr )
	{
		return;
	}

	SoundNodeMap::iterator ifind = m_nodeInstances.find( l_node->getName() );

	if ( ifind == m_nodeInstances.end() )
	{
		ifind = m_nodeInstances.insert( std::make_pair( l_node->getName(), SoundInstanceSet() ) ).first;
	}

	ifind->second.insert( p_instance );
}

void SoundManager::RemoveNode( String const & p_nodeName )
{
	if ( ! m_initialised )
	{
		return;
	}

	const SoundNodeMap::iterator & ifind = m_nodeInstances.find( p_nodeName );

	if ( ifind == m_nodeInstances.end() )
	{
		return;
	}

	const SoundInstanceSet & l_set = ifind->second;
	SoundInstanceSet::const_iterator l_iter = l_set.begin();
	const SoundInstanceSet::const_iterator & l_end = l_set.end();

	for ( ; l_iter != l_end ; ++ l_iter )
	{
		( *l_iter )->GetOwner()->RemoveNode( p_nodeName );
	}

	m_nodeInstances.erase( ifind );
}


void SoundManager::SetVolumePercent( String const & p_nodeName, Real p_percent )
{
	if ( ! m_initialised )
	{
		return ;
	}

	p_percent = minmax <Real> ( 0.0, p_percent, 1.0 );
	const SoundNodeMap::iterator & ifind = m_nodeInstances.find( p_nodeName );

	if ( ifind == m_nodeInstances.end() )
	{
		return;
	}

	const SoundInstanceSet & l_set = ifind->second;
	SoundInstanceSet::const_iterator l_iter = l_set.begin();
	const SoundInstanceSet::const_iterator & l_end = l_set.end();

	for ( ; l_iter != l_end ; ++ l_iter )
	{
		( *l_iter )->SetVolumePercent( p_percent );
	}
}

/*
bool SoundManager::IsSoundObject( String const &  p_name)
{
	SoundObjectMap::iterator l_it = m_objectList.begin();
	const SoundObjectMap::iterator & l_end = m_objectList.end();

	for( ; l_it != l_end ; ++l_it)
	{
		if (l_it->second->GetMediaEntity( p_name) != nullptr)
		{
			return true;
		}
	}

	return false;
}
*/

void SoundManager::Cleanup()
{
	if ( ! m_initialised )
	{
		return;
	}

	General::Utils::map::deleteAll( m_sounds );
	General::Utils::map::deleteAll( m_playlists );
}

void SoundManager::StartAll()
{
	if ( ! m_initialised )
	{
		return;
	}

	General::Utils::map::cycle( m_sounds, &SoundObject::PlayAll );
}

void SoundManager::StopAll()
{
	if ( ! m_initialised )
	{
		return;
	}

	General::Utils::map::cycle( m_sounds, &SoundObject::StopAll );
}

void SoundManager::PauseAll()
{
	if ( ! m_initialised )
	{
		return;
	}

	General::Utils::map::cycle( m_sounds, &SoundObject::PauseAll );
}

void SoundManager::SetVolume( Real p_volume, SoundType p_type )
{
	if ( ! m_initialised )
	{
		return;
	}

	p_volume = minmax<Real>( MINVOLUME, p_volume, MAXVOLUME );
	SoundObjectMap::iterator l_it = m_sounds.begin();
	const SoundObjectMap::iterator & l_end = m_sounds.end();

	if ( p_type == EM_MUSIC )
	{
		m_musicVolume = p_volume;
	}
	else
	{
		m_SFXVolume = p_volume;
	}

	for ( ; l_it != l_end ; ++ l_it )
	{
		if ( l_it->second->GetSoundType() == p_type )
		{
			l_it->second->SetMaxVolume( p_volume );
		}
	}
}

void SoundManager::Mute( bool p_mute, SoundType p_type )
{
	if ( ! m_initialised )
	{
		return;
	}

	if ( p_type == EM_SFX )
	{
		m_SFXMuted = p_mute;
	}
	else if ( p_type == EM_MUSIC )
	{
		m_musicMuted = p_mute;
	}

	SoundObjectMap::iterator l_it = m_sounds.begin();
	const SoundObjectMap::iterator & l_end = m_sounds.end();

	for ( ; l_it != l_end ; ++ l_it )
	{
		if ( l_it->second->GetSoundType() == p_type )
		{
			l_it->second->Mute( p_mute );
		}
	}
}

void SoundManager::Update( const Vector3 & p_position,
						   const Real * p_up,
						   const Real * p_forward,
						   Real p_updateTime )
{
	if ( ! m_initialised )
	{
		return;
	}

	FMOD_VECTOR l_up = { p_up[0], p_up[1], p_up[2] };
	FMOD_VECTOR l_forward = { p_forward[0], p_forward[1], p_forward[2] };
	FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
	m_listenerPos.x = p_position.x / 100;
	m_listenerPos.y = p_position.y / 100;
	m_listenerPos.z = p_position.z / 100;
	l_velocity.x = ( m_listenerPos.x - m_lastListenerPos.x ) / p_updateTime;
	l_velocity.y = ( m_listenerPos.y - m_lastListenerPos.y ) / p_updateTime;
	l_velocity.z = ( m_listenerPos.z - m_lastListenerPos.z ) / p_updateTime;
	m_lastListenerPos = m_listenerPos;
	CHECKFMODERROR( m_system->set3DListenerAttributes( 0, & m_listenerPos, & l_velocity, & l_forward, & l_up ) );
	_update( p_updateTime );
	CHECKFMODERROR( m_system->update() );
}

void SoundManager::_update( Real p_time )
{
	if ( ! m_initialised )
	{
		return;
	}

	SoundObjectMap::iterator l_it = m_sounds.begin();
	const SoundObjectMap::iterator & l_end = m_sounds.end();

	for ( ; l_it != l_end ; ++ l_it )
	{
		l_it->second->UpdateFade( p_time );
		l_it->second->Update3D( p_time );
	}

	General::Utils::map::cycle( m_playlists, &SoundPlaylist::Update, p_time );
}

void SoundManager::_logMessage( String const & p_msg )
{
	EMUSE_MESSAGE_RELEASE( p_msg );
}

bool SoundManager::CheckFMODError( FMOD_RESULT p_result, uint32_t p_line, char const * const p_file )
{
	if ( p_result == FMOD_OK )
	{
		return true;
	}

	char const * const l_errorString = FMOD_ErrorString( p_result );
	String l_msg = String( "FMOD error (" ) + StringConverter::toString( p_result ) + ") : [ " + l_errorString + "] in \"" + p_file + "\"@" + StringConverter::toString( p_line );
	SoundManager::GetSingletonPtr()->_logMessage( l_msg );
	return false;
}

const String SoundManager::OutputTypeToStr( FMOD_OUTPUTTYPE p_outputType )
{
	switch ( p_outputType )
	{
	case FMOD_OUTPUTTYPE_AUTODETECT:
		return "Autodetect";

	case FMOD_OUTPUTTYPE_NOSOUND:
		return "No sound";

	case FMOD_OUTPUTTYPE_WASAPI:
		return "WASAPI";

	case FMOD_OUTPUTTYPE_ASIO:
		return "ASIO";

	case FMOD_OUTPUTTYPE_ALSA:
		return "ALSA";

	case FMOD_OUTPUTTYPE_COREAUDIO:
		return "Core Audio";

	case FMOD_OUTPUTTYPE_WAVWRITER:
		return "Wave writer";

	case FMOD_OUTPUTTYPE_NOSOUND_NRT:
		return "Non real-time no sound";

	case FMOD_OUTPUTTYPE_WAVWRITER_NRT:
		return "Non real-time Wave writer";

	case FMOD_OUTPUTTYPE_UNKNOWN:
		return "Unknown";

	case FMOD_OUTPUTTYPE_MAX:
		break;

	case FMOD_OUTPUTTYPE_FORCEINT:
		break;
	}

	return "[" + Ogre::StringConverter::toString( p_outputType ) + "] isn't a valid output type...";
}
