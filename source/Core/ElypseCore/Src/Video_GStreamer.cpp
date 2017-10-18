/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "PrecompiledHeader.h"

#include "Video_GStreamer.h"

#if ELYPSE_LINUX

#include "VideoObject.h"
#include "VideoTexture.h"
#include "VideoManager.h"

#include "VideoFactory_GStreamer.h"

#include "ElypseLogs.h"

#include <StringConverter.h>

VideoImpl_GStreamer::VideoImpl_GStreamer( VideoObject & p_owner )
	: VideoImplementation( p_owner )
	, m_videoLinked( false )
	, m_pipeline( NULL )
	, m_videoCaps( NULL )
{
}

VideoImpl_GStreamer::~VideoImpl_GStreamer()
{
	_cleanup();

	if ( m_mediaUrlStr != NULL )
	{
		delete [] m_mediaUrlStr;
		m_mediaUrlStr = NULL;
	}
}

void VideoImpl_GStreamer::_cleanup()
{
	m_videoLinked = false;

	if ( m_videoCaps != NULL )
	{
		gst_caps_unref( m_videoCaps );
		m_videoCaps = NULL;
	}

	if ( m_pipeline != NULL )
	{
		gst_element_set_state( m_pipeline, GST_STATE_NULL );
		gst_object_unref( GST_OBJECT( m_pipeline ) );
		m_pipeline = NULL;
	}

	m_binEltSet.clear();
}

bool VideoImpl_GStreamer::Start()
{
	if ( m_pipeline == NULL )
	{
		return false;
	}

	GstEltSet::const_iterator i = m_binEltSet.begin();
	const GstEltSet::const_iterator & iend = m_binEltSet.end();

	for ( ; i != iend; ++ i )
	{
		gst_element_set_state( GST_ELEMENT( * i ), GST_STATE_PLAYING );
	}

	return true;
}

bool VideoImpl_GStreamer::Pause()
{
	if ( m_pipeline == NULL )
	{
		return false;
	}

	gst_element_set_state( GST_ELEMENT( m_pipeline ), GST_STATE_PAUSED );
	return true;
}

bool VideoImpl_GStreamer::Stop()
{
	if ( m_pipeline == NULL )
	{
		return false;
	}

	Pause();
	gst_element_seek_simple( m_pipeline, GST_FORMAT_TIME, static_cast <GstSeekFlags>( GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_ACCURATE ), 0 );
	return true;
}

void VideoImpl_GStreamer::Reset()
{
	_cleanup();
}

void VideoImpl_GStreamer::Initialise( Url const & p_mediaUrl )
{
	if ( m_pipeline != NULL )
	{
		return;
	}

	m_pipeline = gst_pipeline_new( "pipeline" );

	if ( m_pipeline == NULL )
	{
		GENLIB_EXCEPTION( "Can't create pipeline, something went wrong in gstreamer library, check for your gstreamer version please..." );
	}

	GstBus * l_bus = gst_pipeline_get_bus( GST_PIPELINE( m_pipeline ) );
#if ELYPSE_LINUX

	if ( p_mediaUrl.find( "mms://" ) == 0
			|| p_mediaUrl.find( "mmsu://" ) == 0
			|| p_mediaUrl.find( "mmst://" ) == 0 )
	{
		m_fileSrc = gst_element_factory_make( "mmssrc", "fileSource" );
	}
	else
	{
//		filesrc works fine, but can't handle protocol like file://, http://, ftp://, etc
		m_fileSrc = gst_element_factory_make( "gnomevfssrc", "fileSource" );
//		m_fileSrc = gst_element_factory_make( "filesrc", "fileSource");
	}

#elif ELYPSE_WINDOWS

	if ( p_mediaUrl.find( "http://" ) == 0
			|| p_mediaUrl.find( "https://" ) == 0 )
	{
		m_fileSrc = gst_element_factory_make( "neonhttpsrc", "fileSource" );
	}
	else
	{
//		filesrc works fine, but can't handle protocol like file://, http://, ftp://, etc
//		m_fileSrc = gst_element_factory_make( "gnomevfssrc", "fileSource");
		m_fileSrc = gst_element_factory_make( "filesrc", "fileSource" );
	}

#endif
	String l_medialUrl = p_mediaUrl;

	if ( l_medialUrl.find( "file://" ) == 0 )
	{
		l_medialUrl = l_medialUrl.substr( 7 );
	}

	m_mediaUrlStr = CStrCopy( l_medialUrl );
	m_typeFind = gst_element_factory_make( "typefind", "typeFinder" );
	m_audioQueue = gst_element_factory_make( "queue", "audioQueue" );
	m_audioConverter = gst_element_factory_make( "audioconvert", "audioConverter" );
	m_audioSink = gst_element_factory_make( "directsoundsink", "audioOutput" );
	m_videoQueue = gst_element_factory_make( "queue", "videoQueue" );
	m_videoConverter = gst_element_factory_make( "ffmpegcolorspace", "videoColorConverter" );
	m_videoScaler = gst_element_factory_make( "videoscale", "videoScaler" );
	m_videoScaleFilter = gst_element_factory_make( "capsfilter", "videoScaleFilter" );
	m_videoSink = gst_element_factory_make( "fakesink", "videoOutput" );

	if ( l_bus == NULL
			|| m_fileSrc == NULL || m_typeFind == NULL
			|| m_videoQueue == NULL || m_audioQueue == NULL
			|| m_audioSink == NULL || m_audioConverter == NULL || m_videoScaleFilter == NULL
			|| m_videoSink == NULL || m_videoScaler == NULL || m_videoConverter == NULL )
	{
		GENLIB_EXCEPTION( "Something went wrong in creating pipeline element, check your gstreamer plugins versions please..." );
	}

	gst_bin_add_many( GST_BIN( m_pipeline ),
					  m_fileSrc, m_typeFind,
					  m_audioQueue, m_audioConverter, m_audioSink, NULL );
	m_binEltSet.insert( m_pipeline );
	m_binEltSet.insert( m_fileSrc );
	m_binEltSet.insert( m_typeFind );
	m_binEltSet.insert( m_audioQueue );
	m_binEltSet.insert( m_audioSink );
	m_binEltSet.insert( m_audioConverter );
	gst_bus_add_watch( l_bus, VideoImpl_GStreamer::_busCB, this );
	gst_object_unref( l_bus );
	g_object_set( G_OBJECT( m_fileSrc ), "location", m_mediaUrlStr, NULL );
	g_signal_connect( m_typeFind, "have-type", G_CALLBACK( VideoImpl_GStreamer::_typeFoundCB ), this );
	gst_element_link( m_fileSrc, m_typeFind );
	gst_element_link_many( m_audioQueue, m_audioConverter, m_audioSink, NULL );
}

gboolean VideoImpl_GStreamer::_busCB( GstBus * p_bus, GstMessage * p_msg, gpointer p_data )
{
	VideoImpl_GStreamer * l_player = reinterpret_cast <VideoImpl_GStreamer *>( p_data );

	switch ( GST_MESSAGE_TYPE( p_msg ) )
	{
	case GST_MESSAGE_EOS:
	{
		EMUSE_LOG_MESSAGE_DEBUG( String( "End of video stream [" ) + l_player->GetOwner()->GetMediaUrl() + "]" );
		l_player->GetOwner()->Finish();
		return true;
	}

	case GST_MESSAGE_ERROR:
	{
		GError * l_err;
		gchar * l_debug;
		gst_message_parse_error( p_msg, & l_err, & l_debug );
		EMUSE_LOG_MESSAGE_NORMAL( String( "Video error [" ) + l_err->message + "]" );
		g_error_free( l_err );
		g_free( l_debug );
		l_player->GetOwner()->Finish();
		return true;
	}

	default:
	{
		return true;
	}
	}
}

void VideoImpl_GStreamer::_newPadCB( GstElement * p_elt, GstPad * p_pad, gpointer p_data )
{
	genlib_assert( p_data != NULL );
	GstCaps * l_caps = gst_pad_get_caps( p_pad );
	reinterpret_cast <VideoImpl_GStreamer *>( p_data )->_tryToPlug( p_pad, l_caps );
	gst_caps_unref( l_caps );
}

void VideoImpl_GStreamer::_typeFoundCB( GstElement * p_typeFind, guint p_probability, GstCaps * p_caps, gpointer p_data )
{
	genlib_assert( p_data != NULL );
	gchar * l_strCaps = gst_caps_to_string( p_caps );
	GstPad * l_pad = gst_element_get_static_pad( p_typeFind, "src" );
	EMUSE_LOG_MESSAGE_DEBUG( String( "Detected media type [" ) + l_strCaps + "]" );
	reinterpret_cast <VideoImpl_GStreamer *>( p_data )->_tryToPlug( l_pad, p_caps );
	gst_object_unref( GST_OBJECT( l_pad ) );
	g_free( l_strCaps );
}

void VideoImpl_GStreamer::_handoffCB( GstElement *, GstBuffer * p_buffer, void *, VideoImpl_GStreamer * p_player )
{
	genlib_assert( p_player != NULL );
	genlib_assert( p_buffer != NULL );
	p_player->GetOwner()->GetVideoTexture()->CopyToBuffer( GST_BUFFER_DATA( p_buffer ), GST_BUFFER_SIZE( p_buffer ) );
}

void VideoImpl_GStreamer::_closeLink( GstPad * p_srcPad, GstElement * p_sinkElt, const gchar * p_padName, const GList * p_tmplList )
{
	genlib_assert( p_srcPad != NULL );
	genlib_assert( p_sinkElt != NULL );
	genlib_assert( p_padName != NULL );
	String l_sinkName( gst_object_get_name( GST_OBJECT( p_sinkElt ) ) );
	EMUSE_LOG_MESSAGE_DEBUG( String( "Plugging pad " ) + gst_object_get_name( GST_OBJECT( gst_pad_get_parent( p_srcPad ) ) ) + ":" + gst_pad_get_name( p_srcPad )
							 + " to newly created " + l_sinkName + ":" + p_padName );

	if ( m_binEltSet.find( p_sinkElt ) == m_binEltSet.end() )
	{
		EMUSE_LOG_MESSAGE_DEBUG( String( "Adding [" ) + l_sinkName + "] to pipeline" );
		gst_bin_add( GST_BIN( m_pipeline ), p_sinkElt );
		m_binEltSet.insert( p_sinkElt );
		gst_element_set_state( p_sinkElt, GST_STATE_READY );
	}

	GstPad * l_pad = gst_element_get_static_pad( p_sinkElt, p_padName );

	if ( l_pad == NULL )
	{
		l_pad = gst_element_get_pad( p_sinkElt, "sink" );
	}

	gst_pad_link( p_srcPad, l_pad );
	gst_object_unref( GST_OBJECT( l_pad ) );

	if ( p_sinkElt != m_audioQueue && p_sinkElt != m_videoQueue
			&& l_sinkName.find( "id3demux" ) == std::string::npos )
	{
		gst_element_set_state( p_sinkElt, GST_STATE_PAUSED );
	}

	bool l_hasDynamicPads = false;

	for ( ; p_tmplList != NULL ; p_tmplList = p_tmplList->next )
	{
		GstStaticPadTemplate * l_template = reinterpret_cast <GstStaticPadTemplate *>( p_tmplList->data );

		if ( l_template->direction != GST_PAD_SRC || l_template->presence == GST_PAD_REQUEST )
		{
			continue;
		}

		switch ( l_template->presence )
		{
		case GST_PAD_ALWAYS:
		{
			GstPad * l_pad2 = gst_element_get_static_pad( p_sinkElt, l_template->name_template );
			GstCaps * l_caps = gst_pad_get_caps( l_pad2 );
			_tryToPlug( l_pad2, l_caps );
			gst_object_unref( GST_OBJECT( l_pad2 ) );
			gst_caps_unref( l_caps );
			break;
		}

		case GST_PAD_SOMETIMES:
		{
			l_hasDynamicPads = true;
			break;
		}

		default:
			break;
		}
	}

	if ( l_hasDynamicPads )
	{
		g_signal_connect( p_sinkElt, "pad-added", G_CALLBACK( VideoImpl_GStreamer::_newPadCB ), this );
	}

	if ( p_sinkElt != m_audioQueue && p_sinkElt != m_videoQueue
			&& l_sinkName.find( "id3demux" ) != std::string::npos )
	{
		gst_element_set_state( p_sinkElt, GST_STATE_PLAYING );
	}
}

void VideoImpl_GStreamer::_tryToPlug( GstPad * p_pad, const GstCaps * p_caps )
{
	GstObject * l_parent = GST_OBJECT_PARENT( p_pad );
	const gchar * l_mime = gst_structure_get_name( gst_caps_get_structure( p_caps, 0 ) );

	if ( g_strrstr( l_mime, "video" ) )
	{
		if ( ! m_videoLinked )
		{
			GstCaps * l_videoScalerCaps = gst_caps_new_simple(
											  "video/x-raw-rgb",
											  "width", G_TYPE_INT, GetOwner()->GetWidth(),
											  "height", G_TYPE_INT, GetOwner()->GetHeight(),
											  "bpp", G_TYPE_INT, 32,
											  "depth", G_TYPE_INT, 32,
											  "endianness", G_TYPE_INT, G_BIG_ENDIAN, NULL );
			g_object_set( G_OBJECT( m_videoScaleFilter ), "caps", l_videoScalerCaps, NULL );
			g_object_set( G_OBJECT( m_videoScaler ), "method", 1, NULL );
			g_object_set( G_OBJECT( m_videoSink ), "signal-handoffs", TRUE, "sync", TRUE, "can-activate-pull", TRUE, NULL );
			g_signal_connect( m_videoSink, "handoff", G_CALLBACK( VideoImpl_GStreamer::_handoffCB ), this );
			gst_caps_unref( l_videoScalerCaps );
			gst_bin_add_many( GST_BIN( m_pipeline ),
							  m_videoQueue, m_videoConverter, m_videoScaler, m_videoScaleFilter, m_videoSink, NULL );
			gst_element_link_many( m_videoQueue, m_videoConverter, m_videoScaler, m_videoScaleFilter, m_videoSink, NULL );
			m_binEltSet.insert( m_videoQueue );
			m_binEltSet.insert( m_videoConverter );
			m_binEltSet.insert( m_videoScaler );
			m_binEltSet.insert( m_videoScaleFilter );
			m_binEltSet.insert( m_videoSink );
			m_videoCaps = gst_pad_get_caps( gst_element_get_static_pad( m_videoQueue, "sink" ) );
			m_videoLinked = true;
		}

		if ( GST_PAD_IS_LINKED( gst_element_get_static_pad( m_videoQueue, "sink" ) ) )
		{
			EMUSE_MESSAGE_NORMAL( String( "Omitting link for pad " ) + GST_OBJECT_NAME( l_parent ) + ":" + GST_OBJECT_NAME( p_pad ) + " because we're already linked" );
			return;
		}

		if ( ! _tryToPlugVideo( p_pad, p_caps ) )
		{
			EMUSE_MESSAGE_NORMAL( String( "No compatible pad found to decode " ) + l_mime + " on " + GST_OBJECT_NAME( l_parent ) + ":" + GST_OBJECT_NAME( p_pad ) );
			return;
		}

		gst_element_set_state( m_videoQueue, GST_STATE_PLAYING );
		gst_element_set_state( m_videoConverter, GST_STATE_PLAYING );
		gst_element_set_state( m_videoScaler, GST_STATE_PLAYING );
		gst_element_set_state( m_videoScaleFilter, GST_STATE_PLAYING );
		gst_element_set_state( m_videoSink, GST_STATE_PLAYING );
		return;
	}

	if ( GST_PAD_IS_LINKED( gst_element_get_static_pad( m_audioQueue, "sink" ) ) )
	{
		EMUSE_MESSAGE_NORMAL( String( "Omitting link for pad " ) + GST_OBJECT_NAME( l_parent ) + ":" + GST_OBJECT_NAME( p_pad ) + " because we're already linked" );
		return;
	}

	if ( ! _tryToPlugAudio( p_pad, p_caps ) )
	{
		EMUSE_MESSAGE_NORMAL( String( "No compatible pad found to decode " ) + l_mime + " on " + GST_OBJECT_NAME( l_parent ) + ":" + GST_OBJECT_NAME( p_pad ) );
		return;
	}

	gst_element_set_state( m_audioQueue, GST_STATE_PLAYING );
	gst_element_set_state( m_audioConverter, GST_STATE_PLAYING );
	gst_element_set_state( m_audioSink, GST_STATE_PLAYING );
}

bool VideoImpl_GStreamer::_tryToPlugAudio( GstPad * p_pad, const GstCaps * p_caps )
{
	GstCaps * l_audioCaps = gst_pad_get_caps( gst_element_get_static_pad( m_audioQueue, "sink" ) );
	GstCaps * l_res = gst_caps_intersect( p_caps, l_audioCaps );

	if ( l_res != NULL && ! gst_caps_is_empty( l_res ) )
	{
		_closeLink( p_pad, m_audioQueue, "sink", NULL );
		gst_caps_unref( l_audioCaps );
		gst_caps_unref( l_res );
		return true;
	}

	gst_caps_unref( l_audioCaps );
	gst_caps_unref( l_res );

	for ( const GList * l_item = VideoFactory_GStreamer::GetFactories() ; l_item != NULL ; l_item = l_item->next )
	{
		GstElementFactory * l_factory = GST_ELEMENT_FACTORY( l_item->data );

		for ( const GList * l_pads = gst_element_factory_get_static_pad_templates( l_factory ) ; l_pads != NULL ; l_pads = l_pads->next )
		{
			GstStaticPadTemplate * l_template = reinterpret_cast <GstStaticPadTemplate *>( l_pads->data );

			if ( l_template->direction != GST_PAD_SINK || l_template->presence != GST_PAD_ALWAYS )
			{
				continue;
			}

			l_res = gst_caps_intersect( p_caps, gst_static_caps_get( & l_template->static_caps ) );

			if ( l_res != NULL && ! gst_caps_is_empty( l_res ) )
			{
				gchar * l_templateName = g_strdup( l_template->name_template );
				GstElement * l_elt = gst_element_factory_create( l_factory, NULL );
				_closeLink( p_pad, l_elt, l_templateName, gst_element_factory_get_static_pad_templates( l_factory ) );
				gst_caps_unref( l_res );
				g_free( l_templateName );
				return true;
			}

			gst_caps_unref( l_res );
			break;
		}
	}

	return false;
}

bool VideoImpl_GStreamer::_tryToPlugVideo( GstPad * p_pad, const GstCaps * p_caps )
{
	GstCaps * l_res = gst_caps_intersect( p_caps, m_videoCaps );

	if ( l_res != NULL && ! gst_caps_is_empty( l_res ) )
	{
		_closeLink( p_pad, m_videoQueue, "sink", NULL );
		gst_caps_unref( l_res );
		return true;
	}

	gst_caps_unref( l_res );
	const GList * l_item = VideoFactory_GStreamer::GetFactories();

	for ( ; l_item != NULL ; l_item = l_item->next )
	{
		GstElementFactory * l_factory = GST_ELEMENT_FACTORY( l_item->data );

		for ( const GList * l_pads = gst_element_factory_get_static_pad_templates( l_factory ) ; l_pads != NULL ; l_pads = l_pads->next )
		{
			GstStaticPadTemplate * l_template = reinterpret_cast <GstStaticPadTemplate *>( l_pads->data );

			if ( l_template->direction != GST_PAD_SINK || l_template->presence != GST_PAD_ALWAYS )
			{
				continue;
			}

			l_res = gst_caps_intersect( p_caps, gst_static_caps_get( & l_template->static_caps ) );

			if ( l_res != NULL && ! gst_caps_is_empty( l_res ) )
			{
				gchar * l_templateName = g_strdup( l_template->name_template );
				GstElement * l_elt = gst_element_factory_create( l_factory, NULL );
				_closeLink( p_pad, l_elt, l_templateName, gst_element_factory_get_static_pad_templates( l_factory ) );
				gst_caps_unref( l_res );
				g_free( l_templateName );
				return true;
			}

			gst_caps_unref( l_res );
			break;
		}
	}

	return false;
}

#endif  // ELYPSE_LINUX
