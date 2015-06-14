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
#include "PostEffect_MotionBlur.h"

#include <OgreCompositor.h>
#include <OgreCompositorManager.h>
#include <OgreCompositionTechnique.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

using namespace Ogre;

bool PostEffect_MotionBlur::sm_initialised = false;

PostEffect_MotionBlur::PostEffect_MotionBlur( const String & p_name, Viewport * p_viewport )
	: PostEffect( p_name, p_viewport, -1, false )
{
	if ( ! PostEffect_MotionBlur::IsInitialised() )
	{
		PostEffect_MotionBlur::Initialise();
	}

	m_instance = CompositorManager::getSingleton().addCompositor( p_viewport, p_name );
	SetEnabled( true );
}

PostEffect_MotionBlur::~PostEffect_MotionBlur()
{
}

void PostEffect_MotionBlur::Initialise()
{
	sm_initialised = true;
	CompositorPtr comp3 = CompositorManager::getSingleton().create( "MotionBlur", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	{
		CompositionTechnique * t = comp3->createTechnique();
		{
			CompositionTechnique::TextureDefinition * def = t->createTextureDefinition( "scene" );
			def->width = 0;
			def->height = 0;
			def->formatList.push_back( PF_R8G8B8 );
		}
		{
			CompositionTechnique::TextureDefinition * def = t->createTextureDefinition( "sum" );
			def->width = 0;
			def->height = 0;
			def->formatList.push_back( PF_R8G8B8 );
		}
		{
			CompositionTechnique::TextureDefinition * def = t->createTextureDefinition( "temp" );
			def->width = 0;
			def->height = 0;
			def->formatList.push_back( PF_R8G8B8 );
		}
		/// Render scene
		{
			CompositionTargetPass * tp = t->createTargetPass();
			tp->setInputMode( CompositionTargetPass::IM_PREVIOUS );
			tp->setOutputName( "scene" );
		}
		/// Initialisation pass for sum texture
		{
			CompositionTargetPass * tp = t->createTargetPass();
			tp->setInputMode( CompositionTargetPass::IM_PREVIOUS );
			tp->setOutputName( "sum" );
			tp->setOnlyInitial( true );
		}
		/// Do the motion blur
		{
			CompositionTargetPass * tp = t->createTargetPass();
			tp->setInputMode( CompositionTargetPass::IM_NONE );
			tp->setOutputName( "temp" );
			{
				CompositionPass * pass = tp->createPass();
				pass->setType( CompositionPass::PT_RENDERQUAD );
				pass->setMaterialName( "Ogre/Compositor/Combine" );
				pass->setInput( 0, "scene" );
				pass->setInput( 1, "sum" );
			}
		}
		/// Copy back sum texture
		{
			CompositionTargetPass * tp = t->createTargetPass();
			tp->setInputMode( CompositionTargetPass::IM_NONE );
			tp->setOutputName( "sum" );
			{
				CompositionPass * pass = tp->createPass();
				pass->setType( CompositionPass::PT_RENDERQUAD );
				pass->setMaterialName( "Ogre/Compositor/Copyback" );
				pass->setInput( 0, "temp" );
			}
		}
		/// Display result
		{
			CompositionTargetPass * tp = t->getOutputTargetPass();
			tp->setInputMode( CompositionTargetPass::IM_NONE );
			{
				CompositionPass * pass = tp->createPass();
				pass->setType( CompositionPass::PT_RENDERQUAD );
				pass->setMaterialName( "Ogre/Compositor/MotionBlur" );
				pass->setInput( 0, "sum" );
			}
		}
	}
}
