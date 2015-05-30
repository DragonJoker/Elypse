#include "PrecompiledHeader.h"

#include "Video_DirectShow.h"

#if ELYPSE_WINDOWS
#include <dshow.h>

#pragma include_alias( "dxtrans.h", "qedit.h" )
#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

#include <qedit.h>		//SampleGrabber

//EXTERN_C const CLSID CLSID_NullRenderer;
//EXTERN_C const CLSID CLSID_SampleGrabber;
//EXTERN_C const IID IID_ISampleGrabber;

#include "VideoObject.h"
#include "VideoTexture.h"

#include <FastMath.h>
#include <Thread.h>
#include <Utils.h>

#include "EMuseLogs.h"

template< class T > void SafeRelease( T *& p_pReleasable )
{
	if ( p_pReleasable )
	{
		p_pReleasable->Release();
		p_pReleasable = NULL;
	}
}

VideoImpl_DirectShow::VideoImpl_DirectShow( VideoObject * p_owner )
	:	VideoImplementation( p_owner	)
	,	m_mediaControl( NULL	)
	,	m_audio( NULL	)
	,	m_mediaSeeking( NULL	)
	,	m_mediaEvent( NULL	)
	,	m_mediaEventEx( NULL	)
	,	m_sampleGrabber( NULL	)
	,	m_filterGraph( NULL	)
	,	m_buffer( NULL	)
	,	m_bufferLength( 0	)
	,	m_buffer2( NULL	)
	,	m_vidWidth( 0	)
	,	m_vidHeight( 0	)
	,	m_ready( false	)
	,	m_infiniteStream( false	)
	,	m_status( EM_STATUS_NONE	)
	,	m_thread( NULL	)
	,	m_maxVolume( 0	)
	,	m_currentVolume( 0	)
	,	m_volumePercent( 1.0	)
	,	m_muted( false	)
{
//	CoInitialize(NULL);
}

VideoImpl_DirectShow::~VideoImpl_DirectShow()
{
	_cleanup();
}

void VideoImpl_DirectShow::Reset()
{
}

void VideoImpl_DirectShow::_cleanup()
{
	GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );

	if ( m_thread != NULL )
	{
		m_thread->join();
	}

	delete m_thread;
	m_thread = NULL;

	if ( m_mediaControl != NULL )
	{
		m_mediaControl->Stop();
	}

	SafeRelease( m_filterGraph	);
	SafeRelease( m_mediaControl	);
	SafeRelease( m_sampleGrabber	);
	SafeRelease( m_audio	);
	SafeRelease( m_mediaSeeking	);

	if ( m_buffer != NULL )
	{
		delete [] m_buffer;
		m_buffer = NULL;
	}

	if ( m_buffer2 != NULL )
	{
		delete [] m_buffer2;
		m_buffer2 = NULL;
	}
}

bool VideoImpl_DirectShow::_connectPins( IBaseFilter * p_outputFilter,	unsigned int p_outputNum,
		IBaseFilter * p_inputFilter,	unsigned int p_inputNum )
{
	genlib_assert( p_inputFilter != NULL );
	genlib_assert( p_outputFilter != NULL );
	IPin * l_inputPin;
	IPin * l_outputPin;
	_findPin( p_outputFilter, PINDIR_OUTPUT, p_outputNum, & l_outputPin );
	_findPin( p_inputFilter, PINDIR_INPUT, p_inputNum, & l_inputPin );

	if ( l_inputPin != NULL && l_outputPin != NULL )
	{
		return SUCCEEDED( m_filterGraph->Connect( l_outputPin, l_inputPin ) );
	}

	return false;
}

void VideoImpl_DirectShow::_findPin( IBaseFilter * p_baseFilter, int p_iDirection, int p_pinNumber, IPin ** p_destPin )
{
	PIN_DIRECTION p_direction = PIN_DIRECTION( p_iDirection );
	IEnumPins * pEnum = NULL;
	IPin    *   pPin = NULL;
	HRESULT    hr;
	int i = 0;
	hr = p_baseFilter->EnumPins( & pEnum );

	if ( FAILED( hr ) )
	{
		return;
		GENLIB_EXCEPTION( "Internal error while finding pin" );
	}

	while ( pEnum->Next( 1, &pPin, 0 ) == S_OK )
	{
		PIN_DIRECTION PinDirThis;
		hr = pPin->QueryDirection( &PinDirThis );

		if ( FAILED( hr ) )
		{
			pPin->Release();
			pEnum->Release();
			GENLIB_EXCEPTION( "Internal error while finding pin, #2" );
			return;
		}

		if ( PinDirThis == p_direction )
		{
			if ( i == p_pinNumber )
			{
				// Found a match. Return the IPin pointer to the caller.
				*p_destPin = pPin;
				pEnum->Release();
				return;
			}
			else
			{
				i ++;
			}
		}

		// Release the pin for the next time through the loop.
		pPin->Release();
	}

	// No more pins. We did not find a match.
	pEnum->Release();
//	GENLIB_EXCEPTION( "Internal error while finding pin, #3");
	return;
}

bool VideoImpl_DirectShow::Start()
{
	{
		GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );

		if ( ! m_ready )
		{
			m_status = EM_STATUS_PLAY;
			return true;
		}
	}

	if ( m_mediaControl == NULL || FAILED( m_mediaControl->Run() ) )
	{
		return false;
	}

	m_first = true;
	m_mediaSeeking->SetTimeFormat( & TIME_FORMAT_MEDIA_TIME );
	__int64 l_duration;
	m_mediaSeeking->GetDuration( & l_duration );
	m_mediaTime = Real( l_duration * 1.0 / 1e7 );

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
		GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );

		if ( ! m_ready || m_mediaControl == NULL )
		{
			m_status = EM_STATUS_PAUSE;
			return false;
		}
	}
	m_mediaControl->Pause();
	return true;
}

bool VideoImpl_DirectShow::Stop()
{
	{
		GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );

		if ( ! m_ready || m_mediaControl == NULL )
		{
			m_status = EM_STATUS_STOP;
			return false;
		}
	}
	m_mediaControl->Stop();
	__int64 l_position = 0;
	m_mediaSeeking->SetPositions( & l_position, AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame, NULL, AM_SEEKING_NoPositioning );
	return true;
}

IBaseFilter * VideoImpl_DirectShow::_createSampleGrabber()
{
	IBaseFilter * l_grabber;
	CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter,
					  reinterpret_cast <void **>( & l_grabber ) );
	l_grabber->QueryInterface( IID_ISampleGrabber, reinterpret_cast <void **>( & m_sampleGrabber ) );
	m_filterGraph->AddFilter( l_grabber, L"Sample Grabber" );
	AM_MEDIA_TYPE desiredType;
	memset( & desiredType, 0, sizeof( desiredType ) );
	desiredType.majortype = MEDIATYPE_Video;
	desiredType.subtype = MEDIASUBTYPE_RGB32;
	desiredType.formattype = FORMAT_VideoInfo;
	m_sampleGrabber->SetMediaType( & desiredType );
	m_sampleGrabber->SetBufferSamples( TRUE );
	return l_grabber;
}

IBaseFilter * VideoImpl_DirectShow::_findSplitter( const wchar_t * p_name )
{
	IBaseFilter * l_splitter;

	if ( FAILED( m_filterGraph->FindFilterByName( p_name, & l_splitter ) ) )
	{
		GENLIB_EXCEPTION( "Could not find splitter : " + ToString( p_name ) );
	}

	return l_splitter;
}

IBaseFilter * VideoImpl_DirectShow::_createFilter( const IID & p_id, const wchar_t * p_name )
{
	IBaseFilter * l_filter;
	CoCreateInstance( p_id, NULL, CLSCTX_INPROC, IID_IBaseFilter,
					  reinterpret_cast <void **>( & l_filter ) );
//	l_filter.CoCreateInstance( p_id);
	m_filterGraph->AddFilter( l_filter, p_name );
	return l_filter;
}

bool VideoImpl_DirectShow::_initialiseInstances( const String & p_url )
{
	ICaptureGraphBuilder2 * l_graphBuilder;
	IFilterGraph2 * l_filterGraph;
	CoCreateInstance( CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2,
					  reinterpret_cast <void **>( & l_graphBuilder ) );
	CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2,
					  reinterpret_cast <void **>( & l_filterGraph ) );
//	l_graphBuilder.CoCreateInstance( CLSID_CaptureGraphBuilder2);
//	l_filterGraph.CoCreateInstance( CLSID_FilterGraph);
	m_filterGraph = l_filterGraph;
	m_filterGraph->QueryInterface( IID_IMediaControl,	reinterpret_cast <void **>( & m_mediaControl ) );
	m_filterGraph->QueryInterface( IID_IMediaSeeking,	reinterpret_cast <void **>( & m_mediaSeeking ) );
	m_filterGraph->QueryInterface( IID_IMediaEvent,		reinterpret_cast <void **>( & m_mediaEvent ) );
	m_filterGraph->QueryInterface( IID_IMediaEventEx,	reinterpret_cast <void **>( & m_mediaEventEx ) );
	m_filterGraph->QueryInterface( IID_IBasicAudio,		reinterpret_cast <void **>( & m_audio ) );
	l_graphBuilder->SetFiltergraph( m_filterGraph );
	return true;
}

bool VideoImpl_DirectShow::_getDimensions()
{
	bool l_mediaConnected = false;
	AM_MEDIA_TYPE l_connectedType;

	if ( FAILED( m_sampleGrabber->GetConnectedMediaType( & l_connectedType ) ) )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "failed to get dimensions : no connected media type" );
		return false;
	}

	if ( l_connectedType.formattype != FORMAT_VideoInfo )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "failed to get dimensions : bad format type" );
		CoTaskMemFree( l_connectedType.pbFormat );
		return false;
	}

	VIDEOINFOHEADER * l_infoHeader = reinterpret_cast<VIDEOINFOHEADER *>( l_connectedType.pbFormat );
	m_vidWidth = l_infoHeader->bmiHeader.biWidth;
	m_vidHeight = l_infoHeader->bmiHeader.biHeight;
	m_vidPitch = m_vidWidth * 4;
	m_texWidth = General::Math::NextPowOfTwo( m_vidWidth );
	m_texHeight = General::Math:: NextPowOfTwo( m_vidHeight );
	m_texPitch = m_texWidth * 4;
	m_verticScale = m_vidHeight * 1.0f / m_texHeight;
	m_horizScale = m_vidWidth * 1.0f / m_texWidth;
	m_flip = true;

	if ( m_vidWidth < 0 )
	{
		m_vidWidth = -m_vidWidth;
		m_vidPitch = -m_vidPitch;
		m_flip = false;
	}

	m_bufferLength = m_vidWidth * m_vidHeight * 4;
	m_owner->SetSize( static_cast <unsigned int>( m_texWidth ), static_cast <unsigned int>( m_texHeight ) );

	if ( m_buffer == NULL )
	{
		m_buffer = new unsigned char[m_bufferLength];
		m_buffer2 = new unsigned char[m_texWidth * m_texHeight * 4];
	}

	CoTaskMemFree( l_connectedType.pbFormat );
	return true;
}

void VideoImpl_DirectShow::_initialise()
{
	try
	{
		WCHAR l_wUrl[MAX_PATH];
		MultiByteToWideChar( CP_ACP, 0, m_url.c_str(), -1, l_wUrl, MAX_PATH );
		IBaseFilter * l_source;

		if ( FAILED( m_filterGraph->AddSourceFilter( l_wUrl, L"Video Source", & l_source ) ) )
		{
			return;
		}

		IBaseFilter * l_splitter;
		IBaseFilter * l_grabber;
		IBaseFilter * l_audio;
		IBaseFilter * l_null;
		l_grabber = _createSampleGrabber();
		l_audio = _createFilter( CLSID_DSoundRender, L"Audio Renderer" );
		l_null = _createFilter( CLSID_NullRenderer, L"Null Renderer" );

		if ( m_format == EM_DS_VIDEO_FLV )
		{
			l_splitter = _findSplitter( L"FLV Splitter" );
			_connectPins( l_source, 0, l_audio, 0 );
			_connectPins( l_splitter, 1, l_grabber, 0 );
		}
		else if ( m_format == EM_DS_VIDEO_REAL )
		{
			l_splitter = _findSplitter( L"RealMedia Splitter" );
			_connectPins( l_source, 0, l_audio, 0 );
			_connectPins( l_splitter, 1, l_grabber, 0 );
		}
		else if ( m_format == EM_DS_VIDEO_WMV )
		{
			_connectPins( l_source, 0, l_audio, 0 );
			_connectPins( l_source, 1, l_grabber, 0 );
		}
		else
		{
			if ( m_format == EM_DS_VIDEO_AVI )
			{
				l_splitter = _createFilter( CLSID_AviSplitter, L"AVI Splitter" );
			}
			else if ( m_format == EM_DS_VIDEO_MPEG1 )
			{
				l_splitter = _createFilter( CLSID_MPEG1Splitter, L"MPEG 1Splitter" );
			}

			genlib_assert( _connectPins( l_source, 0, l_splitter, 0 ) );
			_connectPins( l_splitter, 1, l_audio, 0 );
			genlib_assert( _connectPins( l_splitter, 0, l_grabber, 0 ) );
		}

		_connectPins( l_grabber, 0, l_null, 0 );
		_getDimensions();

		if ( m_muted )
		{
			m_audio->put_Volume( -10000 );
		}
		else
		{
			m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
		}

		//TODO : video de merde
		{
			//	GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex);
			m_owner->InitialiseVideoTexture();
			m_ready = true;
			Start();
		}

		while ( ! Update( 0.0 ) )
		{
			GENLIB_SLEEP( 5 );
		}

		{
			Start();
			GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );
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
	catch ( GenException & p_e )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Unexpected exception throw while initialising video named " + m_owner->GetName() + " / text : " + p_e.GetDescription() );
	}
}

void VideoImpl_DirectShow::Initialise( const Url & p_url )
{
	m_url = p_url;
	m_format = _getVideoType( p_url );

	if ( m_infiniteStream )
	{
		m_format = EM_DS_VIDEO_WMV;
	}

	_initialiseInstances( m_url );
	m_thread = GENLIB_THREAD_CREATE_MEMBER_FUNC_THREAD( VideoImpl_DirectShow, this, _initialise );
}

void VideoImpl_DirectShow::SetMaxVolume( int p_maxVolume )
{
	m_maxVolume = ( p_maxVolume * 100 ) - 10000;
	m_currentVolume = int( ( m_maxVolume + 10000 ) * m_volumePercent ) - 10000;

	if ( m_ready )
	{
		m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
	}
}

void VideoImpl_DirectShow::SetVolumePercent( Real p_percent )
{
	m_volumePercent = p_percent;
	m_currentVolume = int( ( m_maxVolume + 10000 ) * m_volumePercent ) - 10000;

	if ( m_ready )
	{
		m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
	}
}

void VideoImpl_DirectShow::Mute( bool p_muted )
{
	m_muted = p_muted;

	if ( m_ready )
	{
		if ( m_muted )
		{
			m_audio->put_Volume( -10000 );
		}
		else
		{
			m_audio->put_Volume( static_cast <long>( m_currentVolume ) );
		}
	}
}

bool VideoImpl_DirectShow::Update( Real p_time )
{
	{
		GENLIB_SCOPED_RECURSIVE_LOCK( m_mutex );

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
		if ( m_owner->IsLooped() )
		{
			unsigned long l_flags = AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame;
			__int64 l_position = 0;
			m_mediaSeeking->SetPositions( & l_position, l_flags, NULL, AM_SEEKING_NoPositioning );
			m_playTime = 0.0;
		}
		else
		{
			m_owner->Stop();
		}

		return false;
	}

	long l_eventCode, l_param1, l_param2;

	while ( SUCCEEDED( m_mediaEvent->GetEvent( & l_eventCode, & l_param1, & l_param2, 0 ) ) && ! m_streamEnded )
	{
		m_streamEnded = ( l_eventCode == EC_COMPLETE || l_eventCode == 64 );
		m_mediaEvent->FreeEventParams( l_eventCode, l_param1, l_param2 );
	}

	if ( m_bufferLength == 0 )
	{
		m_bufferLength = m_vidWidth * m_vidHeight * 4;
//		m_buffer = new unsigned char[m_bufferLength];
//		m_buffer2 = new unsigned char[m_texWidth * m_texHeight * 4];
	}

	return _grabFrame();
}

bool VideoImpl_DirectShow::_grabFrame()
{
	HRESULT hr = m_sampleGrabber->GetCurrentBuffer( & m_bufferLength, ( long * )m_buffer );

	if ( hr == S_OK )
	{
		unsigned char * l_targetLine = m_buffer2;
		unsigned char * l_sourceLine = m_buffer;

		if ( m_flip )
		{
			l_sourceLine = m_buffer + m_vidPitch * ( m_vidHeight - 1 );
		}

		for ( int i = 0; i < m_texHeight ; i ++ )
		{
			if ( ! m_flip )
			{
				l_sourceLine = m_buffer + ( ( int )( i * m_verticScale ) ) * m_vidPitch;
			}
			else
			{
				l_sourceLine =	m_buffer
								+ m_vidPitch * ( m_vidHeight - 1 )
								- ( ( int )( i * m_verticScale ) ) * m_vidPitch;
			}

			l_targetLine = m_buffer2 + i * m_texPitch;

			for ( int j = 0 ; j < m_texWidth ; j++ )
			{
				l_targetLine[j * 4    ] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 0];
				l_targetLine[j * 4 + 1] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 1];
				l_targetLine[j * 4 + 2] = l_sourceLine[( ( int )( j * m_horizScale ) ) * 4 + 2];
				l_targetLine[j * 4 + 3] = 0xFF;
			}
		}

		m_owner->GetVideoTexture()->CopyToBuffer( m_buffer2, static_cast <unsigned int>( m_texWidth * m_texHeight * 4 ) );
		m_owner->GetVideoTexture()->Update();
		return true;
	}

	if ( hr == E_INVALIDARG )
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - Samples are not being buffered. Call SetBufferSamples." );
	}
	else if ( hr == E_OUTOFMEMORY )
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The specified buffer is not large enough. - "
								  + StringConverter::toString( m_bufferLength ) );
	}
	else if ( hr == E_POINTER )
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - NULL pointer argument." );
	}
	else if ( hr == VFW_E_NOT_CONNECTED )
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The filter is not connected." );
	}
	else if ( hr == VFW_E_WRONG_STATE )
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - The filter has not received any samples yet." );
	}
	else
	{
		EMUSE_LOG_MESSAGE_NORMAL( "CVideoTexture::frameStarted - Unknown error : " + StringConverter::toString( hr ) + "." );
	}

	return false;
}

DShowVideoFormat VideoImpl_DirectShow::_getVideoType( const String & p_videoUrl )
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

//	return EM_DS_VIDEO_NONE;
}

#endif //ELYPSE_WINDOWS
