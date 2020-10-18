/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_FMOD_API_H___
#define ___ELYPSE_FMOD_API_H___

#if ELYPSE_USE_FMOD
#include "SoundApi.h"

#include <fmod.hpp>

namespace Elypse::Media::fmod
{
	//*********************************************************************************************

	struct SoundSystem
		: public api::SoundSystem
	{
		SoundSystem();
		~SoundSystem();

		api::SoundPtr CreateStream( Url const & url, bool global )override;
		api::SoundChannelPtr CreateChannel( SoundObject const & object ) override;
		bool AddToMusic( api::SoundChannel const & toAdd )override;
		bool AddToSFX( api::SoundChannel const & toAdd )override;
		bool Update3D( const Vector3 & position, const Real * up, const Real * front, Real time )override;
		bool Update()override;

		FMOD::System * system{};
		FMOD::ChannelGroup * musicGroup{};
		FMOD::ChannelGroup * sfxGroup{};
		FMOD_VECTOR lastListenerPos{};
	};

	//*********************************************************************************************

	struct Sound
		: public api::Sound
	{
	public:
		Sound( FMOD::System * fmodSystem, Url const & url, FMOD_MODE mode );
		~Sound();

		bool GetLength( uint32_t & length )override;

		FMOD::Sound * fmodSound{};
	};

	//*********************************************************************************************

	struct SoundChannel
		: public api::SoundChannel
	{
		SoundChannel( FMOD::System * fmodSystem, SoundObject const & object );

		bool Enable3D()override;
		bool SetVelocity( const Vector3 & velocity )override;
		void SetPosition( const Vector3 & position )override;
		bool Update3D( const Vector3 & position, Real time )override;
		bool GetTimePosition( uint32_t & position )const override;
		bool SetTimePosition( uint32_t position );
		bool Loop( bool loop )override;
		bool Mute( bool mute )override;
		bool SetVolume( Real volume );
		bool Pause( bool pause )override;
		bool Restart()override;
		bool Stop()override;

		FMOD::System * fmodSystem{};
		FMOD::Sound * fmodSound{};
		FMOD::Channel * fmodChannel{};
		FMOD_VECTOR position{};
	};

	//*********************************************************************************************
}

#endif
#endif
