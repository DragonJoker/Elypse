#ifndef ___VIDEO_FACTORY_GSTREAMER_H___
#define ___VIDEO_FACTORY_GSTREAMER_H___

#include <Singleton.h>

#include "VideoFactory.h"

#if ELYPSE_LINUX
#include <gst/gst.h>

namespace EMuse
{
	namespace Media
	{
		class d_dll_export VideoFactory_GStreamer : public General::Theory::Singleton <VideoFactory_GStreamer>, public VideoFactory
		{
		private:
			GList * m_factories;

		public:
			VideoFactory_GStreamer();
			virtual ~VideoFactory_GStreamer();

		private:
			virtual void Initialise() {}
			virtual void Update();
			virtual VideoImplementation * Create( VideoObject * p_owner );

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
