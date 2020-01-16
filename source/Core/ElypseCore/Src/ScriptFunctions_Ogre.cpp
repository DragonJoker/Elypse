/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptFunctions.h"
#include "ScriptNode.h"
#include "ScriptEngine.h"

#include "WebImageManager.h"
#include "WebImage.h"
#include "MaterialTarget.h"

#include "Context.h"

#include "PhysicsObject.h"
#include "PhysicsSimulation.h"

#include <OgreSkeletonManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_ShowBoundingBox )
{
	VERBOSE_STARTFUNC( " Entity_ShowBoundingBox " );
	GET_AND_EXEC_TWO_PARAM( Entity *, l_entity, bool, l_show );

	if ( l_entity != NULL )
	{
		l_entity->getParentSceneNode()->showBoundingBox( l_show );
	}
	else
	{
		SCRIPT_ERROR( "Error @ ShowBoundingBox : Entity null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetParent )
{
	VERBOSE_STARTFUNC( " Entity_GetParent " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != NULL )
	{
		RETURN_AS( SceneNode * ) l_entity->getParentSceneNode();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_GetParent : Entity NULL" );
		RETURN_AS( SceneNode * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetBoundingRadius )
{
	VERBOSE_STARTFUNC( " Entity_GetBoundingRadius " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != NULL )
	{
		RETURN_AS( Real ) l_entity->getBoundingRadius();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_GetBoundingRadius : Entity NULL" );
		RETURN_AS( Real ) 0.0;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_SetMaterial )
{
	VERBOSE_STARTFUNC( " Entity_SetMaterial " );
	GET_AND_EXEC_TWO_PARAM( Entity *, p_entity, String, p_materialName );

	if ( !MaterialManager::getSingletonPtr()->getByName( p_materialName ) )
	{
		return SCRIPT_ERROR( "Error @ Entity_SetMaterial -> no such material exists (" + p_materialName + ")" );
	}

	if ( p_entity == NULL )
	{
		return SCRIPT_ERROR( "Error @ Entity_SetMaterial -> no object" );
	}

	WebImage * l_image = WebImageManager::GetSingletonPtr()->GetWebImage( p_materialName );

	if ( l_image == NULL || l_image->IsReady() )
	{
		p_entity->setMaterialName( p_materialName );
	}
	else
	{
		l_image->AddTarget( new MaterialTarget_Entity( p_entity ) );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_SetSubMaterial )
{
	VERBOSE_STARTFUNC( " Entity_SetSubMaterial " );
	GET_AND_EXEC_PARAM( String, l_materialName, 2 );

	if ( MaterialManager::getSingletonPtr()->getByName( l_materialName ) )
	{
		GET_AND_EXEC_TWO_PARAM( Entity *, l_entity, int, l_index );

		if ( l_entity != NULL )
		{
			if ( l_index >= 0 && static_cast <size_t>( l_index ) < ( l_entity->getNumSubEntities() ) )
			{
				l_entity->getSubEntity( static_cast <uint32_t>( l_index ) )->setMaterialName( l_materialName );
			}
			else
			{
				SCRIPT_ERROR( "Error @ Entity_SetSubMaterial -> index out of bounds" );
			}
		}
		else
		{
			SCRIPT_ERROR( "Error @ Entity_SetSubMaterial -> no object" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_SetSubMaterial -> no such material exists (" + l_materialName + ")" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetByName )
{
	VERBOSE_STARTFUNC( " Entity_GetByName " );
	GET_AND_EXEC_PARAM( String, l_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasEntity( l_name ) )
	{
		RETURN_AS( Entity * ) ScriptEngine::GetContext()->sceneManager->getEntity( l_name );
	}
	else
	{
//		SCRIPT_ERROR( "Error @ GetEntity -> no such entity exists (" + l_name + ")");
		RETURN_AS( Entity * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_Show )
{
	VERBOSE_STARTFUNC( " Entity_Show " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity == NULL )
	{
		return SCRIPT_ERROR( "Error @ Entity_Show -> no entity" );
	}

	l_entity->setVisible( true );

	if ( l_entity->getUserObjectBindings().getUserAny().has_value() )
	{
		PhysicsObject * l_object = Ogre::any_cast< PhysicsObject * > ( l_entity->getUserObjectBindings().getUserAny() );

		if ( l_object )
		{
			l_object->SetEnabled( true );
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_Hide )
{
	VERBOSE_STARTFUNC( " Entity_Hide " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity == NULL )
	{
		return SCRIPT_ERROR( "Error @ Entity_Hide -> no entity" );
	}

	l_entity->setVisible( false );

	if ( l_entity->getUserObjectBindings().getUserAny().has_value() )
	{
		PhysicsObject * l_object = Ogre::any_cast< PhysicsObject * > ( l_entity->getUserObjectBindings().getUserAny() );

		if ( l_object )
		{
			l_object->SetEnabled( false );
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_SetVisible )
{
	VERBOSE_STARTFUNC( " Entity_SetVisible " );
	GET_AND_EXEC_TWO_PARAM( Entity *, l_entity, bool, p_visible );

	if ( l_entity == NULL )
	{
		return SCRIPT_ERROR( "Error @ Entity_Hide -> no entity" );
	}

	l_entity->setVisible( p_visible );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_IsVisible )
{
	VERBOSE_STARTFUNC( "Entity_IsVisible" );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity == NULL )
	{
		RETURN_AS( bool ) false;
		return SCRIPT_ERROR( "Error @ Entity_Hide -> no entity" );
	}

	RETURN_AS( bool ) l_entity->isVisible();
}



EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetName )
{
	VERBOSE_STARTFUNC( " Entity_GetName " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != NULL )
	{
		RETURN_AS( String ) l_entity->getName();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_GetName -> no entity" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetMeshName )
{
	VERBOSE_STARTFUNC( " Object_GetMeshName " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != NULL )
	{
		RETURN_AS( String ) l_entity->getMesh()->getName();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Object_GetMeshName -> no entity" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetMaterialName )
{
	VERBOSE_STARTFUNC( " Entity_GetMaterialName " );
	GET_AND_EXEC_PARAM( Entity *, l_entity, 0 );

	if ( l_entity != NULL )
	{
		RETURN_AS( String ) l_entity->getSubEntity( 0 )->getMaterialName();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_GetMaterialName -> no entity" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetSubMaterialName )
{
	VERBOSE_STARTFUNC( " Entity_GetSubMaterialName " );
	GET_AND_EXEC_TWO_PARAM( Entity *, l_entity, int, l_index );

	if ( l_entity != NULL )
	{
		if ( static_cast <size_t>( l_index ) < l_entity->getNumSubEntities() )
		{
			RETURN_AS( String ) l_entity->getSubEntity( static_cast <uint32_t>( l_index ) )->getMaterialName();
		}
		else
		{
			SCRIPT_ERROR( "Error @ Entity_GetSubMaterialName -> index out of bounds for object : " + l_entity->getName() + String( " // index : " + StringConverter::toString( l_index ) ) );
			RETURN_AS( String ) EMPTY_STRING;
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_GetSubMaterialName -> no entity" );
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_Create )
{
	VERBOSE_STARTFUNC( " Entity_Create " );
	GET_AND_EXEC_TWO_PARAM( String, l_objectName, String, l_meshName );

	try
	{
		RETURN_AS( Entity * ) ScriptEngine::GetContext()->sceneManager->createEntity( l_objectName, l_meshName );
	}
	catch ( ... )
	{
		SCRIPT_ERROR( "Error @ Entity_Create -> Mesh " + l_meshName + " does not exist or entity " + l_objectName + " already exists" );
		RETURN_AS( Entity * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_CreateWithSkeleton )
{
	VERBOSE_STARTFUNC( " Entity_CreateWithSkeleton " );
	GET_AND_EXEC_THREE_PARAM( String, l_objectName, String, l_meshName, String, l_skeletonName );
	MeshPtr l_mesh;

	try
	{
		l_mesh = MeshManager::getSingletonPtr()->load( l_meshName, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	}
	catch ( Ogre::Exception & )
	{
		SCRIPT_ERROR( "Error @ Entity_CreateWithSkeleton -> mesh named [" + l_meshName + "] does not exist" );
		RETURN_AS( Entity * ) NULL;
		return;
	}

	if ( !SkeletonManager::getSingletonPtr()->getByName( l_skeletonName ) )
	{
		StringVector l_groups = ResourceGroupManager::getSingletonPtr()->getResourceGroups();
		Ogre::StringVectorPtr l_names;
		bool l_found = false;

		for ( size_t i = 0 ; i < l_groups.size() && ! l_found ; i++ )
		{
			l_names = ResourceGroupManager::getSingletonPtr()->listResourceNames( l_groups[i] );

			for ( size_t j = 0 ; j < l_names.get()->size() ; j++ )
			{
				if ( l_names.get()->operator []( j ) == l_skeletonName )
				{
					l_found = true;
				}
			}
		}

		if ( ! l_found )
		{
			SCRIPT_ERROR( "Error @ Entity_CreateWithSkeleton -> skeleton (" + l_skeletonName + ") not found" );
			RETURN_AS( Entity * ) NULL;
			return;
		}
	}

	size_t l_index = l_meshName.find_last_of( "." );
	l_meshName = l_meshName.substr( 0, l_index );
	l_index = l_skeletonName.find_last_of( "." );
	String l_skelName2 = l_skeletonName.substr( 0, l_index );
	l_mesh = l_mesh->clone( l_meshName + "_" + l_skelName2, l_mesh->getGroup() );
	l_mesh->load();
	l_mesh->setSkeletonName( l_skeletonName );
	RETURN_AS( Entity * ) ScriptEngine::GetContext()->sceneManager->createEntity( l_objectName, l_mesh->getName() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetPhysicsObject )
{
	VERBOSE_STARTFUNC( " Ent_GetPhysicsObject " );
	GET_AND_EXEC_PARAM( Entity *, l_object, 0 );

	if ( l_object != NULL )
	{
		RETURN_AS( PhysicsObject * ) Ogre::any_cast< PhysicsObject * >( l_object->getUserObjectBindings().getUserAny() );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_Destroy )
{
	VERBOSE_STARTFUNC( " Entity_Destroy " );
	GET_AND_EXEC_PARAM( Entity *, l_object, 0 );

	if ( l_object != NULL )
	{
		ScriptEngine::GetContext()->sceneManager->destroyEntity( l_object );
		l_object = NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_SetParent )
{
	VERBOSE_STARTFUNC( " Entity_SetParent " );
	GET_AND_EXEC_TWO_PARAM( Entity *, l_object, SceneNode *, l_node );

	if ( l_object == NULL )
	{
		return SCRIPT_ERROR( "Error @ Entity_SetParent -> Entity is null" );
	}

	if ( l_node )
	{
		if ( l_object->getParentSceneNode() != NULL )
		{
			l_object->getParentSceneNode()->detachObject( l_object->getName() );
		}

		l_node->attachObject( l_object );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Entity_SetParent -> Parent node is null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_CopyPhysicsObject )
{
	VERBOSE_STARTFUNC( " Ent_CopyPhysicsObject " );
	GET_AND_EXEC_TWO_PARAM( Entity *, l_object, PhysicsObject *, l_pobj );

	if ( l_object == NULL )
	{
		return SCRIPT_ERROR( "Error @ Ent_CopyPhysicsObject -> Object is null" );
	}

	if ( l_object->getUserObjectBindings().getUserAny().has_value() )
	{
		return SCRIPT_ERROR( "Error @ Ent_CopyPhysicsObject -> Object already has a physics object" );
	}

	if ( ! l_object->isAttached() )
	{
		return SCRIPT_ERROR( "Error @ Ent_CopyPhysicsObject -> Object must be attached" );
	}

	if ( l_pobj == NULL )
	{
		return SCRIPT_ERROR( "Error @ Ent_CopyPhysicsObject -> PhysicsObject is null" );
	}

	PhysicsObject * l_clonedObject = l_pobj->Clone( l_object->getName(), ScriptEngine::GetContext()->physicsSimulation->GetStaticSpace() );
	l_clonedObject->SetNode( l_object->getParentSceneNode() );
	l_object->getUserObjectBindings().setUserAny( Ogre::Any( l_clonedObject ) );
	RETURN_AS( PhysicsObject * ) l_clonedObject;
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Ent_GetNumSubMaterials )
{
	VERBOSE_STARTFUNC( " Ent_GetNumSubMaterials " );
	GET_AND_EXEC_PARAM( Entity *, l_object, 0 );

	if ( l_object == NULL )
	{
		return SCRIPT_ERROR( "Error @ Ent_GetNumSubMaterials -> Object is null" );
	}

	RETURN_AS( int ) int( l_object->getSubEntities().size() );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetByName )
{
	VERBOSE_STARTFUNC( "SceneNode_GetByName" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );

	if ( l_name == "root node" )
	{
		RETURN_AS( SceneNode * ) ScriptEngine::GetContext()->sceneManager->getRootSceneNode();
	}
	else
	{
		if ( ScriptEngine::GetContext()->sceneManager->hasSceneNode( l_name ) )
		{
			RETURN_AS( SceneNode * ) ScriptEngine::GetContext()->sceneManager->getSceneNode( l_name );
		}
		else
		{
			RETURN_AS( SceneNode * ) NULL;
		}
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Exists )
{
	VERBOSE_STARTFUNC( "ScN_Exists" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );
	RETURN_AS( bool ) ScriptEngine::GetContext()->sceneManager->hasSceneNode( l_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Create )
{
	VERBOSE_STARTFUNC( "SceneNode_Create" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasSceneNode( l_name ) )
	{
		SCRIPT_ERROR( "Error @ SceneNode_Create : Node alraedy exists (" + l_name + ")" );
		RETURN_AS( SceneNode * ) ScriptEngine::GetContext()->sceneManager->getSceneNode( l_name );
		return;
	}

	RETURN_AS( SceneNode * ) ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->createChildSceneNode( l_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Delete )
{
	VERBOSE_STARTFUNC( "SceneNode_Delete" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasSceneNode( l_name ) )
	{
		ScriptEngine::GetContext()->sceneManager->destroySceneNode( l_name );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetName )
{
	VERBOSE_STARTFUNC( "SceneNode_GetName" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( String ) l_node->getName();
	}
	else
	{
		RETURN_AS( String ) EMPTY_STRING;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_RotateAround )
{
	VERBOSE_STARTFUNC( "SceneNode_Rotate" );
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
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_RotateAround : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Rotate )
{
	VERBOSE_STARTFUNC( "SceneNode_Rotate" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Real, l_angle, 2 );

		if ( l_angle != 0.0 )
		{
			GET_AND_EXEC_PARAM( Vector3, l_sAxis, 1 );
			const Vector3 & Axis = l_sAxis.normalisedCopy();
			l_node->rotate( Axis, Degree( l_angle ), Node::TS_WORLD );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Rotate : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_RotateLocal )
{
	VERBOSE_STARTFUNC( "SceneNode_RotateLocal" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Real, l_angle, 2 );

		if ( l_angle != 0.0 )
		{
			GET_AND_EXEC_PARAM( Vector3, l_sAxis, 1 );
			const Vector3 & Axis = l_sAxis.normalisedCopy();
			l_node->rotate( Axis, Degree( l_angle ), Node::TS_LOCAL );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_RotateLocal : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_RotateQuat )
{
	VERBOSE_STARTFUNC( "SceneNode_RotateQuat" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Quaternion, l_quat, 1 );
		l_node->rotate( l_quat, Node::TS_WORLD );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_RotateQuat : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Pitch )
{
	VERBOSE_STARTFUNC( "SceneNode_Pitch" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Real, l_angle, 1 );

		if ( l_angle != 0.0 )
		{
			Matrix3 l_m;
			l_node->getOrientation().ToRotationMatrix( l_m );
			l_node->rotate( l_node->getLocalAxes().GetColumn( 0 ), Degree( l_angle ), Node::TS_WORLD );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Pitch : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetPitch )
{
	VERBOSE_STARTFUNC( "SceneNode_GetPitch" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		Quaternion l_orientation = l_node->_getDerivedOrientation();
		Real l_angle = l_orientation.getPitch( false ).valueDegrees();
		RETURN_AS( Real ) l_angle;
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetPitch : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Yaw )
{
	VERBOSE_STARTFUNC( "SceneNode_Yaw" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Real, l_angle, 1 );

		if ( l_angle != 0.0 )
		{
			l_node->yaw( Degree( l_angle ), Node::TS_WORLD );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Yaw : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetYaw )
{
	VERBOSE_STARTFUNC( "SceneNode_GetYaw" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		Quaternion l_orientation = l_node->_getDerivedOrientation();
		Real l_angle = l_orientation.getYaw( false ).valueDegrees();
		RETURN_AS( Real ) l_angle;
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetYaw : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Roll )
{
	VERBOSE_STARTFUNC( "SceneNode_Roll" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Real, l_angle, 1 );

		if ( l_angle != 0.0 )
		{
			l_node->roll( Degree( l_angle ), Node::TS_WORLD );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Roll : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetRoll )
{
	VERBOSE_STARTFUNC( "SceneNode_GetRoll" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		Quaternion l_orientation = l_node->_getDerivedOrientation();
		Real l_angle = l_orientation.getRoll( false ).valueDegrees();
		RETURN_AS( Real ) l_angle;
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetRoll : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Translate )
{
	VERBOSE_STARTFUNC( "SceneNode_Translate" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_translation, 1 );

		if ( l_translation != Vector3::ZERO )
		{
			l_node->translate( l_translation, Node::TS_WORLD );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Translate : Node null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_TranslateLocal )
{
	VERBOSE_STARTFUNC( "SceneNode_TranslateLocal" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_translation, 1 );

		if ( l_translation != Vector3::ZERO )
		{
			l_node->translate( l_translation, Node::TS_LOCAL );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_TranslateLocal : Node null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Scale )
{
	VERBOSE_STARTFUNC( "SceneNode_Scale" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_scale, 1 );
		l_node->scale( l_scale );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Scale : Node null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetScale )
{
	VERBOSE_STARTFUNC( "SceneNode_SetScale" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_scale, 1 );
		l_node->setScale( l_scale );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetScale : Node null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetPosition )
{
	VERBOSE_STARTFUNC( "SceneNode_SetPosition" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_position, 1 );
		l_node->setPosition( l_position );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SetPosition : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetXPosition )
{
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

	if ( l_node != NULL )
	{
		const Vector3 & l_pos = l_node->getPosition();
		l_node->setPosition( l_position, l_pos.y , l_pos.z );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Node_SetXPosition : no node" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetYPosition )
{
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

	if ( l_node != NULL )
	{
		const Vector3 & l_pos = l_node->getPosition();
		l_node->setPosition( l_pos.x, l_position, l_pos.z );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Node_SetYPosition : no node" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetZPosition )
{
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, Real, l_position );

	if ( l_node != NULL )
	{
		const Vector3 & l_pos = l_node->getPosition();
		l_node->setPosition( l_pos.x, l_pos.y, l_position );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Node_SetZPosition : no node" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetPosition )
{
	VERBOSE_STARTFUNC( "SceneNode_GetPosition" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( Vector3 ) l_node->getPosition();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetPosition : SceneNode null" );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetDerivedPosition )
{
	VERBOSE_STARTFUNC( "SceneNode_GetDerivedPosition" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( Vector3 ) l_node->_getDerivedPosition();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetDerivedPosition : SceneNode null" );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Reset )
{
	VERBOSE_STARTFUNC( "SceneNode_Reset" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		l_node->setOrientation( Quaternion::IDENTITY );
		l_node->setScale( Vector3::UNIT_SCALE );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Reset : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetParent )
{
	VERBOSE_STARTFUNC( "SceneNode_GetParent" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( SceneNode * ) l_node->getParentSceneNode();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetParent : SceneNode null" );
		RETURN_AS( SceneNode * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Group )
{
	/*
	VERBOSE_STARTFUNC( "SceneNode_Group" )
	ScriptNode* l_sNodeA,*l_sNodeB;
	SceneNode * a, *b;

	GET_AND_EXEC_TWO_PARAM(l_sNodeA,l_sNodeB);

	a = l_sNodeA->m_node;
	b = l_sNodeB->m_node;

	if( a != NULL && b != NULL )
	{

		b->setOrientation(b->_getDerivedOrientation() - a->_getDerivedOrientation());
		b->setScale(b->_getDerivedScale() / a->_getDerivedScale());
		b->setPosition(b->_getDerivedPosition() - a->_getDerivedPosition());

		b->getParentSceneNode()->removeChild(b);

		a->addChild(b);

		b->setInheritOrientation(true);
		b->setInheritScale(true);
	}
	else
	{
		SCRIPT_ERROR("Error @ SceneNode_Group : SceneNode(s) null");
	}
	*/
}


EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Ungroup )
{
	/*
	VERBOSE_STARTFUNC( "SceneNode_Ungroup" )
	ScriptNode* l_sNode;
	GET_AND_EXEC_PARAM(l_sNode,0);
	SceneNode * a, *b;
	b = l_sNode->m_node;
	if(b!= NULL)
	{
		a = b->getParentSceneNode();

		b->setOrientation(b->_getDerivedOrientation());
		b->setScale(b->_getDerivedScale());
		b->setPosition(b->_getDerivedPosition());

		a->removeChild(b);

		ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->addChild(b);

		b->setInheritOrientation(false);
		b->setInheritScale(false);
	}
	else
	{
		SCRIPT_ERROR("Error @ SceneNode_Ungroup : SceneNode null");
	}
	*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_UngroupAll )
{
	/*
	VERBOSE_STARTFUNC( "SceneNode_UngroupAll" )
	ScriptNode* l_sNode;
	GET_AND_EXEC_PARAM(l_sNode,0);
	SceneNode * a, *b;
	b = l_sNode->m_node;

	if(b!= NULL)
	{
		while(b->numChildren()>0)
		{
			SceneNode::ChildNodeIterator iter = b->getChildIterator();

			a = static_cast<SceneNode*>(iter.getNext());

			a->setInheritOrientation(false);
			a->setInheritScale(false);

			a->setOrientation(a->_getDerivedOrientation());
			a->setScale(a->_getDerivedScale());
			a->setPosition(a->_getDerivedPosition());

			b->removeChild(a);

			ScriptEngine::GetContext()->sceneManager->getRootSceneNode()->addChild(a);
			a->showBoundingBox(false);
		}
		b->setOrientation(Quaternion::IDENTITY);
		b->setScale(Vector3::UNIT_SCALE);
	}
	else
	{
		SCRIPT_ERROR("Error @ SceneNode_Ungroup : SceneNode null");
	}*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Recenter )
{
	/*
	VERBOSE_STARTFUNC( "SceneNode_Recenter" )
	ScriptNode* l_sNode;
	GET_AND_EXEC_PARAM(l_sNode,0);
	SceneNode *b;
	b = l_sNode->m_node;
	Vector3 position;

	if(b!= NULL)
	{
		position = Vector3::ZERO;
		Node::ChildNodeIterator iter = b->getChildIterator();
		while(iter.hasMoreElements())
		{
			position += iter.getNext()->getPosition();
		}
		position = position / b->numChildren();
		Node::ChildNodeIterator iter2 = b->getChildIterator();
		while(iter2.hasMoreElements())
		{
			iter2.getNext()->translate(-1.0*position);
		}
		b->translate(position);
	}
	else
	{
		SCRIPT_ERROR("Error @ SceneNode_Ungroup : SceneNode null");
	}
	*/
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_AttachTo )
{
	VERBOSE_STARTFUNC( "SceneNode_AttachTo" );
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_sceneNode, SceneNode *, l_sceneNode2 );

	if ( l_sceneNode != NULL )
	{
		if ( l_sceneNode2 != NULL )
		{
			if ( l_sceneNode->getParentSceneNode() != NULL )
			{
				l_sceneNode->getParentSceneNode()->removeChild( l_sceneNode );
			}

			l_sceneNode2->addChild( l_sceneNode );
		}
		else
		{
			SCRIPT_ERROR( "Error @ SceneNode_AttachTo : Scene node parent Null" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_AttachTo : SceneNode Null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetOrientation )
{
	VERBOSE_STARTFUNC( "SceneNode_GetOrientation" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( Quaternion ) l_node->getOrientation();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetOrientation : SceneNode null" );
		RETURN_AS( Quaternion ) Quaternion::IDENTITY;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetVisible )
{
	VERBOSE_STARTFUNC( "SceneNode_SetVisible" );
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, bool, p_visible );

	if ( l_node != NULL )
	{
		l_node->setVisible( p_visible, true );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetVisible : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Show )
{
	VERBOSE_STARTFUNC( "SceneNode_Show" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		l_node->setVisible( true );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Show : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_Hide )
{
	VERBOSE_STARTFUNC( "SceneNode_Hide" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		l_node->setVisible( false );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_Hide : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetInheritScale )
{
	VERBOSE_STARTFUNC( "SceneNode_SetInheritScale" );
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, bool, p_inherit );

	if ( l_node != NULL )
	{
		l_node->setInheritScale( p_inherit );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetInheritScale : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetInheritOrientation )
{
	VERBOSE_STARTFUNC( "SceneNode_SetInheritOrientation" );
	GET_AND_EXEC_TWO_PARAM( SceneNode *, l_node, bool, p_inherit );

	if ( l_node != NULL )
	{
		l_node->setInheritOrientation( p_inherit );
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetInheritOrientation : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetInheritOrientation )
{
	VERBOSE_STARTFUNC( "SceneNode_GetInheritOrientation" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( bool )l_node->getInheritOrientation();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetInheritOrientation : SceneNode null" );
		RETURN_AS( bool ) false;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_GetInheritScale )
{
	VERBOSE_STARTFUNC( "SceneNode_GetInheritScale" );
	GET_AND_EXEC_PARAM( SceneNode *, l_node, 0 );

	if ( l_node != NULL )
	{
		RETURN_AS( bool )l_node->getInheritScale();
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_GetInheritScale : SceneNode null" );
		RETURN_AS( bool ) false;
	}
}



EMUSE_SCRIPT_FUNCTION_DECLARE( ScN_SetOrientation )
{
	VERBOSE_STARTFUNC( "SceneNode_SetOrientation" );
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
		}
		else
		{
			SCRIPT_ERROR( "Error @ SceneNode_SetOrientation : Orientation null" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetOrientation : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_SetAutoTrack )
{
	VERBOSE_STARTFUNC( "Camera_SetAutoTrack" );
	GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, SceneNode *, l_node );

	if ( l_camera != NULL )
	{
		if ( l_node != NULL )
		{
			l_camera->setAutoTracking( true, l_node );
		}
		else
		{
			SCRIPT_ERROR( "Error @ SceneNode_SetOrientation : Tracked sceneNode null" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ SceneNode_SetOrientation : SceneNode null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetByName )
{
	VERBOSE_STARTFUNC( "Camera_GetByName" );
	GET_AND_EXEC_PARAM( String, l_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasCamera( l_name ) )
	{
		RETURN_AS( Camera * ) ScriptEngine::GetContext()->sceneManager->getCamera( l_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetByName : Camera [" + l_name + "] does not exist." );
		RETURN_AS( Camera * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetLocalXAxis )
{
	VERBOSE_STARTFUNC( "Camera_GetLocalXAxis" );
	GET_AND_EXEC_PARAM( Camera *, l_camera, 0 );

	if ( l_camera != NULL )
	{
		RETURN_AS( Vector3 ) l_camera->getDerivedOrientation() * Vector3::UNIT_X;
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetLocalXAxis : no camera." );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetLocalYAxis )
{
	VERBOSE_STARTFUNC( "Camera_GetLocalYAxis" );
	GET_AND_EXEC_PARAM( Camera *, l_camera, 0 );

	if ( l_camera != NULL )
	{
		RETURN_AS( Vector3 ) l_camera->getDerivedOrientation() * Vector3::UNIT_Y;
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetLocalYAxis : no camera." );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetLocalZAxis )
{
	VERBOSE_STARTFUNC( "Camera_GetLocalZAxis" );
	GET_AND_EXEC_PARAM( Camera *, l_camera, 0 );

	if ( l_camera != NULL )
	{
		RETURN_AS( Vector3 ) l_camera->getDerivedOrientation() * Vector3::UNIT_Z;
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetLocalZAxis : no camera." );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetPosition )
{
	VERBOSE_STARTFUNC( "Camera_GetPosition" );
	GET_AND_EXEC_PARAM( Camera *, l_camera, 0 );

	if ( l_camera != NULL )
	{
		RETURN_AS( Vector3 ) l_camera->getPosition();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetPosition : no camera." );
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_SetPosition )
{
	VERBOSE_STARTFUNC( "Camera_SetPosition" );
	GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, Vector3, l_position );

	if ( l_camera != NULL )
	{
		l_camera->setPosition( l_position );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_SetPosition : no camera." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetParent )
{
	VERBOSE_STARTFUNC( "Camera_GetParent" );
	GET_AND_EXEC_PARAM( Camera *, l_camera, 0 );

	if ( l_camera != NULL )
	{
		RETURN_AS( SceneNode * ) l_camera->getParentSceneNode();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_GetParent : no camera." );
		RETURN_AS( SceneNode * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_AttachTo )
{
	VERBOSE_STARTFUNC( "Camera_AttachTo" );
	GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, SceneNode *, l_node );

	if ( l_camera != NULL )
	{
		if ( l_node != NULL )
		{
			if ( l_camera->isAttached() )
			{
				l_camera->getParentSceneNode()->detachObject( l_camera );
			}

			l_node->attachObject( l_camera );
		}
		else
		{
			SCRIPT_ERROR( "Error @ Camera_AttachTo : no node" );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_AttachTo : no camera" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_ModeWireFrame )
{
	VERBOSE_STARTFUNC( "Camera_ModeWireFrame" );
	GET_AND_EXEC_TWO_PARAM( Camera *, l_camera, bool, l_bool );

	if ( l_camera != NULL )
	{
		if ( l_bool )
		{
			l_camera->setPolygonMode( Ogre::PM_WIREFRAME );
		}
		else
		{
			l_camera->setPolygonMode( Ogre::PM_SOLID );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Camera_ModeWireFrame : no camera." );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_Clamp )
{
	GET_AND_EXEC_THREE_PARAM( Camera *, l_camera, String, l_space, Real, l_sMaxDist );
	Ray mouseRay;

	if ( l_camera == NULL )
	{
		l_camera = ScriptEngine::GetContext()->sceneManager->getCamera( "RootCamera" );
	}

	if ( ! l_camera->isAttached() )
	{
		return;
	}

	mouseRay.setDirection( -l_camera->getDerivedDirection() );
	mouseRay.setOrigin( l_camera->getParentSceneNode()->_getDerivedPosition() );
	DistanceMap l_rayresult = ScriptEngine::GetContext()->physicsSimulation->Raytrace( mouseRay, false, true, false, ScriptEngine::GetContext()->physicsSimulation->GetSpace( l_space ) );
	Real l_dist;
	const DistanceMap::iterator & iend = l_rayresult.end();
	DistanceMap::iterator i = l_rayresult.begin();

	if ( l_rayresult.empty() )
	{
		l_dist = l_sMaxDist;
	}
	else
	{
		l_dist = ( l_sMaxDist < 0.9f * l_rayresult.begin()->first ? l_sMaxDist : 0.9f * l_rayresult.begin()->first );
	}

	l_camera->setPosition( l_camera->getPosition().normalisedCopy() * l_dist );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_GetScreenCoords )
{
	GET_AND_EXEC_TWO_PARAM( Camera *, p_camera, Vector3, p_position );

	if ( p_camera == NULL )
	{
		RETURN_AS( Vector3 ) Vector3::ZERO;
	}
	else
	{
		Vector3 l_position;
		Real x, y;
		l_position = p_camera->getProjectionMatrix() * ( p_camera->getViewMatrix() * ( p_position + Vector3::UNIT_Y ) );
		x = 1.0f - ( ( l_position.x * 0.5f ) + 0.5f );
		y = 1.0f - ( ( l_position.y * 0.5f ) + 0.5f );
		int l_width = int( p_camera->getViewport()->getTarget()->getWidth() );
		int l_height = int( p_camera->getViewport()->getTarget()->getHeight() );
		Real l_left = floor( l_width * x );
		Real l_top = floor( l_height * y );
		RETURN_AS( Vector3 ) Vector3( l_left, l_top, 0 );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Cam_IsPositionVisible )
{
	GET_AND_EXEC_TWO_PARAM( Camera *, p_camera, Vector3, p_position );

	if ( p_camera == NULL )
	{
		RETURN_AS( bool ) false;
	}
	else
	{
		RETURN_AS( bool )( p_camera->getDerivedDirection().dotProduct( p_position - p_camera->getDerivedPosition() ) > 0 );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_GetByName )
{
	VERBOSE_STARTFUNC( "Light_GetByName" );
	GET_AND_EXEC_PARAM( String, l_sLightName, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasLight( l_sLightName ) )
	{
		RETURN_AS( Light * ) ScriptEngine::GetContext()->sceneManager->getLight( l_sLightName );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_GetByName : light [" + l_sLightName + "] does not exist." );
		RETURN_AS( Light * ) NULL;
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_SetDirection )
{
	VERBOSE_STARTFUNC( "Light_SetDirection" );
	GET_AND_EXEC_PARAM( Light *, l_light, 0 );

	if ( l_light != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_sLightDirection, 1 );
		l_light->setDirection( l_sLightDirection );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_SetDirection : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_SetPosition )
{
	VERBOSE_STARTFUNC( "Light_SetPosition" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		GET_AND_EXEC_PARAM( Vector3, l_sLightPosition, 1 );
		l_sLight->setPosition( l_sLightPosition );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_SetPosition : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_SetDiffuse )
{
	VERBOSE_STARTFUNC( "Light_SetDiffuse" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		GET_AND_EXEC_PARAM( ColourValue, l_sLightDiffuse, 1 );
		l_sLight->setDiffuseColour( l_sLightDiffuse );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_SetDiffuse : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_SetSpecular )
{
	VERBOSE_STARTFUNC( "Light_SetSpecular" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		GET_AND_EXEC_PARAM( ColourValue, l_sLightSpecular, 1 );
		l_sLight->setSpecularColour( l_sLightSpecular );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_SetSpecular : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_SetVisible )
{
	VERBOSE_STARTFUNC( "Light_SetVisible" );
	GET_AND_EXEC_TWO_PARAM( Light *, l_sLight, bool, p_visible );

	if ( l_sLight != NULL )
	{
		GET_AND_EXEC_PARAM( ColourValue, l_sLightSpecular, 1 );
		l_sLight->setVisible( p_visible );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_SetVisible : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_IsVisible )
{
	VERBOSE_STARTFUNC( "Light_IsVisible" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		RETURN_AS( bool ) l_sLight->isVisible();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_IsVisible : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_Show )
{
	VERBOSE_STARTFUNC( "Light_Show" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		l_sLight->setVisible( true );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_Show : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Lgh_Hide )
{
	VERBOSE_STARTFUNC( "Light_Hide" );
	GET_AND_EXEC_PARAM( Light *, l_sLight, 0 );

	if ( l_sLight != NULL )
	{
		l_sLight->setVisible( false );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Light_Hide : Light null" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_Create )
{
	VERBOSE_STARTFUNC( "Bil_Create" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ! p_name.empty() )
	{
		if ( ! ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
		{
			ScriptEngine::GetContext()->sceneManager->createBillboardSet( p_name );
		}
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_Create : name is empty" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_Exists )
{
	VERBOSE_STARTFUNC( "Bil_Exists" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );
	RETURN_AS( bool ) ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name );
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_SetMaterial )
{
	VERBOSE_STARTFUNC( "Bil_SetMaterial" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, String, p_matName );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );

		if ( !MaterialManager::getSingletonPtr()->getByName( p_matName ) )
		{
			SCRIPT_ERROR( "Error @ Bil_SetMaterial -> no such material exists (" + p_matName + ")" );
			return;
		}

		l_bill->setMaterialName( p_matName );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_SetMaterial : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_SetParent )
{
	VERBOSE_STARTFUNC( "Bil_SetParent" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, SceneNode *, p_parent );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );

		if ( p_parent == NULL )
		{
			SCRIPT_ERROR( "Error @ Bil_SetParent -> SceneNode is null" );
			return;
		}

		if ( l_bill->isAttached() )
		{
			l_bill->getParentSceneNode()->detachObject( l_bill );
		}

		p_parent->attachObject( l_bill );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_SetParent : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_SetDefaultSize )
{
	VERBOSE_STARTFUNC( "Bil_SetDefaultSize" );
	GET_AND_EXEC_THREE_PARAM( String, p_name, Real , p_width, Real, p_height );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );
		l_bill->setDefaultDimensions( p_width, p_height );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_SetDefaultSize : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_Clear )
{
	VERBOSE_STARTFUNC( "Bil_Clear" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );
		l_bill->clear();
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_Clear : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_AddElement )
{
	VERBOSE_STARTFUNC( "Bil_AddElement" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, Vector3, p_pos );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );
		l_bill->createBillboard( p_pos );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_AddElement : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_AddSizedElementAt )
{
	VERBOSE_STARTFUNC( "Bil_AddSizedElementAt" );
	GET_AND_EXEC_FOUR_PARAM( String, p_name, Vector3, p_pos, Real, p_width, Real, p_height );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );
		Billboard * l_b = l_bill->createBillboard( p_pos );
		l_b->setDimensions( p_width, p_height );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_AddSizedElementAt : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_RemoveIndex )
{
	VERBOSE_STARTFUNC( "Bil_RemoveIndex" );
	GET_AND_EXEC_TWO_PARAM( String, p_name, int, p_index );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		BillboardSet * l_bill = ScriptEngine::GetContext()->sceneManager->getBillboardSet( p_name );
		l_bill->removeBillboard( static_cast <uint32_t>( p_index ) );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_RemoveIndex : billboardSet named [" + p_name + "] does not exist" );
	}
}

EMUSE_SCRIPT_FUNCTION_DECLARE( Bil_Destroy )
{
	VERBOSE_STARTFUNC( "Bil_Destroy" );
	GET_AND_EXEC_PARAM( String, p_name, 0 );

	if ( ScriptEngine::GetContext()->sceneManager->hasBillboardSet( p_name ) )
	{
		ScriptEngine::GetContext()->sceneManager->destroyBillboardSet( p_name );
	}
	else
	{
		SCRIPT_ERROR( "Error @ Bil_Destroy : billboardSet named [" + p_name + "] does not exist" );
	}
}


