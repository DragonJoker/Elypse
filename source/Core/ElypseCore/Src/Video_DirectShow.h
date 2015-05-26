#ifndef ___VIDEO_DIRECTSHOW_H___
#define ___VIDEO_DIRECTSHOW_H___

#include "VideoImplementation.h"

#if ELYPSE_WINDOWS

struct IMediaControl;
struct IBasicAudio;
struct IMediaSeeking;
struct IMediaEvent;
struct IMediaEventEx;
struct ISampleGrabber;
struct IFilterGraph2;
struct IBaseFilter;
struct IPin;

#include <Thread.h>

namespace EMuse
{
	namespace Media
	{
		enum DShowVideoFormat
		{
			EM_DS_VIDEO_NONE	= 0,
			EM_DS_VIDEO_FLV		= 1,
			EM_DS_VIDEO_REAL	= 2,
			EM_DS_VIDEO_WMV		= 3,
			EM_DS_VIDEO_AVI		= 4,
			EM_DS_VIDEO_MPEG1	= 5
		};


		class d_dll_export VideoImpl_DirectShow : public VideoImplementation
		{
		private:
			IMediaControl * m_mediaControl;
			IBasicAudio * m_audio;
			IMediaSeeking * m_mediaSeeking;
			IMediaEvent * m_mediaEvent;
			IMediaEventEx * m_mediaEventEx;
			ISampleGrabber * m_sampleGrabber;
			IFilterGraph2 * m_filterGraph;

			DShowVideoFormat m_format;

			bool m_infiniteStream;
			bool m_streamEnded;
			bool m_flip;

			bool m_first;

			int m_maxVolume;		// de -10000 à 0
			int m_currentVolume;	// de -10000 à 0
			Real m_volumePercent;	// de 0.0 à 1.0
			bool m_muted;

			int m_vidWidth;
			int m_vidHeight;
			int m_vidPitch;

			int m_texWidth;
			int m_texHeight;
			int m_texPitch;

			long m_bufferLength;

			unsigned char * m_buffer;
			unsigned char * m_buffer2;

			Real m_verticScale;
			Real m_horizScale;

			Real m_mediaTime;
			Real m_playTime;

			Url m_url;
			RecursiveMutex m_mutex;
			bool m_ready;

			MediaStatus m_status;

			Thread * m_thread;

		private:
			void _cleanup();

		public:
			VideoImpl_DirectShow( VideoObject * p_owner );
			virtual ~VideoImpl_DirectShow();

		public:
			virtual bool Start();
			virtual bool Stop();
			virtual bool Pause();
			virtual void Reset();
			virtual void Initialise( const Url & p_mediaUrl );
			virtual bool Update( Real p_time );
			virtual void SetMaxVolume( int p_maxVolume );	// 0 < p_maxVolume < 100
			virtual void SetVolumePercent( Real p_percent );
			virtual void Mute( bool p_muted );

		private:
			bool _initialiseInstances( const String & p_url );
			void _initialise();
			bool _grabFrame();

			IBaseFilter * _createSampleGrabber();
			IBaseFilter * _findSplitter( const wchar_t * p_name );
			IBaseFilter * _createFilter( const IID & p_id, const wchar_t * p_name );

			bool _getDimensions();

			DShowVideoFormat _getVideoType( const String & p_mediaName );

		public:
			bool _connectPins( IBaseFilter * p_outputFilter,	unsigned int p_outputNum,
							   IBaseFilter * p_inputFilter,	unsigned int p_inputNum );
			void _findPin( IBaseFilter * p_baseFilter, int p_direction, int p_pinNumber, IPin ** p_destPin );
		};
	}
}

#endif // ELYPSE_WINDOWS

#endif
