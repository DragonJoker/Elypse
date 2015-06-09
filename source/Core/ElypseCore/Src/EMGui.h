/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/
#ifndef ___EMGUI__h___
#define ___EMGUI__h___

#include "Module_Gui.h"

#include "Module_Script.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export EMGui : d_noncopyable
		{
		protected:
			EMOverlayMap m_overlays;
			OverlayGroupMap m_groups;

			EMOverlay * m_currentClickedOverlay;
			EMOverlay * m_currentMouseOverOverlay;
			EMOverlay * m_topmostOverlay;

			String m_instanceKey;

			Real m_aspectRatio;

		public:
			EMGui( const String & p_instanceKey, Real p_aspect );
			~EMGui();

		public:
			void AddElement( EMOverlay * p_element );

			EMOverlay * GetTopmostAt( Real p_x, Real p_y )const;
			void MouseOver( Real p_mouseX, Real p_mouseY );
			ScriptNode * Click( Real p_mouseX, Real p_mouseY );

			void SetAllOverlaysVisibility( bool p_visible )const;

			void ClearAll() noexcept;

			EMOverlayGroup * CreateOverlayGroup( const String & p_name );

			void RemoveOverlay( const String & p_overlayName );
			void DestroyOverlayGroup( const String & p_name );
			void DestroyEMOverlay( const String & p_name );

		public:
			inline const OverlayGroupMap &	GetAllOverlays()const
			{
				return m_groups;
			}
			inline EMOverlay 		*		GetTopmost()const
			{
				return m_topmostOverlay;
			}
			inline const String & GetInstanceKey()const
			{
				return m_instanceKey;
			}

			inline EMOverlayGroup * GetOverlayGroup( const String & p_name )const
			{
				return General::Utils::map::findOrNull( m_groups, p_name );
			}
			inline EMOverlay * GetElementByName( const String & p_elementName )const
			{
				return General::Utils::map::findOrNull( m_overlays, p_elementName );
			}

		public:
			Real GetWidth( char p_char, const FontPtr & p_font, Real p_fontSize )const;
			Real GetWidth( const String & p_string, const FontPtr & p_font, Real p_fontSize )const;
		};
	}
}

#endif
