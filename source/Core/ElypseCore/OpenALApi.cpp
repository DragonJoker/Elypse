/*
See LICENSE file in root folder
*/
#if ELYPSE_USE_OPENAL
#include "PrecompiledHeader.h"

#include "OpenALApi.h"

#include "SoundManager.h"
#include "SoundObject.h"
#include "SoundPlaylist.h"
#include "SoundInstance.h"

#include "ElypseLogs.h"

#include <StringUtils.h>

#include <OgreEntity.h>
#include <OgreLog.h>
#include <StringConverter.h>

#include <array>
#include <iomanip>
#include <string_view>

#include <sndfile.h>
#include <mpg123.h>

#ifndef _MSC_VER
#	include <unistd.h>
#else
#	include <io.h>
#endif

#define CHECKSNDERROR( x ) (( x ), CheckError( context, __LINE__, __FILE__))

namespace Elypse::Media::openal
{
	namespace
	{
		bool CheckError( Context & context
			, uint32_t line
			, char const * const file )
		{
			auto err = context.alGetError();

			if ( err == AL_NO_ERROR )
			{
				return true;
			}

			String msg = String( "OpenAL error (" ) + StringConverter::toString( err ) + ") : [Unknown yet] in \"" + file + "\"@" + StringConverter::toString( line );
			EMUSE_MESSAGE_RELEASE( msg );
			return false;
		}

		sf_count_t GetSize( void * user_data )
		{
			auto * stream = static_cast< FILE * >( user_data );
			return sf_count_t( ftell( stream ) );
		}

		sf_count_t Read( void * ptr, sf_count_t count, void * user_data )
		{
			auto * stream = static_cast< FILE * >( user_data );
			return sf_count_t( fread( ptr, 1u, size_t( count ), stream ) );
		}

		sf_count_t Seek( sf_count_t offset, int whence, void * user_data )
		{
			auto * stream = static_cast< FILE * >( user_data );
			return sf_count_t( fseek( stream, offset, whence ) );
		}

		sf_count_t Tell( void * user_data )
		{
			auto * stream = static_cast< FILE * >( user_data );
			return sf_count_t( ftell( stream ) );
		}

		struct StreamInfo
		{
			ALuint format;
			uint32_t sampleRate;
			uint64_t sampleCount;
			uint32_t duration;
			std::vector< int16_t > samples;
		};

		String getFormatName( ALenum format )
		{
			switch ( format )
			{
			case AL_FORMAT_MONO8:
				return "mono_8";
			case AL_FORMAT_MONO16:
				return "mono_16";
			case AL_FORMAT_STEREO8:
				return "stereo_8";
			case AL_FORMAT_STEREO16:
				return "stereo_16";
			default:
				return "Unsupported";
			}
		}

		std::ostream & operator<<( std::ostream & stream, StreamInfo const & v )
		{
			stream << "Fmt:" << getFormatName( v.format )
				<< ", Rate:" << v.sampleRate
				<< ", Count:" << v.sampleCount
				<< ", Duration:" << v.duration << " ms";
			return stream;
		}

		namespace mp3
		{
			StreamInfo load( FILE * file )
			{
				static size_t constexpr INBUFF = 16384u;
				static size_t constexpr OUTBUFF = 32768u;

				StreamInfo result{};
				int ret{};
				auto handle = mpg123_new( nullptr, &ret );

				if ( !handle )
				{
					EMUSE_LOG_MESSAGE_RELEASE( String( "Unable to create mpg123 handle: " ) + mpg123_plain_strerror( ret ) );
					return result;
				}

				mpg123_param( handle, MPG123_VERBOSE, 2, 0 );
				mpg123_open_feed( handle );

				std::array< uint8_t, INBUFF > buf;
				std::array< uint8_t, OUTBUFF > out;
				size_t in = 0;
				size_t offset = 0;
				int channels = 1;
				int encoding = MPG123_ENC_16;

				try
				{
					while ( 1 ) /* Read and write until everything is through. */
					{
						auto len = fread( buf.data(), 1u, INBUFF, file );

						if ( len <= 0 )
						{
							break;
						}

						in += len;
						/* Feed input chunk and get first chunk of decoded audio. */
						size_t size;
						ret = mpg123_decode( handle, buf.data(), len, out.data(), OUTBUFF, &size );

						if ( ret == MPG123_NEW_FORMAT )
						{
							long sampleRate;
							mpg123_getformat( handle, &sampleRate, &channels, &encoding );
							result.sampleRate = sampleRate;

							if ( !( encoding & MPG123_ENC_8 ) && !( encoding & MPG123_ENC_16 ) )
							{
								throw std::runtime_error( String( "Unsupported bit-depth" ) );
							}

							switch ( channels )
							{
							case 1:
								result.format = ( encoding & MPG123_ENC_8 ) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
								break;
							case 2:
								result.format = ( encoding & MPG123_ENC_8 ) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
								break;
							default:
								throw std::runtime_error( String( "Channel count not handled" ) );
							}

							result.sampleCount = mpg123_length( handle );

							if ( result.sampleCount )
							{
								result.samples.resize( result.sampleCount * channels );
							}
						}

						if ( !result.sampleCount )
						{
							result.samples.resize( result.samples.size() + ( size + 1 ) / 2 );
						}

						std::memcpy( reinterpret_cast< uint8_t * >( result.samples.data() ) + offset, out.data(), size );
						offset += size;

						while ( ret != MPG123_ERR && ret != MPG123_NEED_MORE )
						{ /* Get all decoded audio that is available now before feeding more input. */
							ret = mpg123_decode( handle, NULL, 0, out.data(), OUTBUFF, &size );

							if ( !result.sampleCount )
							{
								result.samples.resize( result.samples.size() + ( size + 1 ) / 2 );
							}

							std::memcpy( reinterpret_cast< uint8_t * >( result.samples.data() ) + offset, out.data(), size );
							offset += size;
						}

						if ( ret == MPG123_ERR )
						{
							throw std::runtime_error( String( "Some mpg123 error: " ) + mpg123_strerror( handle ) );
						}
					}

					result.sampleCount = result.samples.size() / channels;
					result.duration = uint32_t( ( 1000ULL * result.sampleCount ) / ( ( ( encoding & MPG123_ENC_8 ) ? 1u : 2u ) * result.sampleRate ) );
				}
				catch ( std::exception & exc )
				{
					EMUSE_LOG_MESSAGE_RELEASE( exc.what() );
				}

				mpg123_delete( handle );
				return result;
			}
		}

		namespace sndfile
		{
			static SF_VIRTUAL_IO callbacks = { GetSize, Seek, Read, nullptr, Tell };

			StreamInfo load( FILE * file )
			{
				StreamInfo result{};
				SF_INFO infos{};
				infos.format = 0; // Unknown format
				auto handle = sf_open_virtual( &callbacks, SFM_READ, &infos, file );

				if ( !handle )
				{
					EMUSE_LOG_MESSAGE_RELEASE( String( "Failed to open sound: " ) + sf_strerror( handle ) );
					return result;
				}

				switch ( infos.channels )
				{
				case 1:
					result.format = AL_FORMAT_MONO16;
					break;
				case 2:
					result.format = AL_FORMAT_STEREO16;
					break;
				default:
					EMUSE_LOG_MESSAGE_RELEASE( String( "Channel count not handled: " ) + sf_strerror( handle ) );
					sf_close( handle );
					return result;
				}

				result.sampleCount = infos.channels * infos.frames;
				result.sampleRate = infos.samplerate;

				// sound duration (s) = samples / channels * rate
				result.duration = uint32_t( ( 1000ULL * result.sampleCount ) / ( infos.format * result.sampleRate ) );

				if ( infos.format & SF_FORMAT_VORBIS )
				{
					// To handle non normalised floats with some files.
					sf_command( handle, SFC_SET_SCALE_FLOAT_INT_READ, nullptr, SF_TRUE );
				}

				result.samples.resize( result.sampleCount );

				if ( sf_read_short( handle, result.samples.data(), result.sampleCount ) != result.sampleCount )
				{
					EMUSE_LOG_MESSAGE_RELEASE( "Failed to read samples" );
				}

				sf_close( handle );
				return result;
			}
		}
	}

	//*********************************************************************************************

	SoundSystem::SoundSystem()
	{
#if defined( _WIN32 )
		std::array< std::string_view, 3u > constexpr libs
		{
			"soft_oal.dll",
			"wrap_oal.dll",
			"openal32.dll"
		};
#elif defined( __linux__ )
		std::array< std::string_view, 3u > constexpr libs
		{
			"libopenal.so.1",
			"libopenal.so.0",
			"libopenal.so"
		};
#endif

		bool succeeded = libs.end() != std::find_if( libs.begin()
			, libs.end()
			, [this]( auto & path )
			{
				String libPath( path );
				return library.Open( libPath ) && _loadFunctions();
			} );

		if ( !succeeded )
		{
			_cleanup();
			return;
		}

		try
		{
			alDevice = context.alcOpenDevice( NULL );

			if ( !alDevice )
			{
				throw std::runtime_error( "Couldn't open OpenAL device" );
			}

			alContext = context.alcCreateContext( alDevice, NULL );

			if ( !alContext )
			{
				throw std::runtime_error( "Couldn't create OpenAL context" );
			}

			if ( !context.alcMakeContextCurrent( alContext ) )
			{
				throw std::runtime_error( "Couldn't activate OpenAL context" );
			}
		}
		catch ( std::exception & exc )
		{
			_cleanup();
			EMUSE_LOG_MESSAGE_RELEASE( "openal::SoundSystem: " + String( exc.what() ) );
			return;
		}

		Vector3 up{ 0.0f, 1.0f, 0.0f };
		Vector3 forward{ 0.0f, 0.0f, 1.0f };
		Vector3 velocity{};
		CHECKSNDERROR( context.alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f ) );
		CHECKSNDERROR( context.alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f ) );
		std::array< float, 6u > data
		{
			forward[0],
			forward[1],
			forward[2],
			up[0],
			up[1],
			up[2],
		};
		CHECKSNDERROR( context.alListenerfv( AL_ORIENTATION, data.data() ) );
		m_initialised = true;
		mpg123_init();
	}

	SoundSystem::~SoundSystem()
	{
		mpg123_exit();
		CHECKSNDERROR( context.alcMakeContextCurrent( nullptr ) );
		_cleanup();
	}

	SoundPtr SoundSystem::CreateStream( Url const & url, bool global )
	{
		return std::make_unique< Sound >( context, url, global );
	}

	void SoundSystem::RemoveChannel( SoundChannel const & toRemove )
	{
		{
			auto it = std::find( channels.begin(), channels.end(), &toRemove );

			if ( it != channels.end() )
			{
				channels.erase( it );
			}
		}
		{
			auto it = std::find( musicGroup.begin(), musicGroup.end(), toRemove.source );

			if ( it != musicGroup.end() )
			{
				musicGroup.erase( it );
			}
		}
		{
			auto it = std::find( sfxGroup.begin(), sfxGroup.end(), toRemove.source );

			if ( it != sfxGroup.end() )
			{
				sfxGroup.erase( it );
			}
		}
	}

	SoundChannelPtr SoundSystem::CreateChannel( SoundObject const & object )
	{
		return std::make_unique< SoundChannel >( *this, object );
	}

	bool SoundSystem::AddToMusic( api::SoundChannel const & toAdd )
	{
		musicGroup.push_back( static_cast< SoundChannel const & >( toAdd ).source );
		return true;
	}

	bool SoundSystem::AddToSFX( api::SoundChannel const & toAdd )
	{
		sfxGroup.push_back( static_cast< SoundChannel const & >( toAdd ).source );
		return true;
	}

	bool SoundSystem::Update3D( const Vector3 & position
		, const Real * up
		, const Real * front
		, Real time )
	{
		auto listener = position / 100;
		Vector3 velocity = ( listener - lastListenerPos ) / time;
		lastListenerPos = listener;
		context.alListener3f( AL_POSITION, listener[0], listener[1], listener[2] );
		context.alListener3f( AL_VELOCITY, velocity[0], velocity[1], velocity[2] );

		Vector3 fup{ up[0], up[1], up[2] };
		Vector3 fforward{ front[0], front[1], front[2] };
		fforward.normalise();
		fup.normalise();
		std::array< float, 6u > data
		{
			fforward[0],
			fforward[1],
			fforward[2],
			fup[0],
			fup[1],
			fup[2],
		};
		context.alListenerfv( AL_ORIENTATION, data.data() );
		return context.alGetError() != AL_NO_ERROR;
	}

	bool SoundSystem::Update()
	{
		return true;
	}

	void SoundSystem::_cleanup()
	{
		if ( alContext )
		{
			CHECKSNDERROR( context.alcDestroyContext( alContext ) );
		}

		if ( alDevice )
		{
			CHECKSNDERROR( context.alcCloseDevice( alDevice ) );
		}

		if ( library.IsOpen() )
		{
			library.Close();
		}
	}

	OpenALFunc SoundSystem::_getEntry( const String & name, bool throwException )
	{
		OpenALFunc result = reinterpret_cast< OpenALFunc >( library.GetFunction( name.c_str() ) );

		if ( !result && throwException )
		{
			std::ostringstream stream;
			stream << "Failed to load \"" << name << '"';
			throw std::runtime_error( stream.str() );
		}

		return result;
	}

	bool SoundSystem::_loadFunctions()
	{
		try
		{
			// al
			context.alBuffer3f = reinterpret_cast< details::LPALBUFFER3F >( _getEntry( "alBuffer3f" ) );
			context.alBuffer3i = reinterpret_cast< details::LPALBUFFER3I >( _getEntry( "alBuffer3i" ) );
			context.alBufferData = reinterpret_cast< details::LPALBUFFERDATA >( _getEntry( "alBufferData" ) );
			context.alBufferf = reinterpret_cast< details::LPALBUFFERF >( _getEntry( "alBufferf" ) );
			context.alBufferfv = reinterpret_cast< details::LPALBUFFERFV >( _getEntry( "alBufferfv" ) );
			context.alBufferi = reinterpret_cast< details::LPALBUFFERI >( _getEntry( "alBufferi" ) );
			context.alBufferiv = reinterpret_cast< details::LPALBUFFERIV >( _getEntry( "alBufferiv" ) );
			context.alDeleteBuffers = reinterpret_cast< details::LPALDELETEBUFFERS >( _getEntry( "alDeleteBuffers" ) );
			context.alDeleteSources = reinterpret_cast< details::LPALDELETESOURCES >( _getEntry( "alDeleteSources" ) );
			context.alDisable = reinterpret_cast< details::LPALDISABLE >( _getEntry( "alDisable" ) );
			context.alDistanceModel = reinterpret_cast< details::LPALDISTANCEMODEL >( _getEntry( "alDistanceModel" ) );
			context.alDopplerFactor = reinterpret_cast< details::LPALDOPPLERFACTOR >( _getEntry( "alDopplerFactor" ) );
			context.alDopplerVelocity = reinterpret_cast< details::LPALDOPPLERVELOCITY >( _getEntry( "alDopplerVelocity" ) );
			context.alEnable = reinterpret_cast< details::LPALENABLE >( _getEntry( "alEnable" ) );
			context.alGenBuffers = reinterpret_cast< details::LPALGENBUFFERS >( _getEntry( "alGenBuffers" ) );
			context.alGenSources = reinterpret_cast< details::LPALGENSOURCES >( _getEntry( "alGenSources" ) );
			context.alGetBoolean = reinterpret_cast< details::LPALGETBOOLEAN >( _getEntry( "alGetBoolean" ) );
			context.alGetBooleanv = reinterpret_cast< details::LPALGETBOOLEANV >( _getEntry( "alGetBooleanv" ) );
			context.alGetBuffer3f = reinterpret_cast< details::LPALGETBUFFER3F >( _getEntry( "alGetBuffer3f" ) );
			context.alGetBuffer3i = reinterpret_cast< details::LPALGETBUFFER3I >( _getEntry( "alGetBuffer3i" ) );
			context.alGetBufferf = reinterpret_cast< details::LPALGETBUFFERF >( _getEntry( "alGetBufferf" ) );
			context.alGetBufferfv = reinterpret_cast< details::LPALGETBUFFERFV >( _getEntry( "alGetBufferfv" ) );
			context.alGetBufferi = reinterpret_cast< details::LPALGETBUFFERI >( _getEntry( "alGetBufferi" ) );
			context.alGetBufferiv = reinterpret_cast< details::LPALGETBUFFERIV >( _getEntry( "alGetBufferiv" ) );
			context.alGetDouble = reinterpret_cast< details::LPALGETDOUBLE >( _getEntry( "alGetDouble" ) );
			context.alGetDoublev = reinterpret_cast< details::LPALGETDOUBLEV >( _getEntry( "alGetDoublev" ) );
			context.alGetEnumValue = reinterpret_cast< details::LPALGETENUMVALUE >( _getEntry( "alGetEnumValue" ) );
			context.alGetError = reinterpret_cast< details::LPALGETERROR >( _getEntry( "alGetError" ) );
			context.alGetFloat = reinterpret_cast< details::LPALGETFLOAT >( _getEntry( "alGetFloat" ) );
			context.alGetFloatv = reinterpret_cast< details::LPALGETFLOATV >( _getEntry( "alGetFloatv" ) );
			context.alGetInteger = reinterpret_cast< details::LPALGETINTEGER >( _getEntry( "alGetInteger" ) );
			context.alGetIntegerv = reinterpret_cast< details::LPALGETINTEGERV >( _getEntry( "alGetIntegerv" ) );
			context.alGetListener3f = reinterpret_cast< details::LPALGETLISTENER3F >( _getEntry( "alGetListener3f" ) );
			context.alGetListener3i = reinterpret_cast< details::LPALGETLISTENER3I >( _getEntry( "alGetListener3i" ) );
			context.alGetListenerf = reinterpret_cast< details::LPALGETLISTENERF >( _getEntry( "alGetListenerf" ) );
			context.alGetListenerfv = reinterpret_cast< details::LPALGETLISTENERFV >( _getEntry( "alGetListenerfv" ) );
			context.alGetListeneri = reinterpret_cast< details::LPALGETLISTENERI >( _getEntry( "alGetListeneri" ) );
			context.alGetListeneriv = reinterpret_cast< details::LPALGETLISTENERIV >( _getEntry( "alGetListeneriv" ) );
			context.alGetProcAddress = reinterpret_cast< details::LPALGETPROCADDRESS >( _getEntry( "alGetProcAddress" ) );
			context.alGetSource3f = reinterpret_cast< details::LPALGETSOURCE3F >( _getEntry( "alGetSource3f" ) );
			context.alGetSource3i = reinterpret_cast< details::LPALGETSOURCE3I >( _getEntry( "alGetSource3i" ) );
			context.alGetSourcef = reinterpret_cast< details::LPALGETSOURCEF >( _getEntry( "alGetSourcef" ) );
			context.alGetSourcefv = reinterpret_cast< details::LPALGETSOURCEFV >( _getEntry( "alGetSourcefv" ) );
			context.alGetSourcei = reinterpret_cast< details::LPALGETSOURCEI >( _getEntry( "alGetSourcei" ) );
			context.alGetSourceiv = reinterpret_cast< details::LPALGETSOURCEIV >( _getEntry( "alGetSourceiv" ) );
			context.alGetString = reinterpret_cast< details::LPALGETSTRING >( _getEntry( "alGetString" ) );
			context.alIsBuffer = reinterpret_cast< details::LPALISBUFFER >( _getEntry( "alIsBuffer" ) );
			context.alIsEnabled = reinterpret_cast< details::LPALISENABLED >( _getEntry( "alIsEnabled" ) );
			context.alIsExtensionPresent = reinterpret_cast< details::LPALISEXTENSIONPRESENT >( _getEntry( "alIsExtensionPresent" ) );
			context.alIsSource = reinterpret_cast< details::LPALISSOURCE >( _getEntry( "alIsSource" ) );
			context.alListener3f = reinterpret_cast< details::LPALLISTENER3F >( _getEntry( "alListener3f" ) );
			context.alListener3i = reinterpret_cast< details::LPALLISTENER3I >( _getEntry( "alListener3i" ) );
			context.alListenerf = reinterpret_cast< details::LPALLISTENERF >( _getEntry( "alListenerf" ) );
			context.alListenerfv = reinterpret_cast< details::LPALLISTENERFV >( _getEntry( "alListenerfv" ) );
			context.alListeneri = reinterpret_cast< details::LPALLISTENERI >( _getEntry( "alListeneri" ) );
			context.alListeneriv = reinterpret_cast< details::LPALLISTENERIV >( _getEntry( "alListeneriv" ) );
			context.alSource3f = reinterpret_cast< details::LPALSOURCE3F >( _getEntry( "alSource3f" ) );
			context.alSource3i = reinterpret_cast< details::LPALSOURCE3I >( _getEntry( "alSource3i" ) );
			context.alSourcef = reinterpret_cast< details::LPALSOURCEF >( _getEntry( "alSourcef" ) );
			context.alSourcefv = reinterpret_cast< details::LPALSOURCEFV >( _getEntry( "alSourcefv" ) );
			context.alSourcei = reinterpret_cast< details::LPALSOURCEI >( _getEntry( "alSourcei" ) );
			context.alSourceiv = reinterpret_cast< details::LPALSOURCEIV >( _getEntry( "alSourceiv" ) );
			context.alSourcePause = reinterpret_cast< details::LPALSOURCEPAUSE >( _getEntry( "alSourcePause" ) );
			context.alSourcePausev = reinterpret_cast< details::LPALSOURCEPAUSEV >( _getEntry( "alSourcePausev" ) );
			context.alSourcePlay = reinterpret_cast< details::LPALSOURCEPLAY >( _getEntry( "alSourcePlay" ) );
			context.alSourcePlayv = reinterpret_cast< details::LPALSOURCEPLAYV >( _getEntry( "alSourcePlayv" ) );
			context.alSourceQueueBuffers = reinterpret_cast< details::LPALSOURCEQUEUEBUFFERS >( _getEntry( "alSourceQueueBuffers" ) );
			context.alSourceRewind = reinterpret_cast< details::LPALSOURCEREWIND >( _getEntry( "alSourceRewind" ) );
			context.alSourceRewindv = reinterpret_cast< details::LPALSOURCEREWINDV >( _getEntry( "alSourceRewindv" ) );
			context.alSourceStop = reinterpret_cast< details::LPALSOURCESTOP >( _getEntry( "alSourceStop" ) );
			context.alSourceStopv = reinterpret_cast< details::LPALSOURCESTOPV >( _getEntry( "alSourceStopv" ) );
			context.alSourceUnqueueBuffers = reinterpret_cast< details::LPALSOURCEUNQUEUEBUFFERS >( _getEntry( "alSourceUnqueueBuffers" ) );
			context.alSpeedOfSound = reinterpret_cast< details::LPALSPEEDOFSOUND >( _getEntry( "alSpeedOfSound" ) );

			// alc
			context.alcCaptureCloseDevice = reinterpret_cast< details::LPALCCAPTURECLOSEDEVICE >( _getEntry( "alcCaptureCloseDevice" ) );
			context.alcCaptureOpenDevice = reinterpret_cast< details::LPALCCAPTUREOPENDEVICE >( _getEntry( "alcCaptureOpenDevice" ) );
			context.alcCaptureSamples = reinterpret_cast< details::LPALCCAPTURESAMPLES >( _getEntry( "alcCaptureSamples" ) );
			context.alcCaptureStart = reinterpret_cast< details::LPALCCAPTURESTART >( _getEntry( "alcCaptureStart" ) );
			context.alcCaptureStop = reinterpret_cast< details::LPALCCAPTURESTOP >( _getEntry( "alcCaptureStop" ) );
			context.alcCloseDevice = reinterpret_cast< details::LPALCCLOSEDEVICE >( _getEntry( "alcCloseDevice" ) );
			context.alcCreateContext = reinterpret_cast< details::LPALCCREATECONTEXT >( _getEntry( "alcCreateContext" ) );
			context.alcDestroyContext = reinterpret_cast< details::LPALCDESTROYCONTEXT >( _getEntry( "alcDestroyContext" ) );
			context.alcGetContextsDevice = reinterpret_cast< details::LPALCGETCONTEXTSDEVICE >( _getEntry( "alcGetContextsDevice" ) );
			context.alcGetCurrentContext = reinterpret_cast< details::LPALCGETCURRENTCONTEXT >( _getEntry( "alcGetCurrentContext" ) );
			context.alcGetEnumValue = reinterpret_cast< details::LPALCGETENUMVALUE >( _getEntry( "alcGetEnumValue" ) );
			context.alcGetError = reinterpret_cast< details::LPALCGETERROR >( _getEntry( "alcGetError" ) );
			context.alcGetIntegerv = reinterpret_cast< details::LPALCGETINTEGERV >( _getEntry( "alcGetIntegerv" ) );
			context.alcGetProcAddress = reinterpret_cast< details::LPALCGETPROCADDRESS >( _getEntry( "alcGetProcAddress" ) );
			context.alcGetString = reinterpret_cast< details::LPALCGETSTRING >( _getEntry( "alcGetString" ) );
			context.alcIsExtensionPresent = reinterpret_cast< details::LPALCISEXTENSIONPRESENT >( _getEntry( "alcIsExtensionPresent" ) );
			context.alcMakeContextCurrent = reinterpret_cast< details::LPALCMAKECONTEXTCURRENT >( _getEntry( "alcMakeContextCurrent" ) );
			context.alcOpenDevice = reinterpret_cast< details::LPALCOPENDEVICE >( _getEntry( "alcOpenDevice" ) );
			context.alcProcessContext = reinterpret_cast< details::LPALCPROCESSCONTEXT >( _getEntry( "alcProcessContext" ) );
			context.alcSuspendContext = reinterpret_cast< details::LPALCSUSPENDCONTEXT >( _getEntry( "alcSuspendContext" ) );

			return true;
		}
		catch ( std::exception & exc )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "openal::SoundSystem: " + String( exc.what() ) );
			return false;
		}
	}

	//*********************************************************************************************

	Sound::Sound( Context & context
		, Url const & url
		, bool global )
		: context{ context }
	{
		StreamInfo streamInfo;
		FILE * file = fopen( url.c_str(), "rb" );

		if ( !file )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "Sound file " + url + " - Failed to open" );
			return;
		}

		if ( General::Utils::string::endsWith( url.GetFilename(), ".mp3" ) )
		{
			streamInfo = mp3::load( file );
		}
		else
		{
			streamInfo = sndfile::load( file );
		}

		if ( !streamInfo.samples.empty() )
		{
			std::stringstream stream;
			stream << "Sound file " << url << " loaded" << ", " << streamInfo;
			EMUSE_LOG_MESSAGE_RELEASE( stream.str() );

			if ( !CHECKSNDERROR( context.alGenBuffers( 1, &buffer ) ) )
			{
				EMUSE_LOG_MESSAGE_RELEASE( "Sound file " + url + " - Error in alGenBuffers" );
				return;
			}

			if ( !CHECKSNDERROR( context.alBufferData( buffer
				, streamInfo.format
				, streamInfo.samples.data()
				, ALsizei( streamInfo.sampleCount * sizeof( int16_t ) )
				, ALsizei( streamInfo.sampleRate ) ) ) )
			{
				EMUSE_LOG_MESSAGE_RELEASE( "Sound file " + url + " - Error in alBufferData" );
				CHECKSNDERROR( context.alDeleteBuffers( 1, &buffer ) );
				buffer = ~ALuint( 0 );
			}
		}
		else
		{
			EMUSE_LOG_MESSAGE_RELEASE( "Sound file " + url + " - No samples" );
		}
	}

	Sound::~Sound()
	{
		if ( buffer != ~ALuint( 0 ) )
		{
			CHECKSNDERROR( context.alDeleteBuffers( 1u, &buffer ) );
		}
	}

	bool Sound::GetLength( uint32_t & length )
	{
		length = duration;
		return true;
	}

	//*********************************************************************************************

	SoundChannel::SoundChannel( SoundSystem & system
		, SoundObject const & object )
		: system{ system }
		, context{ system.context }
		, buffer{ static_cast< Sound & >( object.GetSoundApi() ).buffer }
	{
		if ( !CHECKSNDERROR( context.alGenSources( 1, &source ) ) )
		{
			EMUSE_LOG_MESSAGE_RELEASE( object.GetName() + " - Error in alGenSources" );
			return;
		}
		
		if ( !CHECKSNDERROR( context.alSourcei( source, AL_BUFFER, buffer ) ) )
		{
			EMUSE_LOG_MESSAGE_RELEASE( object.GetName() + " - Error in alSourcei" );
			CHECKSNDERROR( context.alDeleteSources( 1u, &source ) );
			source = ~ALuint( 0 );
		}
	}

	SoundChannel::~SoundChannel()
	{
		system.RemoveChannel( *this );

		if ( source != ~ALuint( 0 ) )
		{
			CHECKSNDERROR( context.alDeleteSources( 1u, &source ) );
		}
	}

	bool SoundChannel::Enable3D()
	{
		return CHECKSNDERROR( context.alSource3f( source, AL_VELOCITY, 0.0f, 0.0f, 0.0f ) );
	}

	bool SoundChannel::SetVelocity( const Vector3 & velocity )
	{
		return CHECKSNDERROR( context.alSource3f( source, AL_VELOCITY, velocity[0], velocity[1], velocity[2] ) );
	}

	void SoundChannel::SetPosition( const Vector3 & newPosition )
	{
		position = newPosition;
	}

	bool SoundChannel::Update3D( const Vector3 & newPosition, Real time )
	{
		auto velocity = ( newPosition - position ) / time;
		position = newPosition;
		context.alSource3f( source, AL_POSITION, position[0], position[1], position[2] );
		context.alSource3f( source, AL_VELOCITY, velocity[0], velocity[1], velocity[2] );
		return context.alGetError() != AL_NO_ERROR;
	}

	bool SoundChannel::GetTimePosition( uint32_t & position )const
	{
		float pos;
		auto result = CHECKSNDERROR( context.alGetSourcef( source, AL_SEC_OFFSET, &pos ) );
		position = pos / 1000;
		return result;
	}

	bool SoundChannel::SetTimePosition( uint32_t position )
	{
		return CHECKSNDERROR( context.alSourcef( source, AL_SEC_OFFSET, position / 1000.0f ) );
	}

	bool SoundChannel::Loop( bool loop )
	{
		return CHECKSNDERROR( context.alSourcei( source, AL_LOOPING, ( loop ? AL_TRUE : AL_FALSE ) ) );
	}

	bool SoundChannel::Mute( bool mute )
	{
		return CHECKSNDERROR( context.alSourcef( source, AL_GAIN, mute ? 0.0f : gain ) );
	}

	bool SoundChannel::SetVolume( Real volume )
	{
		gain = volume;
		return CHECKSNDERROR( context.alSourcef( source, AL_GAIN, gain ) );
	}

	bool SoundChannel::Pause( bool pause )
	{
		if ( !pause && !startedPlaying )
		{
			startedPlaying = CHECKSNDERROR( context.alSourcePlay( source ) );
			return startedPlaying;
		}

		return CHECKSNDERROR( context.alSourcePause( source ) );
	}

	bool SoundChannel::Restart()
	{
		startedPlaying = false;
		auto result = CHECKSNDERROR( context.alSourceRewind( source ) )
			&& CHECKSNDERROR( context.alSourcePlay( source ) );

		if ( result )
		{
			startedPlaying = true;
		}

		return result;
	}

	bool SoundChannel::Stop()
	{
		startedPlaying = false;
		return CHECKSNDERROR( context.alSourceStop( source ) );
	}

	//*********************************************************************************************
}
#endif
