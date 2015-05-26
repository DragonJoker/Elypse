//***********************************************************************************************************
#ifndef ___TELIGHT_H___
#define ___TELIGHT_H___
//***********************************************************************************************************
#include "Module_3D.h"
#include "3DObject.h"
//***********************************************************************************************************
namespace Troll
{
	namespace Objects3D
	{
		class TrollLight : public Troll3DObject
		{
		private:
			TrollLightType m_lightType;
			Ogre::Vector3 m_direction;
			Ogre::Vector3 m_position;
			Ogre::ColourValue m_diffuse;
			Ogre::ColourValue m_specular;
			Ogre::Vector4 m_attenuation;
			Ogre::Vector3 m_spotLightRange;
			Ogre::Light * m_ogreLight;

		public:
			TrollLight( const wxString & p_lightName, const wxString & p_fileName );
			virtual ~TrollLight();

			virtual void AttachTo( TrollSceneNode * p_node );
			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetOgreLight( Ogre::Light * p_light );
			void SetLightType( TrollLightType p_lt, bool p_updateOgre = true );
			void SetDirection( const Vector3 & p_dir, bool p_updateOgre = true );
			void SetPosition( const Vector3 & p_pos, bool p_updateOgre = true );
			void SetDiffuse( const ColourValue & p_l, bool p_updateOgre = true );
			void SetSpecular( const ColourValue & p_l, bool p_updateOgre = true );
			void SetAttenuation( const Vector4 & p_att, bool p_updateOgre = true );
			void SetSpotLightRange( const Vector3 & p_splr, bool p_updateOgre = true );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline TrollLightType			GetLightType()const
			{
				return m_lightType;
			}
			inline const Vector3 	&	GetDirection()const
			{
				return m_direction;
			}
			inline const Vector3 	&	GetPosition()const
			{
				return m_position;
			}
			inline const ColourValue &	GetDiffuse()const
			{
				return m_diffuse;
			}
			inline const ColourValue &	GetSpecular()const
			{
				return m_specular;
			}
			inline const Vector4 	&	GetAttenuation()const
			{
				return m_attenuation;
			}
			inline const Vector3 	&	GetSpotlightRange()const
			{
				return m_spotLightRange;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
