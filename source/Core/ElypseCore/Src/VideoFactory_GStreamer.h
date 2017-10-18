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
#ifndef ___VIDEO_FACTORY_GSTREAMER_H___
#define ___VIDEO_FACTORY_GSTREAMER_H___

#include <Singleton.h>

#include "VideoFactory.h"

#if ELYPSE_LINUX
#include <gst/gst.h>

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoFactory_GStreamer
			: public General::Theory::Singleton< VideoFactory_GStreamer >
			, public VideoFactory
		{
		private:
			GList * m_factories;

		public:
			VideoFactory_GStreamer();
			virtual ~VideoFactory_GStreamer();

		private:
			virtual void Initialise() {}
			virtual void Update();
			virtual VideoImplementation * Create( VideoObject & p_owner );

		private:
			void _initFactories();

		private:
			static gboolean _featureFilterCB( GstPluginFeature * p_feature, gpointer p_data );
			static gint _compareRanksCB( GstPluginFeature * p_feature1, GstPluginFeature * p_feature2 );

		public:
			static GList * GetFactories()
			{
				return GetSingletonPtr()->m_factories;
			}
		};
	}
}
#endif // ELYPSE_LINUX

#endif
