/*
See LICENSE file in root folder
*/
#ifndef ___VIDEO_GSTREAMER_H___
#define ___VIDEO_GSTREAMER_H___

#include "VideoImplementation.h"
#include "VideoFactory.h"

#if ELYPSE_LINUX

#include <gst/gst.h>

typedef std::set <GstElement *> GstEltSet;

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoImpl_GStreamer
			: public VideoImplementation
		{
		public:
			VideoImpl_GStreamer( VideoObject & p_owner );
			virtual ~VideoImpl_GStreamer();

		public:
			virtual bool Start();
			virtual bool Stop();
			virtual bool Pause();
			virtual void Reset();
			virtual void Initialise( Url const & p_mediaUrl );
			virtual bool Update( Real p_time )
			{
				return true;
			}
			virtual void SetMaxVolume( int p_maxVolume ) {}
			virtual void SetVolumePercent( Real p_percent ) {}
			virtual void Mute( bool p_muted ) {}

		private:
			static gboolean _busCB( GstBus * p_bus, GstMessage * p_msg, gpointer p_data );
			static void _newPadCB( GstElement * p_elt, GstPad * p_pad, gpointer p_data );
			static void _typeFoundCB( GstElement * p_typeFind, guint p_probability, GstCaps * p_caps, gpointer p_data );
			static void _handoffCB( GstElement * p_identitySink, GstBuffer * p_buffer, void *, VideoImpl_GStreamer * p_player );

		private:
			void _closeLink( GstPad * p_srcPad, GstElement * p_sinkElt, const gchar * p_padName, const GList * p_tmplList );
			void _tryToPlug( GstPad * p_pad, const GstCaps * p_caps );
			bool _tryToPlugAudio( GstPad * p_pad, const GstCaps * p_caps );
			bool _tryToPlugVideo( GstPad * p_pad, const GstCaps * p_caps );
			void _cleanup();

		private:
			bool m_videoLinked;
			GstEltSet m_binEltSet;

			GstElement * m_fileSrc;
			GstElement * m_typeFind;
			GstElement * m_audioQueue;
			GstElement * m_audioConverter;
			GstElement * m_audioSink;
			GstElement * m_videoQueue;
			GstElement * m_videoConverter;
			GstElement * m_videoScaler;
			GstElement * m_videoScaleFilter;
			GstElement * m_videoSink;
			GstElement * m_pipeline;
			GstCaps * m_videoCaps;
			char * m_mediaUrlStr;
		};
	}
}

#endif // ELYPSE_LINUX

#endif
