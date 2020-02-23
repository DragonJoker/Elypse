/*
See LICENSE file in root folder
*/
#ifndef ___VIDEO_DIRECTSHOW_H___
#define ___VIDEO_DIRECTSHOW_H___

#include "VideoImplementation.h"

#if ELYPSE_WINDOWS

#include <thread>

#include <opencv2/opencv.hpp>

namespace Elypse
{
	namespace Media
	{
		enum DShowVideoFormat
		{
			EM_DS_VIDEO_NONE = 0,
			EM_DS_VIDEO_FLV = 1,
			EM_DS_VIDEO_REAL = 2,
			EM_DS_VIDEO_WMV = 3,
			EM_DS_VIDEO_AVI = 4,
			EM_DS_VIDEO_MPEG1 = 5
		};

		class d_dll_export VideoImpl_DirectShow
			: public VideoImplementation
		{
		public:
			VideoImpl_DirectShow( VideoObject & p_owner );
			virtual ~VideoImpl_DirectShow();

		public:
			virtual bool Start();
			virtual bool Stop();
			virtual bool Pause();
			virtual void Reset();
			virtual void Initialise( Url const & p_mediaUrl );
			virtual bool Update( Real p_time );
			virtual void SetMaxVolume( int p_maxVolume );	// 0 < p_maxVolume < 100
			virtual void SetVolumePercent( Real p_percent );
			virtual void Mute( bool p_muted );

		private:
			void DoCleanup();
			void DoInitialise( String const & p_url );
			bool DoGrabFrame();

			DShowVideoFormat DoGetVideoType( String const & p_mediaName );

		private:
			DShowVideoFormat m_format{ EM_DS_VIDEO_NONE };

			bool m_infiniteStream{ false };
			bool m_streamEnded{ false };
			bool m_flip{ false };

			bool m_first{ false };

			int m_maxVolume{ 0 };		// de -10000 � 0
			int m_currentVolume{ 0 };	// de -10000 � 0
			Real m_volumePercent{ 1.0 };
			bool m_muted{ false };

			uint32_t m_vidWidth{ 0u };
			uint32_t m_vidHeight{ 0u };
			int m_vidPitch{ 0 };

			uint32_t m_texWidth{ 0u };
			uint32_t m_texHeight{ 0u };
			int m_texPitch{ 0 };

			std::vector< uint8_t > m_buffer1;
			std::vector< uint8_t > m_buffer2;

			Real m_verticScale{ 0.0 };
			Real m_horizScale{ 0.0 };

			Real m_mediaTime{ 0.0 };
			Real m_playTime{ 0.0 };

			Url m_url;
			std::recursive_mutex m_mutex;
			bool m_ready{ false };

			MediaStatus m_status{ EM_STATUS_NONE };

			std::thread * m_thread{ nullptr };
			cv::VideoCapture m_capture;
		};
	}
}

#endif // ELYPSE_WINDOWS

#endif
