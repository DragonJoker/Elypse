/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "VideoOverlay.h"
#include "VideoObject.h"

void VideoOverlay::Initialise( String const & p_videoTextureName )
{
	m_oldMaterialName = m_EMOverlay->GetOgreOverlayElement()->getMaterialName();
	m_EMOverlay->GetOgreOverlayElement()->setMaterialName( p_videoTextureName );
}

void VideoOverlay::Reset()
{
	m_EMOverlay->GetOgreOverlayElement()->setMaterialName( m_oldMaterialName );
}

void VideoOverlay::AddToZone( Zone * p_zone )
{
}

void VideoOverlay::RemoveFromZone( Zone * p_zone )
{
	GetOwner()->DeleteOverlay( m_EMOverlay );
}
