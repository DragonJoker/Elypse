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

#include "PostEffect_GaussianBlur.h"

#include <OgreViewport.h>
#include <OgreMath.h>
#include <OgreTechnique.h>

using namespace Ogre;

PostEffect_GaussianBlur::PostEffect_GaussianBlur( const String & p_name, Viewport * p_viewport )
	: PostEffect( p_name, p_viewport, -1 )
{
	m_listener = new GaussianListener;
	m_instance->addListener( m_listener );
	m_listener->notifyViewportSize( m_viewport->getActualWidth(), m_viewport->getActualHeight() );
}

PostEffect_GaussianBlur::~PostEffect_GaussianBlur()
{
	delete m_listener;
}

GaussianListener::GaussianListener()
{
}

GaussianListener::~GaussianListener()
{
}

void GaussianListener::notifyViewportSize( int width, int height )
{
	mVpWidth = width;
	mVpHeight = height;
	// Calculate gaussian texture offsets & weights
	float deviation = 3.0f;
	float texelSize = 1.0f / ( float )std::min( mVpWidth, mVpHeight );
	// central sample, no offset
	mBloomTexOffsetsHorz[0][0] = 0.0f;
	mBloomTexOffsetsHorz[0][1] = 0.0f;
	mBloomTexOffsetsVert[0][0] = 0.0f;
	mBloomTexOffsetsVert[0][1] = 0.0f;
	mBloomTexWeights[0][0] = mBloomTexWeights[0][1] = 
								 mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution( 0, 0, deviation );
	mBloomTexWeights[0][3] = 1.0f;

	// 'pre' samples
	for ( int i = 1; i < 8; ++i )
	{
		mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = mBloomTexWeights[i][2] = Ogre::Math::gaussianDistribution( Real( i ), 0, deviation );
		mBloomTexWeights[i][3] = 1.0f;
		mBloomTexOffsetsHorz[i][0] = i * texelSize;
		mBloomTexOffsetsHorz[i][1] = 0.0f;
		mBloomTexOffsetsVert[i][0] = 0.0f;
		mBloomTexOffsetsVert[i][1] = i * texelSize;
	}

	// 'post' samples
	for ( int i = 8; i < 15; ++i )
	{
		mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = 
									 mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
		mBloomTexWeights[i][3] = 1.0f;
		mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
		mBloomTexOffsetsHorz[i][1] = 0.0f;
		mBloomTexOffsetsVert[i][0] = 0.0f;
		mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
	}
}

void GaussianListener::notifyMaterialSetup( uint32 pass_id, MaterialPtr & mat )
{
	// Prepare the fragment params offsets
	switch ( pass_id )
	{
	case 701: // blur horz
	{
		// horizontal bloom
		mat->load();
		GpuProgramParametersSharedPtr fparams = 
			mat->getBestTechnique()->getPass( 0 )->getFragmentProgramParameters();
		fparams->setNamedConstant( "sampleOffsets", mBloomTexOffsetsHorz[0], 15 );
		fparams->setNamedConstant( "sampleWeights", mBloomTexWeights[0], 15 );
		break;
	}

	case 700: // blur vert
	{
		// vertical bloom
		mat->load();
		GpuProgramParametersSharedPtr fparams = 
			mat->getTechnique( 0 )->getPass( 0 )->getFragmentProgramParameters();
		fparams->setNamedConstant( "sampleOffsets", mBloomTexOffsetsVert[0], 15 );
		fparams->setNamedConstant( "sampleWeights", mBloomTexWeights[0], 15 );
		break;
	}
	}
}

void GaussianListener::notifyMaterialRender( uint32 pass_id, MaterialPtr & mat )
{
}
