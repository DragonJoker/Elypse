/*
See LICENSE file in root folder
*/
#if ELYPSE_USE_FMOD
#include "PrecompiledHeader.h"

#include "FmodApi.h"

#include "SoundManager.h"
#include "SoundObject.h"
#include "SoundPlaylist.h"
#include "SoundInstance.h"

#include "ElypseLogs.h"

#include <OgreEntity.h>
#include <OgreLog.h>
#include <StringConverter.h>

#include <fmod_errors.h>

#define CHECKSNDERROR( X) CheckFMODError( X, __LINE__, __FILE__)

namespace Elypse::Media::fmod
{
	namespace
	{
		bool CheckFMODError( FMOD_RESULT result
			, uint32_t line
			, char const * const file )
		{
			if ( result == FMOD_OK )
			{
				return true;
			}

			char const * const errorString = FMOD_ErrorString( result );
			String msg = String( "FMOD error (" ) + StringConverter::toString( result ) + ") : [ " + errorString + "] in \"" + file + "\"@" + StringConverter::toString( line );
			EMUSE_MESSAGE_RELEASE( msg );
			return false;
		}

		const String OutputTypeToStr( FMOD_OUTPUTTYPE outputType )
		{
			switch ( outputType )
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

			return "[" + Ogre::StringConverter::toString( outputType ) + "] isn't a valid output type...";
		}

		void normalize( FMOD_VECTOR & vector )
		{
			auto norm = vector.x * vector.x
				+ vector.y * vector.y
				+ vector.z * vector.z;
			norm = sqrt( norm );

			if ( norm )
			{
				vector.x /= norm;
				vector.y /= norm;
				vector.z /= norm;
			}
		}
	}

	//*********************************************************************************************

	SoundSystem::SoundSystem()
	{
		uint32_t version{};

		try
		{
			FMOD_RESULT res = FMOD::System_Create( &system );

			if ( res != FMOD_OK )
			{
				return;
			}

			res = system->getVersion( &version );

			if ( res != FMOD_OK )
			{
				return;
			}
		}
		catch ( ... )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "FMOD::System_Create ou m_system->getVersion -> exception" );
			return;
		}

		if ( version < FMOD_VERSION )
		{
			size_t len = 0;
			EMUSE_MESSAGE_RELEASE( String( "Warning! You are using an old version of FMOD [0x" )
				+ uitoa( version, len, 16 ) + "].  This program requires [0x"
				+ uitoa( FMOD_VERSION, len, 16 ) + "]" );
		}

#if ELYPSE_LINUX
		FMOD_OUTPUTTYPE outputModes[5] =
		{
			FMOD_OUTPUTTYPE_AUTODETECT,
			FMOD_OUTPUTTYPE_ALSA,
			FMOD_OUTPUTTYPE_OSS,
			FMOD_OUTPUTTYPE_ESD,
			FMOD_OUTPUTTYPE_NOSOUND
		};
#elif ELYPSE_WINDOWS
		FMOD_OUTPUTTYPE outputModes[4] =
		{
			FMOD_OUTPUTTYPE_AUTODETECT,
			FMOD_OUTPUTTYPE_WASAPI,
			FMOD_OUTPUTTYPE_ASIO,
			FMOD_OUTPUTTYPE_NOSOUND
		};
#endif

		for ( auto outputMode : outputModes )
		{
			CHECKSNDERROR( system->setOutput( outputMode ) );

			if ( CHECKSNDERROR( system->init( MAXCHANNEL, FMOD_INIT_NORMAL, 0 ) ) )
			{
				m_initialised = true;
				EMUSE_MESSAGE_RELEASE( "FMOD is initialized with Output type : [" + OutputTypeToStr( outputMode ) + "]" );
				break;
			}
		}

		CHECKSNDERROR( system->createChannelGroup( "Music", &musicGroup ) );
		CHECKSNDERROR( system->createChannelGroup( "SFX", &sfxGroup ) );
		FMOD_VECTOR up{ 0.0f, 1.0f, 0.0f };
		FMOD_VECTOR forward{ 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR velocity{};
		FMOD_VECTOR listener{};
		normalize( forward );
		normalize( up );
		CHECKSNDERROR( system->set3DListenerAttributes( 0, &listener, &velocity, &forward, &up ) );
	}

	SoundSystem::~SoundSystem()
	{
		if ( musicGroup )
		{
			CHECKSNDERROR( musicGroup->stop() );
			CHECKSNDERROR( musicGroup->release() );
			CHECKSNDERROR( sfxGroup->stop() );
			CHECKSNDERROR( sfxGroup->release() );
		}

		if ( system )
		{
			CHECKSNDERROR( system->close() );
			CHECKSNDERROR( system->release() );
		}
	}

	SoundPtr SoundSystem::CreateStream( Url const & url, bool global )
	{
		FMOD_MODE mode = FMOD_DEFAULT;

		if ( !global )
		{
			mode |= FMOD_3D;
		}

		return std::make_unique< Sound >( system, url, mode );
	}

	SoundChannelPtr SoundSystem::CreateChannel( SoundObject const & object )
	{
		return std::make_unique< SoundChannel >( system, object );
	}

	bool SoundSystem::AddToMusic( api::SoundChannel const & toAdd )
	{
		return CHECKSNDERROR( static_cast< SoundChannel const & >( toAdd ).fmodChannel->setChannelGroup( musicGroup ) );
	}

	bool SoundSystem::AddToSFX( api::SoundChannel const & toAdd )
	{
		return CHECKSNDERROR( static_cast< SoundChannel const & >( toAdd ).fmodChannel->setChannelGroup( sfxGroup ) );
	}

	bool SoundSystem::Update3D( const Vector3 & position
		, const Real * up
		, const Real * front
		, Real time )
	{
		FMOD_VECTOR fup{ up[0], up[1], up[2] };
		FMOD_VECTOR fforward{ front[0], front[1], front[2] };
		normalize( fforward );
		normalize( fup );
		FMOD_VECTOR velocity{};
		FMOD_VECTOR listener{};
		listener.x = position.x / 100;
		listener.y = position.y / 100;
		listener.z = position.z / 100;
		velocity.x = ( listener.x - lastListenerPos.x ) / time;
		velocity.y = ( listener.y - lastListenerPos.y ) / time;
		velocity.z = ( listener.z - lastListenerPos.z ) / time;
		lastListenerPos = listener;
		return CHECKSNDERROR( system->set3DListenerAttributes( 0, &listener, &velocity, &fforward, &fup ) );
	}

	bool SoundSystem::Update()
	{
		return CHECKSNDERROR( system->update() );
	}

	//*********************************************************************************************

	Sound::Sound( FMOD::System * fmodSystem
		, Url const & url
		, FMOD_MODE mode )
	{
		if ( CHECKSNDERROR( fmodSystem->createStream( url.c_str(), mode, nullptr, &fmodSound ) ) )
		{
			CHECKSNDERROR( fmodSound->set3DMinMaxDistance( MINDISTSOUND, MAXDISTSOUND ) );
		}
	}

	Sound::~Sound()
	{
		CHECKSNDERROR( fmodSound->release() );
	}

	bool Sound::GetLength( uint32_t & length )
	{
		return CHECKSNDERROR( fmodSound->getLength( &length, FMOD_TIMEUNIT_MS ) );
	}

	//*********************************************************************************************

	SoundChannel::SoundChannel( FMOD::System * fmodSystem
		, SoundObject const & object )
		: fmodSystem{ fmodSystem }
		, fmodSound{ static_cast< Sound & >( object.GetSoundApi() ).fmodSound }
	{
		if ( !CHECKSNDERROR( fmodSystem->playSound( fmodSound, nullptr, true, &fmodChannel ) ) )
		{
			EMUSE_MESSAGE_RELEASE( "Sound " + object.GetName() + ", could not create the sound channel" );
		}
	}

	bool SoundChannel::Enable3D()
	{
		assert( fmodChannel );
		auto result = CHECKSNDERROR( fmodChannel->setMode( FMOD_3D ) );

		if ( result )
		{
			FMOD_VECTOR velocity{};
			result = CHECKSNDERROR( fmodChannel->set3DAttributes( &position, &velocity ) );
		}

		return result;
	}

	bool SoundChannel::SetVelocity( const Vector3 & velocity )
	{
		FMOD_VECTOR fmodVelocity{};
		fmodVelocity.x = velocity.x;
		fmodVelocity.y = velocity.y;
		fmodVelocity.z = velocity.z;
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->set3DAttributes( &position, &fmodVelocity ) );
	}

	void SoundChannel::SetPosition( const Vector3 & newPosition )
	{
		position.x = newPosition.x;
		position.y = newPosition.y;
		position.z = newPosition.z;
	}

	bool SoundChannel::Update3D( const Vector3 & newPosition, Real time )
	{
		FMOD_VECTOR velocity{};
		velocity.x = ( newPosition.x - position.x ) / time;
		velocity.y = ( newPosition.y - position.y ) / time;
		velocity.z = ( newPosition.z - position.z ) / time;
		position.x = newPosition.x;
		position.y = newPosition.y;
		position.z = newPosition.z;
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->set3DAttributes( &position, &velocity ) );
	}

	bool SoundChannel::GetTimePosition( uint32_t & position )const
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->getPosition( &position, FMOD_TIMEUNIT_MS ) );
	}

	bool SoundChannel::SetTimePosition( uint32_t position )
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->setPosition( 0, FMOD_TIMEUNIT_MS ) );
	}

	bool SoundChannel::Loop( bool loop )
	{
		if ( loop )
		{
			return CHECKSNDERROR( fmodChannel->setMode( FMOD_LOOP_NORMAL ) );
		}

		return CHECKSNDERROR( fmodChannel->setMode( FMOD_LOOP_OFF ) );
	}

	bool SoundChannel::Mute( bool mute )
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->setMute( mute ) );
	}

	bool SoundChannel::SetVolume( Real volume )
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->setVolume( volume ) );
	}

	bool SoundChannel::Pause( bool pause )
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->setPaused( pause ) );
	}

	bool SoundChannel::Restart()
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodSystem->playSound( fmodSound
			, nullptr
			, false
			, &fmodChannel ) );
	}

	bool SoundChannel::Stop()
	{
		assert( fmodChannel );
		return CHECKSNDERROR( fmodChannel->stop() );
	}

	//*********************************************************************************************
}
#endif
