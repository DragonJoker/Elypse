/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Video_GStreamer.h"

#if ELYPSE_LINUX

#include "VideoFactory_GStreamer.h"

#undef new
#undef delete

#include <gtkmm.h>

GENLIB_INIT_SINGLETON( VideoFactory_GStreamer );

VideoFactory_GStreamer::VideoFactory_GStreamer()
	: VideoFactory( "GStreamer" )
	, m_factories( nullptr )
{
	GENLIB_SET_SINGLETON();
	int l_argc;
	gst_init( & l_argc, nullptr );
	_initFactories();
}

VideoFactory_GStreamer::~VideoFactory_GStreamer()
{
}

VideoImplementation * VideoFactory_GStreamer::Create( VideoObject * p_object )
{
	return new VideoImpl_GStreamer( p_object );
}

void VideoFactory_GStreamer::_initFactories()
{
	m_factories = gst_registry_feature_filter( gst_registry_get_default(), static_cast <GstPluginFeatureFilter>( VideoFactory_GStreamer::_featureFilterCB ), FALSE, nullptr );
	m_factories = g_list_sort( m_factories, reinterpret_cast <GCompareFunc>( VideoFactory_GStreamer::_compareRanksCB ) );
}

void VideoFactory_GStreamer::Update()
{
	Glib::MainContext::get_default()->iteration( false );
}

gboolean VideoFactory_GStreamer::_featureFilterCB( GstPluginFeature * p_feature, gpointer p_data )
{
	if ( ! GST_IS_ELEMENT_FACTORY( p_feature ) )
	{
		return false;
	}

	const gchar * l_klass = gst_element_factory_get_klass( GST_ELEMENT_FACTORY( p_feature ) );

	if ( g_strrstr( l_klass, "Demux" ) == nullptr
			&&	g_strrstr( l_klass, "Decoder" ) == nullptr
			&&	g_strrstr( l_klass, "Parse" ) == nullptr )
	{
		return false;
	}

	return ( gst_plugin_feature_get_rank( p_feature ) >= GST_RANK_MARGINAL );
}

gint VideoFactory_GStreamer::_compareRanksCB( GstPluginFeature * p_feature1, GstPluginFeature * p_feature2 )
{
	return gst_plugin_feature_get_rank( p_feature2 ) - gst_plugin_feature_get_rank( p_feature1 );
}

#endif // ELYPSE_LINUX
