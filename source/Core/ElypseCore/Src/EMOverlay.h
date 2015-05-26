/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___EMOVERLAY_H___
#define ___EMOVERLAY_H___

#include "Module_Gui.h"
#include "Module_Script.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export EMOverlay :	public General::Theory::named,
			public General::Theory::owned_by<EMOverlayGroup>,
			d_noncopyable
		{
		protected:
			bool m_visible;

			String m_baseMaterialName;
			String m_mouseOverMaterialName;
			String m_clickedMaterialName;

			ScriptNode * m_clickedScript;

			Ogre::OverlayElement * m_overlay;

			EMOverlayArray m_childs;

			EMOverlay * m_parent;

			Ogre::Material * m_baseMaterialCopy;

			ClickableShapeArray m_clickables;

			Ogre::TextAreaOverlayElement::Alignment m_textAlign;

			bool m_wordWrapping;

		public :
			EMOverlay( const String & p_name, EMOverlayGroup * p_owner );
			~EMOverlay();

		public:
			void SetBaseMaterial( const String & p_materialName );
			void SetClickedScript( ScriptNode * p_script );

			EMOverlay * CreateChild( const String & p_name );
			//void AddChild( EMOverlay * p_child );

			EMOverlay * RecursiveGetScript( Real p_x, Real p_y );
			EMOverlay * RecursiveGetMouseOver( Real p_x, Real p_y );
			EMOverlay * RecursiveGetClick( Real p_x, Real p_y );

			void MouseOver();
			void Click();
			void Reset();

			bool TestMouseCoords( Real p_mouseX, Real p_mouseY )const;

			EMOverlay * GetAt( Real p_x, Real p_y );

			void AddClickableShape( ClickableShape * p_shape );

			Ogre::Material * GetOrCopyMaterial();

			bool Initialise( const String & p_className );
			void Reinitialise( const String & p_className );

			void SetAlignment( Ogre::TextAreaOverlayElement::Alignment p_alignement );
			Real GetTop()const;
			Real GetLeft()const;
			Real GetWidth()const;
			Real GetHeight()const;
			void SetTop( Real p_top );
			void SetLeft( Real p_left );
			void SetWidth( Real p_width );
			void SetHeight( Real p_height );

			void SetVisible( bool p_visible );

			void SetCaption( const String & p_caption );

			void ChangeParent( EMOverlay * p_parent );

			void DestroyChild( EMOverlay * p_child );

		private:
			void _removeChildImpl();
			void _wrapWords();
			Real _getCharSize( const Ogre::FontPtr & p_font, char p_char, Real p_fontSize );
			void _addChild( EMOverlay * p_child );

		public:
			inline bool					IsVisible()const
			{
				return m_visible;
			}
			inline bool					HasScript()const
			{
				return m_clickedScript != NULL;
			}
			inline bool					HasMouseOverMaterial()const
			{
				return ! m_mouseOverMaterialName.empty();
			}
			inline bool					HasClickedMaterial()const
			{
				return ! m_clickedMaterialName.empty();
			}
			inline unsigned int			GetNumChilds()const
			{
				return static_cast <unsigned int>( m_childs.size() );
			}
			inline const String 	&	GetBaseMaterial()const
			{
				return m_baseMaterialName;
			}
			inline const String 	&	GetMouseOverMaterial()const
			{
				return m_mouseOverMaterialName;
			}
			inline const String 	&	GetClickedMaterial()const
			{
				return m_clickedMaterialName;
			}

			inline Ogre::OverlayElement * GetOgreOverlayElement()const
			{
				return m_overlay;
			}
			inline EMOverlay		*	GetParent()const
			{
				return m_parent;
			}
			inline ScriptNode 	*		GetScript()const
			{
				return m_clickedScript;
			}
			inline bool					IsWordWrapped()const
			{
				return m_wordWrapping;
			}

			inline const EMOverlayArray & GetChilds()const
			{
				return m_childs;
			}

			inline void SetMouseOverMaterial( const String & p_materialName )
			{
				m_mouseOverMaterialName = p_materialName;
			}
			inline void SetClickedMaterial( const String & p_materialName )
			{
				m_clickedMaterialName = p_materialName;
			}

			inline void RemoveChild( EMOverlay * p_child )
			{
				General::Utils::vector::eraseValue( m_childs, p_child );
			}

			inline void SetParent( EMOverlay * p_parent )
			{
				m_parent = p_parent;
			}

			inline Ogre::TextAreaOverlayElement::Alignment GetTextAlign()const
			{
				return m_textAlign;
			}

			inline void SetWordWrap( bool p_wordWrap )
			{
				m_wordWrapping = p_wordWrap;

				if ( m_wordWrapping )
				{
					_wrapWords();
				}
			}
		};
	}
}

#endif
