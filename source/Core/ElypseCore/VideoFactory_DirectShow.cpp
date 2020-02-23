/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "VideoFactory_DirectShow.h"
#include "Video_DirectShow.h"

#if ELYPSE_WINDOWS

VideoFactory_DirectShow::VideoFactory_DirectShow()
	: VideoFactory( "DirectShow" )
{
}

VideoFactory_DirectShow::~VideoFactory_DirectShow()
{
}

void VideoFactory_DirectShow::Initialise()
{
}

void VideoFactory_DirectShow::Update()
{
}

VideoImplementation * VideoFactory_DirectShow::Create( VideoObject & p_owner )
{
	return new VideoImpl_DirectShow( p_owner );
}

#endif
