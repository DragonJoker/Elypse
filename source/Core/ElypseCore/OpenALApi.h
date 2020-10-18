/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_OPENAL_API_H___
#define ___ELYPSE_OPENAL_API_H___

#if ELYPSE_USE_OPENAL
#include "SoundApi.h"

#include <DynamicLibrary.h>

namespace Elypse::Media::openal
{
	namespace details
	{
#		include <AL/al.h>
#		include <AL/alc.h>
	}

	using details::ALboolean;
	using details::ALbyte;
	using details::ALchar;
	using details::ALdouble;
	using details::ALenum;
	using details::ALfloat;
	using details::ALint;
	using details::ALshort;
	using details::ALsizei;
	using details::ALubyte;
	using details::ALuint;
	using details::ALushort;
	using details::ALvoid;

	using details::ALCboolean;
	using details::ALCbyte;
	using details::ALCchar;
	using details::ALCcontext;
	using details::ALCdevice;
	using details::ALCdouble;
	using details::ALCenum;
	using details::ALCfloat;
	using details::ALCint;
	using details::ALCshort;
	using details::ALCsizei;
	using details::ALCubyte;
	using details::ALCuint;
	using details::ALCushort;
	using details::ALCvoid;

	using OpenALFunc = void( * )();

	//*********************************************************************************************

	struct Context
	{
		// al
		details::LPALBUFFER3F alBuffer3f;
		details::LPALBUFFER3I alBuffer3i;
		details::LPALBUFFERDATA alBufferData;
		details::LPALBUFFERF alBufferf;
		details::LPALBUFFERFV alBufferfv;
		details::LPALBUFFERI alBufferi;
		details::LPALBUFFERIV alBufferiv;
		details::LPALDELETEBUFFERS alDeleteBuffers;
		details::LPALDELETESOURCES alDeleteSources;
		details::LPALDISABLE alDisable;
		details::LPALDISTANCEMODEL alDistanceModel;
		details::LPALDOPPLERFACTOR alDopplerFactor;
		details::LPALDOPPLERVELOCITY alDopplerVelocity;
		details::LPALENABLE alEnable;
		details::LPALGENBUFFERS alGenBuffers;
		details::LPALGENSOURCES alGenSources;
		details::LPALGETBOOLEAN alGetBoolean;
		details::LPALGETBOOLEANV alGetBooleanv;
		details::LPALGETBUFFER3F alGetBuffer3f;
		details::LPALGETBUFFER3I alGetBuffer3i;
		details::LPALGETBUFFERF alGetBufferf;
		details::LPALGETBUFFERFV alGetBufferfv;
		details::LPALGETBUFFERI alGetBufferi;
		details::LPALGETBUFFERIV alGetBufferiv;
		details::LPALGETDOUBLE alGetDouble;
		details::LPALGETDOUBLEV alGetDoublev;
		details::LPALGETENUMVALUE alGetEnumValue;
		details::LPALGETERROR alGetError;
		details::LPALGETFLOAT alGetFloat;
		details::LPALGETFLOATV alGetFloatv;
		details::LPALGETINTEGER alGetInteger;
		details::LPALGETINTEGERV alGetIntegerv;
		details::LPALGETLISTENER3F alGetListener3f;
		details::LPALGETLISTENER3I alGetListener3i;
		details::LPALGETLISTENERF alGetListenerf;
		details::LPALGETLISTENERFV alGetListenerfv;
		details::LPALGETLISTENERI alGetListeneri;
		details::LPALGETLISTENERIV alGetListeneriv;
		details::LPALGETPROCADDRESS alGetProcAddress;
		details::LPALGETSOURCE3F alGetSource3f;
		details::LPALGETSOURCE3I alGetSource3i;
		details::LPALGETSOURCEF alGetSourcef;
		details::LPALGETSOURCEFV alGetSourcefv;
		details::LPALGETSOURCEI alGetSourcei;
		details::LPALGETSOURCEIV alGetSourceiv;
		details::LPALGETSTRING alGetString;
		details::LPALISBUFFER alIsBuffer;
		details::LPALISENABLED alIsEnabled;
		details::LPALISEXTENSIONPRESENT alIsExtensionPresent;
		details::LPALISSOURCE alIsSource;
		details::LPALLISTENER3F alListener3f;
		details::LPALLISTENER3I alListener3i;
		details::LPALLISTENERF alListenerf;
		details::LPALLISTENERFV alListenerfv;
		details::LPALLISTENERI alListeneri;
		details::LPALLISTENERIV alListeneriv;
		details::LPALSOURCE3F alSource3f;
		details::LPALSOURCE3I alSource3i;
		details::LPALSOURCEF alSourcef;
		details::LPALSOURCEFV alSourcefv;
		details::LPALSOURCEI alSourcei;
		details::LPALSOURCEIV alSourceiv;
		details::LPALSOURCEPAUSE alSourcePause;
		details::LPALSOURCEPAUSEV alSourcePausev;
		details::LPALSOURCEPLAY alSourcePlay;
		details::LPALSOURCEPLAYV alSourcePlayv;
		details::LPALSOURCEQUEUEBUFFERS alSourceQueueBuffers;
		details::LPALSOURCEREWIND alSourceRewind;
		details::LPALSOURCEREWINDV alSourceRewindv;
		details::LPALSOURCESTOP alSourceStop;
		details::LPALSOURCESTOPV alSourceStopv;
		details::LPALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;
		details::LPALSPEEDOFSOUND alSpeedOfSound;

		// alc
		details::LPALCCAPTURECLOSEDEVICE alcCaptureCloseDevice;
		details::LPALCCAPTUREOPENDEVICE alcCaptureOpenDevice;
		details::LPALCCAPTURESAMPLES alcCaptureSamples;
		details::LPALCCAPTURESTART alcCaptureStart;
		details::LPALCCAPTURESTOP alcCaptureStop;
		details::LPALCCLOSEDEVICE alcCloseDevice;
		details::LPALCCREATECONTEXT alcCreateContext;
		details::LPALCDESTROYCONTEXT alcDestroyContext;
		details::LPALCGETCONTEXTSDEVICE alcGetContextsDevice;
		details::LPALCGETCURRENTCONTEXT alcGetCurrentContext;
		details::LPALCGETENUMVALUE alcGetEnumValue;
		details::LPALCGETERROR alcGetError;
		details::LPALCGETINTEGERV alcGetIntegerv;
		details::LPALCGETPROCADDRESS alcGetProcAddress;
		details::LPALCGETSTRING alcGetString;
		details::LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
		details::LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
		details::LPALCOPENDEVICE alcOpenDevice;
		details::LPALCPROCESSCONTEXT alcProcessContext;
		details::LPALCSUSPENDCONTEXT alcSuspendContext;
	};

	//*********************************************************************************************

	struct SoundSystem;
	struct Sound;
	struct SoundChannel;

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

		void RemoveChannel( SoundChannel const & toRemove );

		Context context{};
		ALCdevice * alDevice{};
		ALCcontext * alContext{};
		DynamicLibrary library;
		std::vector< ALuint > musicGroup;
		std::vector< ALuint > sfxGroup;
		std::vector< SoundChannel const * > channels;
		Vector3 lastListenerPos{};

	private:
		void _cleanup();
		OpenALFunc _getEntry( const String & entryPoint, bool throwException = true );
		bool _loadFunctions();
	};

	//*********************************************************************************************

	struct Sound
		: public api::Sound
	{
	public:
		Sound( Context & context
			, Url const & url
			, bool global );
		~Sound();

		bool GetLength( uint32_t & length )override;

		Context & context;
		ALuint buffer{ ~ALuint( 0 ) };
		uint32_t duration;
	};

	//*********************************************************************************************

	struct SoundChannel
		: public api::SoundChannel
	{
		SoundChannel( SoundSystem & system
			, SoundObject const & object );
		~SoundChannel();

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

		SoundSystem & system;
		Context & context;
		ALuint buffer{ ~ALuint( 0 ) };
		ALuint source{ ~ALuint( 0 ) };
		Vector3 position{};
		Real gain{ 1.0f };
		bool startedPlaying{};
	};

	//*********************************************************************************************
}

#endif
#endif
