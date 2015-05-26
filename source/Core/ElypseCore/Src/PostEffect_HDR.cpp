#include "PrecompiledHeader.h"

#include "PostEffect_HDR.h"

#include <OgreCompositorManager.h>
#include <OgreCompositionTechnique.h>
#include <OgreViewport.h>
#include <OgreTechnique.h>

using namespace Ogre;

PostEffect_HDR :: PostEffect_HDR( const String & p_name, Viewport * p_viewport )
	:	PostEffect( p_name, p_viewport, 0 )
{
	if ( m_instance != NULL )
	{
		m_listener = new HDR_Listener;
		m_instance->addListener( m_listener );
		m_listener->notifyViewportSize( m_viewport->getActualWidth(), m_viewport->getActualHeight() );
		m_listener->notifyCompositor( m_instance );
	}
}

PostEffect_HDR :: ~PostEffect_HDR()
{
	delete m_listener;
}

HDR_Listener :: HDR_Listener()
{
}

HDR_Listener :: ~HDR_Listener()
{
}

void HDR_Listener :: notifyViewportSize( int p_width, int p_height )
{
	mVpWidth = p_width;
	mVpHeight = p_height;
}

void HDR_Listener :: notifyCompositor( CompositorInstance * instance )
{
	try
	{
		// Get some RTT dimensions for later calculations
		Ogre::CompositionTechnique::TextureDefinitionIterator defIter =
			instance->getTechnique()->getTextureDefinitionIterator();

		while ( defIter.hasMoreElements() )
		{
			Ogre::CompositionTechnique::TextureDefinition * def =
				defIter.getNext();

			if ( def->name == "rt_bloom0" )
			{
				mBloomSize = ( int )def->width; // should be square
				// Calculate gaussian texture offsets & weights
				float deviation = 5.0f;
				float texelSize = 1.0f / ( float )mBloomSize;
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
					mBloomTexWeights[i][0] = mBloomTexWeights[i][1] = mBloomTexWeights[i][2] = 1.25f * Ogre::Math::gaussianDistribution( Real( i ), 0, deviation );
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
		}
	}
	catch ( Ogre::Exception & )
	{
	}
}

void HDR_Listener :: notifyMaterialSetup( uint32 pass_id, MaterialPtr & mat )
{
	try
	{
		// Prepare the fragment params offsets
		switch ( pass_id )
		{
		//case 994: // rt_lum4
		case 993: // rt_lum3
		case 992: // rt_lum2
		case 991: // rt_lum1
		case 990: // rt_lum0
		case 800: // rt_brightpass
		{
			break;
		}

		case 701: // rt_bloom1
		{
			// horizontal bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getBestTechnique()->getPass( 0 )->getFragmentProgramParameters();
//				const Ogre::String& progName = mat->getBestTechnique()->getPass(0)->getFragmentProgramName();
			fparams->setNamedConstant( "sampleOffsets", mBloomTexOffsetsHorz[0], 15 );
			fparams->setNamedConstant( "sampleWeights", mBloomTexWeights[0], 15 );
			break;
		}

		case 700: // rt_bloom0
		{
			// vertical bloom
			mat->load();
			Ogre::GpuProgramParametersSharedPtr fparams =
				mat->getTechnique( 0 )->getPass( 0 )->getFragmentProgramParameters();
//				const Ogre::String& progName = mat->getBestTechnique()->getPass(0)->getFragmentProgramName();
			fparams->setNamedConstant( "sampleOffsets", mBloomTexOffsetsVert[0], 15 );
			fparams->setNamedConstant( "sampleWeights", mBloomTexWeights[0], 15 );
			break;
		}
		}
	}
	catch ( Exception & )
	{
	}
}

void HDR_Listener :: notifyMaterialRender( uint32, MaterialPtr & )
{
}
