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

#include "ElypseLoadingBar.h"
#include "ElypseController.h"
#include "ElypseInstance.h"
#include "ElypsePlugin.h"

#include <OgreException.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>

ElypseLoadingBar::ElypseLoadingBar()
	: m_window( NULL )
	, m_loadOverlay( NULL )
	, m_loadingBarElement( NULL )
	, m_loadingDescriptionElement( NULL )
	, m_loadingPercentElement( NULL )
	, m_loadingOverlayName( "Core/LoadOverlay" )
	, m_loadingBarProgressName( "Core/LoadPanel/Bar/Progress" )
	, m_loadingBarPercentageName( "Core/LoadPanel/Bar/Percentage" )
	, m_loadingBarName( "Core/LoadPanel/Bar" )
	, m_loadingBarOffset( 22 )
	, m_started( false )
{
}

ElypseLoadingBar::~ElypseLoadingBar()
{
	if ( m_started )
	{
		finish();
	}
}

void ElypseLoadingBar::finish()
{
	if ( m_started )
	{
		m_loadOverlay->hide();
		m_started = false;
		ResourceGroupManager::getSingletonPtr()->removeResourceGroupListener( this );
	}
}

void ElypseLoadingBar::start( uint16_t p_numGroupsInit, uint16_t p_numGroupsLoad, Real p_initProportion )
{
	if ( p_numGroupsInit == 0 )
	{
		return;
	}

	if ( m_window == NULL )
	{
		return;
	}

	if ( m_started )
	{
		return;
	}

	m_numGroupsInit = p_numGroupsInit;
	m_numGroupsLoad = p_numGroupsLoad;
	m_initProportion = p_initProportion;
	OverlayManager * l_overMan = OverlayManager::getSingletonPtr();
	m_loadOverlay = static_cast <Overlay *>( l_overMan->getByName( m_loadingOverlayName ) );

	if ( m_loadOverlay == NULL )
	{
		return;
	}

	m_loadOverlay->show();
	m_loadingBarElement = l_overMan->getOverlayElement( m_loadingBarProgressName );
	m_loadingPercentElement = l_overMan->getOverlayElement( m_loadingBarPercentageName );
	OverlayElement * l_bar = l_overMan->getOverlayElement( m_loadingBarName );
	m_progressBarMaxSize = l_bar->getWidth() - m_loadingBarOffset;
	// add the listener
	ResourceGroupManager::getSingletonPtr()->addResourceGroupListener( this );
	m_loadingBarElement->setWidth( 0.0 );
	m_previousWidth = 0.0;
	m_started = true;
	_updateWindow();
}

void ElypseLoadingBar::_updateWindow()
{
	ElypseController::GetSingletonPtr()->GetOwner()->GetPlugin()->LockGui();
	m_window->update();
	ElypseController::GetSingletonPtr()->GetOwner()->GetPlugin()->UnlockGui();
}

void ElypseLoadingBar::UpdateBar()
{
	m_loadingBarElement->setWidth( m_loadingBarElement->getWidth() + m_progressBarInc );
	int percent = Math::IFloor( 100.0f * float( m_loadingBarElement->getWidth() ) / m_progressBarMaxSize );
	m_loadingPercentElement->setCaption( StringConverter::toString( percent ) + " %" );
	_updateWindow();
}

void ElypseLoadingBar::resourceGroupScriptingStarted( String const & p_name , size_t scriptCount )
{
	m_progressBarInc = m_progressBarMaxSize * m_initProportion / Real( scriptCount );
	m_progressBarInc /= m_numGroupsInit;
	_updateWindow();
}

void ElypseLoadingBar::scriptParseEnded( String const & p_name )
{
	UpdateBar();
}

void ElypseLoadingBar::resourceGroupLoadStarted( String const & p_name, size_t resourceCount )
{
	m_progressBarInc = m_progressBarMaxSize * ( 1 - m_initProportion ) / Real( resourceCount );
	m_progressBarInc /= m_numGroupsLoad;
	_updateWindow();
}


void ElypseLoadingBar::resourceLoadEnded()
{
	UpdateBar();
}

void ElypseLoadingBar::resourceDownloadStarted( String const & p_name )
{
	m_dlres = p_name;
}

void ElypseLoadingBar::setPercent( float p_percent, float p_downSpeed )
{
	float l_width = float( Math::IFloor( m_progressBarMaxSize * p_percent ) );
	String l_tmp = m_dlres + " - ";

	if ( p_percent < 1.0 )
	{
		l_tmp += StringConverter::toString( Math::IFloor( p_percent * 100.0f ) );
	}
	else
	{
		l_tmp += "100";
	}

	l_tmp += "%, " + StringConverter::toString( Math::IFloor( p_downSpeed ) ) + "Ko/s.";
	m_loadingPercentElement->setCaption( l_tmp );
	m_loadingBarElement->setWidth( l_width );
	_updateWindow();
}

void ElypseLoadingBar::StartDownloadOfFile()
{
	m_startByDownload = ! m_started;

	if ( m_startByDownload )
	{
		start( 1, 0, Real( 0.1 ) );
	}
	else
	{
		m_previousWidth = m_loadingBarElement->getWidth();
		m_loadingPercentElement->setCaption( m_dlres + " - 0%" );
		m_loadingBarElement->setWidth( 0 );
		_updateWindow();
	}
}

void ElypseLoadingBar::EndDownloadOfFile()
{
	if ( m_startByDownload )
	{
		finish();
	}
	else
	{
		m_loadingBarElement->setWidth( m_previousWidth );
		_updateWindow();
	}
}
