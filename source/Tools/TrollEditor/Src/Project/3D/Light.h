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
#ifndef ___TELIGHT_H___
#define ___TELIGHT_H___

#include "3DObject.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class TrollLight
				: public Troll3DObject
			{
			public:
				TrollLight( wxString const & p_lightName, wxString const & p_fileName );
				virtual ~TrollLight();

				virtual void AttachTo( TrollSceneNode * p_node );

				void SetOgreLight( Ogre::Light * p_light );
				void SetLightType( TrollLightType p_lt, bool p_updateOgre = true );
				void SetDirection( const Vector3 & p_dir, bool p_updateOgre = true );
				void SetPosition( const Vector3 & p_pos, bool p_updateOgre = true );
				void SetDiffuse( const ColourValue & p_l, bool p_updateOgre = true );
				void SetSpecular( const ColourValue & p_l, bool p_updateOgre = true );
				void SetAttenuation( const Vector4 & p_att, bool p_updateOgre = true );
				void SetSpotLightRange( const Vector3 & p_splr, bool p_updateOgre = true );

				void Write( wxTextOutputStream & p_stream );

				inline TrollLightType GetLightType()const
				{
					return m_lightType;
				}
				inline const Ogre::Vector3 & GetDirection()const
				{
					return m_direction;
				}
				inline const Ogre::Vector3 & GetPosition()const
				{
					return m_position;
				}
				inline const Ogre::ColourValue & GetDiffuse()const
				{
					return m_diffuse;
				}
				inline const Ogre::ColourValue & GetSpecular()const
				{
					return m_specular;
				}
				inline const Ogre::Vector4 & GetAttenuation()const
				{
					return m_attenuation;
				}
				inline const Ogre::Vector3 & GetSpotlightRange()const
				{
					return m_spotLightRange;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				TrollLightType m_lightType;
				Ogre::Vector3 m_direction;
				Ogre::Vector3 m_position;
				Ogre::ColourValue m_diffuse;
				Ogre::ColourValue m_specular;
				Ogre::Vector4 m_attenuation;
				Ogre::Vector3 m_spotLightRange;
				Ogre::Light * m_ogreLight;
			};
		}
	}
}

#endif

