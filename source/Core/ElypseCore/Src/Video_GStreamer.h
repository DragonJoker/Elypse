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
