/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SoundObject.h"

#include "SceneFileSaver.h"

#include "EMGui.h"
#include "EMOverlay.h"
#include "EMOverlayGroup.h"

#include "SoundManager.h"
#include "SoundInstance.h"

#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreBorderPanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlay.h>

using namespace Ogre;

SceneFileSaver::SceneFileSaver( SceneManager * p_manager, ScriptEngine * p_engine, EMGui * p_gui )
	: m_sceneManager( p_manager )
	, m_scriptEngine( p_engine )
	, m_gui( p_gui )
{
}

SceneFileSaver::~SceneFileSaver()
{
	;
}

void SceneFileSaver::_ntabs( uint32_t p_numTabs )
{
	for ( uint32_t  i = 0 ; i < p_numTabs ; i ++ )
	{
		m_buffer << '\t';
	}
}

void SceneFileSaver::_writeObjects()
{
	SceneManager::MovableObjectIterator iter = m_sceneManager->getMovableObjectIterator( "Entity" );

	while ( iter.hasMoreElements() )
	{
		Entity * l_entity = static_cast <Entity *>( iter.getNext() );
		m_buffer << "object " << l_entity->getName();
		m_buffer << std::endl;
		m_buffer << "{" << std::endl;
		m_buffer << "	mesh " << l_entity->getMesh()->getName() << std::endl;

		if ( l_entity->getCastShadows() )
		{
			m_buffer << "	shadows true" << std::endl;
		}

		if ( l_entity->isAttached() && l_entity->getParentSceneNode()->getName() != "root node" )
		{
			m_buffer << "	attach_to " << l_entity->getParentSceneNode()->getName() << std::endl;
		}

		auto & l_subMeshes = l_entity->getMesh()->getSubMeshes();

		if ( l_subMeshes.size() > 1 )
		{
			for ( uint16_t i = 0 ; i < uint32_t( l_subMeshes.size() ); i ++ )
			{
				if ( l_entity->getSubEntity( i )->getMaterialName() != l_subMeshes[i]->getMaterialName() )
				{
					m_buffer << "	submesh " << i << std::endl;
					m_buffer << "	{" << std::endl;
					m_buffer << "		material " << l_entity->getSubEntity( i )->getMaterialName() << std::endl;
					m_buffer << "	}" << std::endl;
				}
			}
		}
		else
		{
			if ( l_entity->getSubEntity( 0 )->getMaterialName() != l_entity->getMesh()->getSubMesh( 0 )->getMaterialName() )
			{
				m_buffer << "	material " << l_entity->getSubEntity( 0 )->getMaterialName() << std::endl;
			}
		}

		m_buffer << "}" << std::endl;
		m_buffer << std::endl;
	}
}

void SceneFileSaver::_writeSceneNodes( SceneNode * p_node )
{
	if ( p_node != m_sceneManager->getRootSceneNode() )
	{
		m_buffer <<  "scene_node " << p_node->getName() << std::endl;
		m_buffer <<  "{" << std::endl;

		if ( p_node->getParentSceneNode() != NULL && p_node->getParentSceneNode() != m_sceneManager->getRootSceneNode() )
		{
			m_buffer <<  "	parent " << p_node->getParentSceneNode()->getName() << std::endl;
		}

		Vector3 l_vector = p_node->getPosition();

		if ( l_vector != Vector3::ZERO )
		{
			m_buffer <<  "	position " << l_vector.x << " " << l_vector.y << " " << l_vector.z << std::endl;
		}

		l_vector = p_node->getScale();

		if ( l_vector != Vector3::UNIT_SCALE )
		{
			m_buffer <<  "	scale " << l_vector.x << " " << l_vector.y << " " << l_vector.z << std::endl;
		}

		Quaternion l_quat = p_node->getOrientation();

		if ( l_quat != Quaternion::IDENTITY )
		{
			Radian l_angle;
			l_quat.ToAngleAxis( l_angle, l_vector );
			m_buffer <<  "	orientation " << l_vector.x << " " << l_vector.y << " " << l_vector.z << " " << l_angle.valueDegrees() << std::endl;
		}

		if ( p_node->getInheritOrientation() )
		{
			m_buffer << "	inherit_orientation true" << std::endl;
		}

		if ( p_node->getInheritScale() )
		{
			m_buffer << "	inherit_scale true" << std::endl;
		}

		//TODO : sauvegarde des sons sur le scenefile.
		//TODO : sauvegarde de la video sur le sceneFile
		//TODO : sauvegarde de la physique sur le sceneFile
		/*
		SoundObject * l_object = SoundManager::GetSingletonPtr()->GetSoundForNode( start);

		if (l_object)
		{
			m_buffer+= "\tsound\n\t{\n";
			m_buffer+= "\t\tname "+l_object->GetName()+"\n";
			SoundInstance * l_me = l_object->m_instances.find( start->getName() )->second;
			if( l_me->IsLooped() )
			{
				m_buffer+= "\t\tlooped true\n";
			}
			m_buffer+= "\t}\n";
		}
		*/
		m_buffer << "}" << std::endl;
		m_buffer << std::endl;
	}

	auto & l_children = p_node->getChildren();

	for ( auto & l_child : l_children )
	{
		SceneNode * l_next = static_cast <SceneNode *>( l_child );
		_writeSceneNodes( l_next );
	}
}

void SceneFileSaver::_writeLights()
{
	SceneManager::MovableObjectIterator  iter = m_sceneManager->getMovableObjectIterator( "Light" );

	while ( iter.hasMoreElements() )
	{
		Light * l_light = static_cast<Light *>( iter.getNext() );
		m_buffer << "light " << l_light->getName() << std::endl;
		m_buffer << "{" << std::endl;
		Light::LightTypes l_type = l_light->getType();

		if ( l_type == Light::LT_DIRECTIONAL )
		{
			m_buffer << "	type directional" << std::endl;
		}
		else if ( l_type == Light::LT_POINT )
		{
			m_buffer << "	type point" << std::endl;
		}
		else if ( l_type == Light::LT_SPOTLIGHT )
		{
			m_buffer << "	type spotlight" << std::endl;
		}

		const Vector3 & l_direction = l_light->getDirection();

		if ( l_direction != Vector3::UNIT_Z )
		{
			m_buffer << "	direction " << l_direction.x << " " << l_direction.y << " " << l_direction.z << std::endl;
		}

		if ( l_type != Light::LT_DIRECTIONAL )
		{
			const Vector3 & l_position = l_light->getPosition();

			if ( l_position != Vector3::ZERO )
			{
				m_buffer << "	position " << l_position.x << " " << l_position.y << " " << l_position.z << std::endl;
			}

			if ( l_light->getAttenuationRange() != 100000.0
					|| 	l_light->getAttenuationConstant() != 1.0
					|| 	l_light->getAttenuationLinear() != 0.0
					|| 	l_light->getAttenuationQuadric() != 0.0 )
			{
				m_buffer << "	attenuation " << l_light->getAttenuationRange() << " " << l_light->getAttenuationConstant() << " " << l_light->getAttenuationLinear() << " " << l_light->getAttenuationQuadric() << std::endl;
			}

			if ( l_light->isAttached() )
			{
				m_buffer << "	attach_to " << l_light->getParentSceneNode()->getName() << std::endl;
			}
		}

		const ColourValue & l_diffuse = l_light->getDiffuseColour();

		if ( l_diffuse != ColourValue::White )
		{
			m_buffer << "	diffuse " << l_diffuse.r << " " << l_diffuse.g << " " << l_diffuse.b << std::endl;
		}

		const ColourValue & l_specular = l_light->getSpecularColour();

		if ( l_specular != ColourValue::Black )
		{
			m_buffer << "	specular " << l_specular.r << " " << l_specular.g << " " << l_specular.b << std::endl;
		}

		if ( l_type == Light::LT_SPOTLIGHT )
		{
			if ( l_light->getSpotlightFalloff() != 1.0
					||	l_light->getSpotlightInnerAngle() != Degree( 30.0f )
					||	l_light->getSpotlightOuterAngle() != Degree( 40.0f ) )
			{
				m_buffer << "	spotlight_range " << l_light->getSpotlightInnerAngle().valueDegrees() << " " << l_light->getSpotlightOuterAngle().valueDegrees() << " " << l_light->getSpotlightFalloff() << std::endl;
			}
		}

		m_buffer << "}" << std::endl;
		m_buffer << std::endl;
	}
}

void SceneFileSaver::_writeCameras()
{
	auto & l_cameras = m_sceneManager->getCameras();

	for ( auto & l_cameraIt : l_cameras )
	{
		Camera * l_camera = l_cameraIt.second;
		m_buffer << "camera " << l_camera->getName() << std::endl;
		m_buffer << "{" << std::endl;
		const Vector3 & l_position = l_camera->getPosition();

		if ( l_position != Vector3::ZERO )
		{
			m_buffer << "	position " << l_position.x << " " << l_position.y << " " << l_position.z << std::endl;
		}

		const Vector3 & l_direction = ( l_camera->getOrientation() * Vector3::NEGATIVE_UNIT_Z );

		if ( l_direction != Vector3::NEGATIVE_UNIT_Z )
		{
			m_buffer << "	direction " << l_position.x << " " << l_position.y << " " << l_position.z << std::endl;
		}

		if ( l_camera->isAttached() && l_camera->getParentSceneNode() != m_sceneManager->getRootSceneNode() )
		{
			m_buffer << "	attach_to " + l_camera->getParentSceneNode()->getName() << std::endl;
		}

		m_buffer << "}" << std::endl;
		m_buffer << std::endl;
	}
}
/*
void SceneFileSaver::_writeOverlays( OverlayElement * over, int depth)
{
	OverlayContainer*cont;
	OverlayElement *o;
	const OverlayElement*tem;
	BorderPanelOverlayElement *borderPanel,*templateBorderPanel;
	TextAreaOverlayElement *textPanel,*templateTextPanel;
	String code;
	Real x,y;

	if(depth>0)
	{
		String l_name = over->getName();
		l_name.erase(l_name.end()-1);
		m_buffer+= NTabs(depth)+"element "+l_name+String("\n")+NTabs(depth)+String("{\n");
	}
	tem = over->getSourceTemplate();
	m_buffer+= NTabs(depth)+"\tclass "+tem->getName()+String("\n");
	if( ( over->getHorizontalAlignment() != tem->getHorizontalAlignment() ) || ( over->getVerticalAlignment() != tem->getVerticalAlignment() ) )
	{
		m_buffer+= NTabs(depth+1) + "align ";
		if( over->getVerticalAlignment() == GVA_TOP )
		{
			m_buffer+= "top ";
		}else
		if( over->getVerticalAlignment() == GVA_CENTER )
		{
			m_buffer+= "center ";
		}else
		if( over->getVerticalAlignment() == GVA_BOTTOM )
		{
			m_buffer+= "bottom ";
		}

		if( over->getHorizontalAlignment() == GHA_LEFT )
		{
			m_buffer+= "left ";
		}else
		if( over->getHorizontalAlignment() == GHA_CENTER )
		{
			m_buffer+= "center ";
		}else
		if( over->getHorizontalAlignment() == GHA_RIGHT )
		{
			m_buffer+= "right ";
		}

		m_buffer+= "\n";
	}

	if(tem->getMaterialName() != over->getMaterialName())
	{
		m_buffer+= NTabs(depth)+"\tmaterial "+over->getMaterialName()+String("\n");
	}


	if(depth>0)
	{
		x = over->getLeft()*100.0;
		x /= over->getParent()->getWidth();
		y = over->getTop()*100.0;
		y /= over->getParent()->getHeight();
	}
	else
	{
		x = over->getLeft()*100.0;
		x /= 1024.0;
		y = over->getTop()*100.0;
		y /= 768.0;
	}
	m_buffer+= NTabs(depth)+"\tposition " + StringConverter::toString(x,4) + String("% ") + StringConverter::toString(y,4)  + String("%\n");
//	m_buffer+= NTabs(depth)+"\tposition " + StringConverter::toString(over->getLeft()) + String(" ") + StringConverter::toString(over->getTop())  + String("\n");

	if(depth>0)
	{
		x = over->getWidth()*100.0;
		x /= over->getParent()->getWidth();
		y = over->getHeight()*100.0;
		y /= over->getParent()->getHeight();
	}
	else
	{
		x = over->getWidth()*100.0;
		x /= 1024.0;
		y = over->getHeight()*100.0;
		y /= 768.0;
	}
	m_buffer+= NTabs(depth)+"\tsize " + StringConverter::toString(x,4) + String("% ") + StringConverter::toString(y,4)  + String("%\n");

	if(over->getTypeName() == "BorderPanel")
	{
		borderPanel = (BorderPanelOverlayElement*)over;
		templateBorderPanel = (BorderPanelOverlayElement*)tem;
		if(borderPanel->getBorderMaterialName() != templateBorderPanel->getBorderMaterialName())
		{
			m_buffer+= NTabs(depth)+"\tborder_material "+borderPanel->getBorderMaterialName()+String("\n");
		}

		if(borderPanel->getBottomBorderSize() != templateBorderPanel->getBottomBorderSize() ||
			borderPanel->getTopBorderSize() != templateBorderPanel->getTopBorderSize() ||
			borderPanel->getLeftBorderSize() != templateBorderPanel->getLeftBorderSize() ||
			borderPanel->getRightBorderSize() != templateBorderPanel->getRightBorderSize())
		{
			m_buffer+= NTabs(depth)+"\tborder_size " + StringConverter::toString(borderPanel->getLeftBorderSize()) + String(" ") + StringConverter::toString(borderPanel->getRightBorderSize()) + String(" ") + StringConverter::toString(borderPanel->getTopBorderSize()) + String(" ") + StringConverter::toString(borderPanel->getBottomBorderSize()) + String("\n");
		}
	}

	if(over->getTypeName() == "TextArea")
	{
		textPanel = (TextAreaOverlayElement*)over;
		templateTextPanel = (TextAreaOverlayElement*)tem;

		if(textPanel->getCaption() != templateTextPanel->getCaption())
		{
			m_buffer+= NTabs(depth)+"\ttext "+textPanel->getCaption().asUTF8()+String("\n");
		}
		if(textPanel->getFontName() != templateTextPanel->getFontName())
		{
			m_buffer+= NTabs(depth)+"\tfont_name "+textPanel->getFontName()+String("\n");
		}
		if(textPanel->getColour() != templateTextPanel->getColour())
		{
			m_buffer+= NTabs(depth)+"\ttext_colour "+StringConverter::toString(textPanel->getColour())+String("\n");
		}
	}

	EMOverlay * l_EMOE = m_gui->GetElementByName( over->getName() );
	if( l_EMOE != NULL )
	{
		if( l_EMOE->HasMouseOverMaterial() )
		{
			m_buffer+= NTabs(depth+1)+"mouse_over_material "+l_EMOE->GetMouseOverMaterial()+String("\n");
		}
	}

	if(over->isContainer())
	{
		cont = (OverlayContainer*)over;

		OverlayContainer::ChildIterator iter = cont->getChildIterator();

		while(iter.hasMoreElements())
		{
			o = iter.getNext();
			_writeOverlays(o,depth+1);
		}
	}

	if(depth>0)
	{
		m_buffer+= NTabs(depth)+"}\n";
	}
}


void SceneFileSaver::_writeAllOverlays()
{
	const OverlayGroupMap & l_overList = m_gui->GetAllOverlays();
	OverlayGroupMap::const_iterator iter = l_overList.begin();

	while(iter!= l_overList.end())
	{
		String l_name = (iter->second)->GetOverlay()->getName();
		l_name.erase(l_name.end()-1);
		m_buffer+= "\noverlay "+l_name+"\n{\n";

		Overlay::Overlay2DElementsIterator iter2 = (iter->second)->GetOverlay()->get2DElementsIterator();
		while(iter2.hasMoreElements())
		{
			_writeOverlays((OverlayElement*)iter2.getNext(),0);
		}

		m_buffer+= "}\n\n";
		++iter;
	}
}
*/
/*
void SceneFileSaver::_writeSounds()
{
	const SoundObjectMap & l_soundMap = SoundManager::GetSingletonPtr()->GetAllSoundsObjects();

	SoundObjectMap::const_iterator i = l_soundMap.begin();
	const SoundObjectMap::const_iterator & iend = l_soundMap.end();

	for ( ; i != iend ; ++ i)
	{
		SoundObject * l_object = i->second;

		m_buffer << "sound " << l_object->GetName() << std::endl;
		m_buffer << "{" << std::endl;
		m_buffer << "	url " << l_object->GetSoundURL() << std::endl;
		m_buffer << "	url " << l_object->GetSoundURL() << std::endl;
		m_buffer+= "\ttype "+StringConverter::toString(l_object->GetSoundType())+"\n";
		m_buffer+= "\tglobal "+StringConverter::toString( l_object->IsGlobal() )+"\n";
		if( l_object->IsLooped() )
		{
			m_buffer+= "\tlooped true\n";
		}
		m_buffer << "}" << std::endl;
		m_buffer << std::endl;

	}
}
*/

void SceneFileSaver::SaveAll( String const & p_filename )
{
	m_buffer.open( p_filename.c_str() );

	if ( ! m_buffer.is_open() )
	{
		return;
	}

	m_buffer.clear();
	const ColourValue & l_ambient = m_sceneManager->getAmbientLight();

	if ( l_ambient != ColourValue::Black )
	{
		m_buffer << "ambient_light " << l_ambient.r << " " << l_ambient.g << " " << l_ambient.b << " " << l_ambient.a << std::endl;
	}

	if ( m_sceneManager->getShadowTechnique() == SHADOWTYPE_STENCIL_ADDITIVE )
	{
		m_buffer << "shadows stencil" << std::endl;
	}
	else if ( m_sceneManager->getShadowTechnique() == SHADOWTYPE_TEXTURE_ADDITIVE )
	{
		m_buffer << "shadows texture" << std::endl;
	}

//	_writeSounds();
	_writeSceneNodes( m_sceneManager->getRootSceneNode() );
	_writeObjects();
	_writeLights();
	_writeCameras();
//	m_buffer += "\n//Code\n\n";
//	_writeCode();
//	_writeAllOverlays();
	/*	if(m_engine->m_gui->m_overlays.size())
		{
			m_buffer += "\n//Overlays\n\n";

		}
	*/
	/*
	FILE *fp;
	fp = fopen(p_filename.c_str(), "w");
	if (!fp)
		OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create export file.",
		"SceneFileSaver::SaveAll");

	fputs(m_buffer.c_str(), fp);
	fclose(fp);
	*/
	m_buffer.close();
//	std::cout<<"Start------------------------\n\n"<<m_buffer<<"\n\n---------------------------End\n\n";
}
