/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___MIRROR_H___
#define ___MIRROR_H___

#include "Module_SpecialEffects.h"

namespace EMuse
{
	namespace SpecialEffects
	{
		class Mirror : public named, noncopyable
		{
		private:
			Camera * m_camera;
			Texture * m_texture;
			SceneNode * m_planeNode;
			SceneManager * m_manager;
			Camera * m_mainCamera;
			MovablePlane * m_plane;
			RenderTarget * m_render;
			Viewport * m_viewport;
			MirrorMaterialMap m_materials;

			bool m_needUpdate;
			bool m_autoUpdate;
			bool m_alwaysRedraw;

			Vector3 m_position;
			Vector3 m_normal;
			ColourValue m_background;
			Real m_alpha;

			unsigned int m_texWidth;
			unsigned int m_texHeight;

			String m_instanceKey;

		public:
			Mirror( const String & p_name );
			~Mirror();

		public:
			void AddMaterial( const String & p_materialName, Real p_opacity );
			void RemoveMaterial( const String & p_materialName );
			void ChangeMaterialAlpha( const String & p_materialName, Real p_opacity );
			void Update();
			bool IsVisible()const;
			void SetAutoUpdate( bool p_redrawWhenCameraMoves, bool p_redrawEveryFrame );
			void Initialise( const String & p_instanceKey, SceneManager * p_sceneManager );

		private:
			void _linkCameras();
			void _setBackground();

		public:
			inline void RequireUpdate()
			{
				m_needUpdate = true;
			}

			inline void SetPosition( const Vector3 & p_position )
			{
				m_position = p_position;
			}
			inline void SetNormal( const Vector3 & p_normal )
			{
				m_normal = p_normal;
			}
			inline void SetTextureResolution( unsigned int p_width, unsigned int p_height )
			{
				m_texWidth = p_width;
				m_texHeight = p_height;
			}
			inline void SetBackground( const ColourValue & p_colour )
			{
				m_background = p_colour;
				_setBackground();
			}

			inline void SetMainCamera( Camera * p_mainCamera )
			{
				m_mainCamera = p_mainCamera;
				_linkCameras();
			}

			inline Camera * GetCamera()const
			{
				return m_camera;
			}
		};
	}
}

#endif