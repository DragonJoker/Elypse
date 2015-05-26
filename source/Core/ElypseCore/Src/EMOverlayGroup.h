/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___EMOVERLAYGROUP_H___
#define ___EMOVERLAYGROUP_H___

#include "Module_Gui.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export EMOverlayGroup :  public General::Theory::named, public General::Theory::owned_by<EMGui>, d_noncopyable
		{
			friend class EMOverlay;

		protected:
			Ogre::Overlay * m_overlay;
			EMOverlay * m_first;
			EMOverlayMap m_elements;

			bool m_visible;

		public:
			EMOverlayGroup( const String & p_name, EMGui * p_owner );
			~EMOverlayGroup();

		public:
			EMOverlay * CreateElement( const String & p_name );
			void ShowImmediate( bool p_visible );
			bool Contains( Real p_x, Real p_y )const;
			void DestroyElement( const String & p_name );

		protected:
			void _finaliseAddElement( EMOverlay * p_element );
			void _removeElement( const String & p_name );

		public:
			inline void					SetVisible( bool p_visible )
			{
				m_visible = p_visible;
			}
			inline bool					IsVisible()const
			{
				return m_visible;
			}
			inline Ogre::Overlay 	*	GetOverlay()const
			{
				return m_overlay;
			}
			inline EMOverlay 	*		GetFirstElement()const
			{
				return m_first;
			}
			inline const EMOverlayMap &	GetElementMap()const
			{
				return m_elements;
			}

			inline EMOverlay * GetElement( const String & p_name )const
			{
				return General::Utils::map::findOrNull( m_elements, p_name );
			}
		};
	}
}

#endif
