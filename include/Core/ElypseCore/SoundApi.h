/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_SOUND_API_H___
#define ___ELYPSE_SOUND_API_H___

#include "Module_Media.h"

#include <Url.h>

namespace Elypse::Media::api
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual SoundPtr CreateStream( Url const & url, bool global ) = 0;
		virtual SoundChannelPtr CreateChannel( SoundObject const & object ) = 0;
		virtual bool AddToMusic( SoundChannel const & toAdd ) = 0;
		virtual bool AddToSFX( SoundChannel const & toAdd ) = 0;
		virtual bool Update3D( const Vector3 & position, const Real * up, const Real * front, Real time ) = 0;
		virtual bool Update() = 0;

		inline bool IsInitialised()
		{
			return m_initialised;
		}

	protected:
		bool m_initialised{ false };
	};

	class SoundChannel
	{
	public:
		virtual ~SoundChannel() = default;

		virtual bool Enable3D() = 0;
		virtual bool SetVelocity( const Vector3 & velocity ) = 0;
		virtual void SetPosition( const Vector3 & position ) = 0;
		virtual bool Update3D( const Vector3 & position, Real time ) = 0;
		virtual bool GetTimePosition( uint32_t & position )const = 0;
		virtual bool SetTimePosition( uint32_t position ) = 0;
		virtual bool Loop( bool loop ) = 0;
		virtual bool Mute( bool mute ) = 0;
		virtual bool SetVolume( Real volume ) = 0;
		virtual bool Pause( bool pause ) = 0;
		virtual bool Restart() = 0;
		virtual bool Stop() = 0;
	};

	class Sound
	{
	public:
		virtual ~Sound() = default;

		virtual bool GetLength( uint32_t & length ) = 0;
	};
}

#endif
