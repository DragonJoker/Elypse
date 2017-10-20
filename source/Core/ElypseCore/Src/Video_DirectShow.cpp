/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Video_DirectShow.h"

#if ELYPSE_WINDOWS

#include "VideoObject.h"
#include "VideoTexture.h"

#include <FastMath.h>
#include <thread>
#include <Utils.h>

#include "ElypseLogs.h"

VideoImpl_DirectShow::VideoImpl_DirectShow( VideoObject & p_owner )
	: VideoImplementation{ p_owner }
{
}

VideoImpl_DirectShow::~VideoImpl_DirectShow()
{
	DoCleanup();
}

void VideoImpl_DirectShow::Reset()
{
}

void VideoImpl_DirectShow::DoCleanup()
{
	auto l_lock = make_unique_lock( m_mutex );

	if ( m_thread )
	{
		m_thread->join();
	}

	delete m_thread;
	m_thread = nullptr;

	m_buffer1.clear();
	m_buffer2.clear();
}

bool VideoImpl_DirectShow::Start()
{
	{
		auto l_lock = make_unique_lock( m_mutex );

		if ( ! m_ready )
		{
			m_status = EM_STATUS_PLAY;
			return true;
		}
	}

	//if ( m_mediaControl == NULL || FAILED( m_mediaControl->Run() ) )
	{
		return false;
	}

	m_first = true;
	//m_mediaSeeking->SetTimeFormat( & TIME_FORMAT_MEDIA_TIME );
	//__int64 l_duration;
	//m_mediaSeeking->GetDuration( & l_duration );
	//m_mediaTime = Real( l_duration * 1.0 / 1e7 );

	if ( m_infiniteStream )
	{
		m_mediaTime = std::numeric_limits <Real> ::max();
	}

	m_streamEnded = false;
	return true;
}


bool VideoImpl_DirectShow::Pause()
{
	{
		auto l_lock = make_unique_lock( m_mutex );

		if ( ! m_ready /*|| m_mediaControl == NULL*/ )
		{
			m_status = EM_STATUS_PAUSE;
			return false;
		}
	}
	//m_mediaControl->Pause();
	return true;
}

bool VideoImpl_DirectShow::Stop()
{
	{
		auto l_lock = make_unique_lock( m_mutex );

		if ( ! m_ready /*|| m_mediaControl == NULL*/ )
		{
			m_status = EM_STATUS_STOP;
			return false;
		}
	}
	//m_mediaControl->Stop();
	//__int64 l_position = 0;
	//m_mediaSeeking->SetPositions( & l_position, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, NULL, AM_SEEKING_NoPositioning );
	return true;
}

void VideoImpl_DirectShow::Initialise( Url const & p_url )
{
	m_url = p_url;
	m_format = DoGetVideoType( p_url );

	if ( m_infiniteStream )
	{
		m_format = EM_DS_VIDEO_WMV;
	}

	m_thread = new std::thread
	{
		[this, p_url]()
		{
			DoInitialise( p_url );
		}
	};
}

void VideoImpl_DirectShow::SetMaxVolume( int p_maxVolume )
{
	m_maxVolume = ( p_maxVolume * 100 ) - 10000;
	m_currentVolume = int( ( m_maxVolume + 10000 ) * m_volumePercent ) - 10000;

	if ( m_ready )
	{
		//m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
	}
}

void VideoImpl_DirectShow::SetVolumePercent( Real p_percent )
{
	m_volumePercent = p_percent;
	m_currentVolume = int( ( m_maxVolume + 10000 ) * m_volumePercent ) - 10000;

	if ( m_ready )
	{
		//m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
	}
}

void VideoImpl_DirectShow::Mute( bool p_muted )
{
	m_muted = p_muted;

	if ( m_ready )
	{
		if ( m_muted )
		{
			//m_audio->put_Volume( -10000 );
		}
		else
		{
			//m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
		}
	}
}

bool VideoImpl_DirectShow::Update( Real p_time )
{
	{
		auto l_lock = make_unique_lock( m_mutex );

		if ( ! m_ready )
		{
			return false;
		}
	}

	if ( m_first )
	{
		m_playTime = -p_time;
		m_first = false;
	}

	m_playTime += p_time;

	if ( m_playTime >= m_mediaTime )
	{
		if ( GetOwner()->IsLooped() )
		{
			//unsigned long l_flags = AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame;
			//__int64 l_position = 0;
			//m_mediaSeeking->SetPositions( & l_position, l_flags, NULL, AM_SEEKING_NoPositioning );
			m_playTime = 0.0;
		}
		else
		{
			GetOwner()->Stop();
		}

		return false;
	}

	//long l_eventCode;
	//LONG_PTR l_param1, l_param2;

	//while ( SUCCEEDED( m_mediaEvent->GetEvent( &l_eventCode, &l_param1, &l_param2, 0 ) ) && ! m_streamEnded )
	//{
	//	m_streamEnded = ( l_eventCode == EC_COMPLETE || l_eventCode == 64 );
	//	m_mediaEvent->FreeEventParams( l_eventCode, l_param1, l_param2 );
	//}

	if ( m_buffer1.empty() )
	{
		m_buffer1.resize( m_vidWidth * m_vidHeight * 4 );
		m_buffer2.resize( m_vidWidth * m_vidHeight * 4 );
	}

	return DoGrabFrame();
}

void VideoImpl_DirectShow::DoInitialise( String const & p_url )
{
	try
	{
		m_capture.open( p_url );

		if ( m_capture.isOpened() )
		{
			m_capture.set( cv::CAP_PROP_FRAME_WIDTH, 2000 );
			m_capture.set( cv::CAP_PROP_FRAME_HEIGHT, 2000 );
			auto l_width = int( m_capture.get( cv::CAP_PROP_FRAME_WIDTH ) );
			auto l_height = int( m_capture.get( cv::CAP_PROP_FRAME_HEIGHT ) );
			//WCHAR l_wUrl[MAX_PATH];
			//MultiByteToWideChar( CP_ACP, 0, m_url.c_str(), -1, l_wUrl, MAX_PATH );
			//IBaseFilter * l_source;

			//if ( FAILED( m_filterGraph->AddSourceFilter( l_wUrl, L"Video Source", &l_source ) ) )
			//{
			//	return;
			//}

			//IBaseFilter * l_splitter;
			//IBaseFilter * l_grabber;
			//IBaseFilter * l_audio;
			//IBaseFilter * l_null;
			//l_grabber = _createSampleGrabber();
			//l_audio = _createFilter( CLSID_DSoundRender, L"Audio Renderer" );
			//l_null = _createFilter( CLSID_NullRenderer, L"Null Renderer" );

			//if ( m_format == EM_DS_VIDEO_FLV )
			//{
			//	l_splitter = _findSplitter( L"FLV Splitter" );
			//	_connectPins( l_source, 0, l_audio, 0 );
			//	_connectPins( l_splitter, 1, l_grabber, 0 );
			//}
			//else if ( m_format == EM_DS_VIDEO_REAL )
			//{
			//	l_splitter = _findSplitter( L"RealMedia Splitter" );
			//	_connectPins( l_source, 0, l_audio, 0 );
			//	_connectPins( l_splitter, 1, l_grabber, 0 );
			//}
			//else if ( m_format == EM_DS_VIDEO_WMV )
			//{
			//	_connectPins( l_source, 0, l_audio, 0 );
			//	_connectPins( l_source, 1, l_grabber, 0 );
			//}
			//else
			//{
			//	if ( m_format == EM_DS_VIDEO_AVI )
			//	{
			//		l_splitter = _createFilter( CLSID_AviSplitter, L"AVI Splitter" );
			//	}
			//	else if ( m_format == EM_DS_VIDEO_MPEG1 )
			//	{
			//		l_splitter = _createFilter( CLSID_MPEG1Splitter, L"MPEG 1Splitter" );
			//	}

			//	genlib_assert( _connectPins( l_source, 0, l_splitter, 0 ) );
			//	_connectPins( l_splitter, 1, l_audio, 0 );
			//	genlib_assert( _connectPins( l_splitter, 0, l_grabber, 0 ) );
			//}

			//_connectPins( l_grabber, 0, l_null, 0 );
			//_getDimensions();

			if ( m_muted )
			{
				//m_audio->put_Volume( -10000 );
			}
			else
			{
				//m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
			}

			//TODO : video de merde
			{
				//auto l_lock = make_unique_lock( m_mutex );
				GetOwner()->InitialiseVideoTexture();
				m_ready = true;
				Start();
			}

			while ( !Update( 0.0 ) )
			{
				std::this_thread::sleep_for( std::chrono::milliseconds{ 5 } );
			}

			{
				Start();
				auto l_lock = make_unique_lock( m_mutex );
				m_ready = true;
				Stop();

				if ( m_status == EM_STATUS_PLAY )
				{
					Start();
				}
				else if ( m_status == EM_STATUS_PAUSE )
				{
					Start();
					Pause();
				}
				else if ( m_status == EM_STATUS_STOP )
				{
					Stop();
				}
			}
		}
	}
	catch ( GenException & p_e )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Unexpected exception throw while initialising video named " + GetOwner()->GetName() + " / text : " + p_e.GetDescription() );
	}
}


bool VideoImpl_DirectShow::DoGrabFrame()
{
	//HRESULT hr = m_sampleGrabber->GetCurrentBuffer( & m_bufferLength, ( long * )m_buffer );

	//if ( hr == S_OK )
	//{
	//	unsigned char * l_targetLine = m_buffer2;
	//	unsigned char * l_sourceLine = m_buffer;

	//	if ( m_flip )
	//	{
	//		l_sourceLine = m_buffer + m_vidPitch * ( m_vidHeight - 1 );
	//	}

	//	for ( int i = 0; i < m_texHeight ; i ++ )
	//	{
	//		if ( ! m_flip )
	//		{
	//			l_sourceLine = m_buffer + ( ( int )( i * m_verticScale ) ) * m_vidPitch;
	//		}
	//		else
	//		{
	//			l_sourceLine = m_buffer
	//							+ m_vidPitch * ( m_vidHeight - 1 )
	//							- ( ( int )( i * m_verticScale ) ) * m_vidPitch;
	//		}

	//		l_targetLine = m_buffer2 + i * m_texPitch;

	//		for ( int j = 0 ; j < m_texWidth ; j++ )
	//		{
	//			l_targetLine[j * 4    ] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 0];
	//			l_targetLine[j * 4 + 1] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 1];
	//			l_targetLine[j * 4 + 2] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 2];
	//			l_targetLine[j * 4 + 3] = 0xFF;
	//		}
	//	}

	//	GetOwner()->GetVideoTexture()->CopyToBuffer( m_buffer2, static_cast <uint32_t>( m_texWidth * m_texHeight * 4 ) );
	//	GetOwner()->GetVideoTexture()->Update();
	//	return true;
	//}

	//if ( hr == E_INVALIDARG )
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - Samples are not being buffered. Call SetBufferSamples." );
	//}
	//else if ( hr == E_OUTOFMEMORY )
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The specified buffer is not large enough. - " + StringConverter::toString( m_bufferLength ) );
	//}
	//else if ( hr == E_POINTER )
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - NULL pointer argument." );
	//}
	//else if ( hr == VFW_E_NOT_CONNECTED )
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The filter is not connected." );
	//}
	//else if ( hr == VFW_E_WRONG_STATE )
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The filter has not received any samples yet." );
	//}
	//else
	//{
	//	EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - Unknown error : " + StringConverter::toString( hr ) + "." );
	//}

	return false;
}

DShowVideoFormat VideoImpl_DirectShow::DoGetVideoType( String const & p_videoUrl )
{
	String l_extension = p_videoUrl.substr( p_videoUrl.find_last_of( '.' ) + 1 );

	if ( l_extension == "flv" )
	{
		return EM_DS_VIDEO_FLV;
	}
	else if ( l_extension == "wmv" || l_extension == "asf" )
	{
		return EM_DS_VIDEO_WMV;
	}
	else if ( l_extension == "rm" || l_extension == "ram" )
	{
		return EM_DS_VIDEO_REAL;
	}
	else if ( l_extension == "mpg" || l_extension == "mpeg" )
	{
		return EM_DS_VIDEO_MPEG1;
	}
	else if ( l_extension == "avi" )
	{
		return EM_DS_VIDEO_AVI;
	}
	else
	{
		m_infiniteStream = true;
		return EM_DS_VIDEO_WMV;
	}

	//return EM_DS_VIDEO_NONE;
}

#endif //ELYPSE_WINDOWS
