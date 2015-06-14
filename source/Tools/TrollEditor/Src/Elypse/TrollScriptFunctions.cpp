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
#include "TrollEditorPch.h"

#include "Elypse/TrollScriptFunctions.h"

#include "Project/2D/Overlay.h"
#include "Project/3D/Entity.h"
#include "Project/3D/SceneNode.h"
#include "Project/3D/Camera.h"
#include "Project/3D/Light.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"

#include <ElypseResourceGroupManager.h>

BEGIN_TROLL_ELYPSE_NAMESPACE
{
	using namespace TROLL_NAMESPACE;
	using namespace TROLL_GUI_NAMESPACE;
	using namespace TROLL_PROJECT_NAMESPACE;
	using namespace TROLL_PROJECT_3D_NAMESPACE;
	using namespace TROLL_PROJECT_2D_NAMESPACE;
	using namespace Ogre;

	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Ent_SetMaterial )
	{
		VERBOSE_STARTFUNC( " Troll_Entity_SetMaterial " );
		GET_AND_EXEC_PARAM( String, l_materialName, 1 );

		if ( MaterialManager::getSingletonPtr()->getByName( l_materialName ).isNull() == false )
		{
			GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

			if ( l_entity != NULL )
			{
				l_entity->setMaterialName( l_materialName );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetEntity( make_wxString( l_entity->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetEntity( make_wxString( l_entity->getName() ) )->SetMaterial( make_wxString( l_materialName ), false );
				}
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_Entity_SetMaterial -> no object" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Entity_SetMaterial -> no such material exists (" + l_materialName + ")" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetHeight )
	{
		GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, l_height );

		if ( p_overlay != NULL )
		{
			p_overlay->GetOgreOverlayElement()->setHeight( l_height / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetHeight( l_height / ScriptEngine::GetContext()->mainViewport->getActualHeight(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetHeight : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetWidth )
	{
		GET_AND_EXEC_TWO_PARAM( EMOverlay *, p_overlay, Real, l_width );

		if ( p_overlay != NULL )
		{
			p_overlay->GetOgreOverlayElement()->setWidth( l_width / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetWidth( l_width / ScriptEngine::GetContext()->mainViewport->getActualWidth(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetWidth : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetCaption )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetCaption" );
		GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

		if ( p_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( String, l_caption, 1 );
			p_overlay->GetOgreOverlayElement()->setCaption( ToWString( l_caption ) );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetCaption( make_wxString( l_caption ), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetCaption : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetMouseOverMaterial )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetMaterial" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( String, l_materialName, 1 );

			if ( MaterialManager::getSingletonPtr()->getByName( l_materialName ).isNull() == false )
			{
				l_overlay->SetMouseOverMaterial( l_materialName );
				TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( l_overlay->GetName() ) );

				if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
				{
					l_teOv->SetMouseOverMaterial( make_wxString( l_materialName ), false );
				}
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_OvE_SetMouseOverMaterial : material " + l_materialName + "does not exist" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetMouseOverMaterial : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetMaterial )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetMaterial" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( String, l_materialName, 1 );

			if ( ! MaterialManager::getSingletonPtr()->getByName( l_materialName ).isNull() )
			{
				l_overlay->SetBaseMaterial( l_materialName );
				TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( l_overlay->GetName() ) );

				if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
				{
					l_teOv->SetBaseMaterial( make_wxString( l_materialName ), false );
				}
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_OvE_SetMaterial : material " + l_materialName + "does not exist" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetMaterial : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetBorderMaterial )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetBorderMaterial" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( String, l_materialName, 1 );

			if ( ! MaterialManager::getSingletonPtr()->getByName( l_materialName ).isNull() )
			{
				( static_cast <BorderPanelOverlayElement *>( l_overlay->GetOgreOverlayElement() ) )->setBorderMaterialName( l_materialName.c_str() );
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_OvE_SetBorderMaterial : material [" + l_materialName + "] does not exist" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetBorderMaterial : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetTextColour )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetTextColour" );
		GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

		if ( p_overlay != NULL )
		{
			OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();

			if ( l_overlay->getTypeName() == "TextArea" )
			{
				GET_AND_EXEC_PARAM( ColourValue, l_colour, 1 );
				TextAreaOverlayElement * l_over = static_cast <TextAreaOverlayElement *>( l_overlay );
				l_over->setColourBottom( l_colour );
				l_over->setColourTop( l_colour );
				TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

				if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
				{
					l_teOv->SetTextColour( l_colour, false );
				}
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_OvE_SetTextColour : OverlayElement [" + l_overlay->getName() + "] is not a TextArea" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetTextColour : OverlayElement null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_Resize )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_Resize" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( Real, x, 1 );
			GET_AND_EXEC_PARAM( Real, y, 2 );
			l_overlay->GetOgreOverlayElement()->setDimensions( l_overlay->GetOgreOverlayElement()->getWidth() + ( x / ScriptEngine::GetContext()->mainViewport->getActualWidth() ),
					l_overlay->GetOgreOverlayElement()->getHeight() + ( y / ScriptEngine::GetContext()->mainViewport->getActualHeight() ) );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( l_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetHeight( l_overlay->GetOgreOverlayElement()->getHeight(), false );
				l_teOv->SetWidth( l_overlay->GetOgreOverlayElement()->getWidth(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_Resize : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_Translation )
	{
		VERBOSE_STARTFUNC( "Troll_OverlayElement_Translation" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( Real, x, 1 );
			GET_AND_EXEC_PARAM( Real, y, 2 );
			l_overlay->GetOgreOverlayElement()->setPosition( l_overlay->GetOgreOverlayElement()->getLeft() + x / ScriptEngine::GetContext()->mainViewport->getActualWidth(),
					l_overlay->GetOgreOverlayElement()->getTop() + y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( l_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetTop( l_overlay->GetOgreOverlayElement()->getTop(), false );
				l_teOv->SetLeft( l_overlay->GetOgreOverlayElement()->getLeft(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OverlayElement_Translation : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetSize )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetSize" );
		GET_AND_EXEC_PARAM( EMOverlay *, l_overlay, 0 );

		if ( l_overlay != NULL )
		{
			GET_AND_EXEC_PARAM( Real, x, 1 );
			GET_AND_EXEC_PARAM( Real, y, 2 );
			l_overlay->GetOgreOverlayElement()->setDimensions( x / ScriptEngine::GetContext()->mainViewport->getActualWidth(),
					y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( l_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetHeight( y / ScriptEngine::GetContext()->mainViewport->getActualHeight(), false );
				l_teOv->SetWidth( x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_Resize : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetPosV3 )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetPosV3" );
		GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

		if ( p_overlay != NULL )
		{
			OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();
			GET_AND_EXEC_PARAM( Vector3, l_vpos, 1 );
			l_overlay->setPosition( l_vpos.x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), l_vpos.y / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetTop( l_vpos.y / ScriptEngine::GetContext()->mainViewport->getActualHeight(), false );
				l_teOv->SetLeft( l_vpos.x / ScriptEngine::GetContext()->mainViewport->getActualWidth(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetPosV3 : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetPosX )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetPosX" );
		GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

		if ( p_overlay != NULL )
		{
			OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();
			GET_AND_EXEC_PARAM( Real, posX, 1 );
			l_overlay->setLeft( posX / ScriptEngine::GetContext()->mainViewport->getActualWidth() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetLeft( posX / ScriptEngine::GetContext()->mainViewport->getActualWidth(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetPosV3 : overlay null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_OvE_SetPosY )
	{
		VERBOSE_STARTFUNC( "Troll_OvE_SetPosY" );
		GET_AND_EXEC_PARAM( EMOverlay *, p_overlay, 0 );

		if ( p_overlay != NULL )
		{
			OverlayElement * l_overlay = p_overlay->GetOgreOverlayElement();
			GET_AND_EXEC_PARAM( Real, posY, 1 );
			l_overlay->setTop( posY / ScriptEngine::GetContext()->mainViewport->getActualHeight() );
			TrollOverlay * l_teOv = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetOverlay( make_wxString( p_overlay->GetName() ) );

			if ( GUI::MainFrame::GetInstance()->IsEditingOverlays() && l_teOv != NULL )
			{
				l_teOv->SetTop( posY / ScriptEngine::GetContext()->mainViewport->getActualHeight(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_OvE_SetPosV3 : overlay null" );
		}
	}




	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_RotateAround )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_RotateAround" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_rotationCenter, 1 );
			GET_AND_EXEC_PARAM( Vector3, l_sAxis, 2 );
			GET_AND_EXEC_PARAM( Real, l_angle, 3 );
			const Vector3 & l_axis = l_sAxis.normalisedCopy();

			if ( l_angle != 0.0 )
			{
				Quaternion l_quat = Quaternion( Degree( l_angle ), l_axis );
				l_node->rotate( l_quat, Node::TS_WORLD );
				const Vector3 & l_oa = l_node->_getDerivedPosition() - l_rotationCenter;
				l_node->translate( l_quat * l_oa - l_oa );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_RotateAround : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Rotate )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Rotate" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Real, l_angle, 2 );

			if ( l_angle != 0.0 )
			{
				GET_AND_EXEC_PARAM( Vector3, l_sAxis, 1 );
				const Vector3 & Axis = l_sAxis.normalisedCopy();
				l_node->rotate( Axis, Degree( l_angle ), Node::TS_WORLD );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Rotate : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_RotateLocal )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_RotateLocal" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Real, l_angle, 2 );

			if ( l_angle != 0.0 )
			{
				GET_AND_EXEC_PARAM( Vector3, l_sAxis, 1 );
				const Vector3 & Axis = l_sAxis.normalisedCopy();
				l_node->rotate( Axis, Degree( l_angle ), Node::TS_LOCAL );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_RotateLocal : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_RotateQuat )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_RotateQuat" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Quaternion, l_quat, 1 );
			l_node->rotate( l_quat, Node::TS_WORLD );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_RotateQuat : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Pitch )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Pitch" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Real, l_angle, 1 );

			if ( l_angle != 0.0 )
			{
				Matrix3 l_m;
				l_node->getOrientation().ToRotationMatrix( l_m );
				l_node->rotate( l_node->getLocalAxes().GetColumn( 0 ), Degree( l_angle ), Node::TS_WORLD );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Pitch : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Yaw )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Yaw" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Real, l_angle, 1 );

			if ( l_angle != 0.0 )
			{
				l_node->yaw( Degree( l_angle ), Node::TS_WORLD );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Yaw : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Translate )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Translate" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_translation, 1 );

			if ( l_translation != Vector3::ZERO )
			{
				l_node->translate( l_translation, Node::TS_WORLD );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Translate : Node null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_TranslateLocal )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_TranslateLocal" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_translation, 1 );

			if ( l_translation != Vector3::ZERO )
			{
				l_node->translate( l_translation, Node::TS_LOCAL );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_TranslateLocal : Node null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Scale )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Scale" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_scale, 1 );

			if ( l_scale != Vector3( 1.0, 1.0, 1.0 ) )
			{
				l_node->scale( l_scale.x, l_scale.y, l_scale.z );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetScale( l_node->getScale(), false );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Scale : Node null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetScale )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_SetScale" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_scale, 1 );
			l_node->setScale( l_scale.x, l_scale.y, l_scale.z );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetScale( l_scale, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_SetScale : Node null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetPosition )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_SetPosition" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_position, 1 );
			l_node->setPosition( l_position );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SetPosition : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetXPosition )
	{
		GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

		if ( l_node != NULL )
		{
			const Vector3 & l_pos = l_node->getPosition();
			l_node->setPosition( l_position, l_pos.y , l_pos.z );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_SetXPosition : no node" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetYPosition )
	{
		GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

		if ( l_node != NULL )
		{
			const Vector3 & l_pos = l_node->getPosition();
			l_node->setPosition( l_pos.x, l_position, l_pos.z );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_SetYPosition : no node" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetZPosition )
	{
		GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

		if ( l_node != NULL )
		{
			const Vector3 & l_pos = l_node->getPosition();
			l_node->setPosition( l_pos.x, l_pos.y, l_position );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetPosition( l_node->getPosition(), false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_SetZPosition : no node" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_Reset )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_Reset" );
		GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

		if ( l_node != NULL )
		{
			l_node->setOrientation( Quaternion::IDENTITY );
			l_node->setScale( Vector3::UNIT_SCALE );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetScale( Vector3::UNIT_SCALE, false );
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( Quaternion::IDENTITY, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_Reset : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_ScN_SetOrientation )
	{
		VERBOSE_STARTFUNC( "Troll_SceneNode_SetOrientation" );
		GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Quaternion, l_quat );

		if ( l_node != NULL )
		{
			Vector3 l_axis;
			Radian l_angle;
			l_quat.ToAngleAxis( l_angle, l_axis );

			if ( l_axis != Vector3::ZERO )
			{
				Quaternion q = l_quat;
				q.normalise();
				l_node->setOrientation( q );

				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetSceneNode( make_wxString( l_node->getName() ) )->SetOrientation( l_node->getOrientation(), false );
				}
			}
			else
			{
				SCRIPT_ERROR( "Error @ Troll_SceneNode_SetOrientation : Orientation null" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_SceneNode_SetOrientation : TrollSceneNode null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Cam_ModeWireFrame )
	{
		VERBOSE_STARTFUNC( "Troll_Camera_ModeWireFrame" );
		GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, bool, l_bool );

		if ( l_camera != NULL )
		{
			if ( l_bool )
			{
				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) )->SetPolygonMode( tcWireFrame );
				}
			}
			else
			{
				if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) ) != NULL )
				{
					GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) )->SetPolygonMode( tcSolid );
				}
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Camera_ModeWireFrame : no camera." );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Cam_SetPosition )
	{
		VERBOSE_STARTFUNC( "Troll_Camera_SetPosition" );
		GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, Vector3, l_position );

		if ( l_camera != NULL )
		{
			l_camera->setPosition( l_position );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetCamera( make_wxString( l_camera->getName() ) )->SetPosition( l_position, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Camera_SetPosition : no camera." );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Lgh_SetDirection )
	{
		VERBOSE_STARTFUNC( "Troll_Light_SetDirection" );
		GET_AND_EXEC_PARAM( Light *, l_light, 0 );

		if ( l_light != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_sLightDirection, 1 );
			l_light->setDirection( l_sLightDirection );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_light->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_light->getName() ) )->SetDirection( l_sLightDirection, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Light_SetDirection : Light null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Lgh_SetPosition )
	{
		VERBOSE_STARTFUNC( "Troll_Light_SetPosition" );
		GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

		if ( l_sLight != NULL )
		{
			GET_AND_EXEC_PARAM( Vector3, l_sLightPosition, 1 );
			l_sLight->setPosition( l_sLightPosition );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) )->SetPosition( l_sLightPosition, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Light_SetPosition : Light null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Lgh_SetDiffuse )
	{
		VERBOSE_STARTFUNC( "Troll_Light_SetDiffuse" );
		GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

		if ( l_sLight != NULL )
		{
			GET_AND_EXEC_PARAM( ColourValue, l_sLightDiffuse, 1 );
			l_sLight->setDiffuseColour( l_sLightDiffuse );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) )->SetDiffuse( l_sLightDiffuse, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Light_SetDiffuse : Light null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Lgh_SetSpecular )
	{
		VERBOSE_STARTFUNC( "Troll_Light_SetSpecular" );
		GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

		if ( l_sLight != NULL )
		{
			GET_AND_EXEC_PARAM( ColourValue, l_sLightSpecular, 1 );
			l_sLight->setSpecularColour( l_sLightSpecular );

			if ( GUI::MainFrame::GetInstance()->IsEditingScene() && GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) ) != NULL )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetLight( make_wxString( l_sLight->getName() ) )->SetSpecular( l_sLightSpecular, false );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Troll_Light_SetSpecular : Light null" );
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Spc_SaveScript )
	{
		if ( GUI::MainFrame::GetInstance()->GetCurrentProject() != NULL )
		{
			GUI::MainFrame::GetInstance()->GetCurrentProject()->Write();
		}
	}



	TROLL_SCRIPT_FUNCTION_DECLARE( Troll_Spc_ImportMesh )
	{
		std::cout << "Special_ImportMesh\n";
		VERBOSE_STARTFUNC( "Special_ImportMesh" );
		String l_meshName;
		String l_path;
		String l_entName;
		String l_dir;
		SceneNode * l_node;
		Entity * l_entity;
		int l_objNum;

		if ( ScriptEngine::GetContext()->lastDataRep.empty() )
		{
			l_dir = ScriptEngine::GetContext()->sceneFileParser->m_context->baseDirectory;
		}
		else
		{
			l_dir = ScriptEngine::GetContext()->lastDataRep;
		}

		wxFileDialog l_dialog( NULL, wxT( "Choisissez un mesh a ouvrir" ), wxEmptyString, wxEmptyString,
							   wxT( "Mesh Files (*.mesh)|*.mesh|" ),
							   wxFD_OPEN | wxFD_FILE_MUST_EXIST );

		if ( l_dialog.ShowModal() == wxID_OK )
		{
			l_path = make_string( l_dialog.GetPath() );
			unsigned int l_index = l_path.find_last_of( wxFileName::GetPathSeparator() );
			l_meshName = l_path.substr( l_index + 1 );
			ScriptEngine::GetContext()->lastDataRep = l_path.substr( 0, l_index );
			l_entName = l_meshName.substr( 0, l_meshName.length() - 5 );
			l_objNum = 1;

			if ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName ) )
			{
				while ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_entName + String( "_" ) + StringConverter::toString( l_objNum ) ) )
				{
					l_objNum++;
				}

				l_entName = l_entName + String( "_" ) + StringConverter::toString( l_objNum );
			}

			File * l_file = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetDataFile( 0 );
			l_index = ScriptEngine::GetContext()->lastDataRep.find_last_of( wxFileName::GetPathSeparator() );
			String l_folder = ScriptEngine::GetContext()->lastDataRep.substr( l_index + 1 );
			ElypseResourceGroupManager::addResourceLocation( ScriptEngine::GetContext()->lastDataRep, "FileSystem", l_folder );
			ElypseResourceGroupManager::initialiseResourceGroup( l_folder );
			String l_source = ScriptEngine::GetContext()->lastDataRep + make_string( wxString( wxFileName::GetPathSeparator() ) ) + l_meshName;
			String l_destination = make_string( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetPath() + l_file->FileName + wxFileName::GetPathSeparator() ) + l_meshName;

			if ( ! FileCopy( l_source, l_destination ) )
			{
				std::cout << "Can't copy file " << l_meshName << "\n";
			}

			l_entity = ScriptEngine::GetContext()->sceneManager->createEntity( l_entName, l_meshName );
			l_node = ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->createChildSceneNode( l_entName );
			l_node->attachObject( l_entity );
			Mesh * l_mesh = l_entity->getMesh().getPointer();
			unsigned short l_nbSubmeshes = l_mesh->getNumSubMeshes();
			unsigned short l_nbTechniques, l_nbPasses, l_nbTextureUnits;
			String l_fileName;
			String l_materialName;
			SubMesh * l_submesh;
			Material * l_material;
			Technique * l_technique;
			Pass * l_pass;
			TextureUnitState * l_textureUnit;

			for ( unsigned short i = 0 ; i < l_nbSubmeshes ; i++ )
			{
				l_submesh = l_mesh->getSubMesh( i );
				l_materialName = l_submesh->getMaterialName();
				l_material = reinterpret_cast <Material *>( MaterialManager::getSingletonPtr()->getByName( l_materialName ).getPointer() );

				if ( l_material != NULL )
				{
					if ( l_material->getOrigin() != l_fileName )
					{
						l_fileName = l_material->getOrigin();
						String l_source = ScriptEngine::GetContext()->lastDataRep + "/" + l_fileName;
						String l_destination = make_string( wxString( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetPath() ) << l_file->FileName << wxString( wxFileName::GetPathSeparator() ) ) + l_fileName;

						if ( ! FileCopy( l_source, l_destination ) )
						{
							std::cout << "Can't copy file " << l_fileName << "\n";
						}
					}

					l_nbTechniques = l_material->getNumTechniques();

					for ( unsigned short j = 0 ; j < l_nbTechniques ; j++ )
					{
						l_technique = l_material->getTechnique( j );
						l_nbPasses = l_technique->getNumPasses();

						for ( unsigned short k = 0 ; k < l_nbPasses ; k++ )
						{
							l_pass = l_technique->getPass( k );
							l_nbTextureUnits = l_pass->getNumTextureUnitStates();

							for ( unsigned short l = 0 ; l < l_nbTextureUnits ; l++ )
							{
								l_textureUnit = l_pass->getTextureUnitState( l );
								String l_source = ScriptEngine::GetContext()->lastDataRep + "/" + l_textureUnit->getTextureName();
								String l_destination = make_string( wxString( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetPath() ) << l_file->FileName << wxString( wxFileName::GetPathSeparator() ) ) + l_textureUnit->getTextureName();

								if ( ! FileCopy( l_source, l_destination ) )
								{
									std::cout << "Can't copy file " << l_textureUnit->getTextureName() << "\n";
								}
							}
						}
					}
				}
			}

			RETURN_AS( Entity * ) l_entity;
		}
		else
		{
			RETURN_AS( Entity * ) NULL;
		}
	}
}
END_TROLL_ELYPSE_NAMESPACE
