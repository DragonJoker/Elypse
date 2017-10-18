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
#ifndef _TrollOverlay_H_
#define _TrollOverlay_H_

#include "Module_2D.h"

#include <Module_Gui.h>

#include "Project/Media/Module_Media.h"
#include "Project/Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects2D
		{
			class TrollOverlay
				: public Object
			{
			public:
				TrollOverlay( wxString const & p_name, TrollOverlay * p_parent, wxString const & p_fileName );
				~TrollOverlay();

				void Reinitialise( wxString const & p_templateName );

				virtual void AddChild( TrollOverlay * p_child, bool p_updateElypse );
				virtual void RemoveChild( wxString const & p_name );

				void SetMuseOverlay( Elypse::Gui::EMOverlay * p_museOverlay );
				void SetOverlayClass( wxString const & p_className );
				void SetBaseMaterial( wxString const & p_matName, bool p_updateOgre = true );
				void SetMouseOverMaterial( wxString const & p_matName, bool p_updateOgre = true );
				void SetClickedMaterial( wxString const & p_matName, bool p_updateOgre = true );
				void SetPosition( Vector2 const & p_position, bool p_updateOgre = true );
				void SetLeft( float p_value, bool p_updateOgre = true );
				void SetTop( float p_value, bool p_updateOgre = true );
				void SetSize( Vector2 const & p_size, bool p_updateOgre = true );
				void SetWidth( float p_value, bool p_updateOgre = true );
				void SetHeight( float p_value, bool p_updateOgre = true );
				void SetVerticalAlign( TEVerticAlign p_align, bool p_updateOgre = true );
				void SetHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre = true );

				void SetCaption( wxString const & p_caption, bool p_updateOgre = true );
				void SetTextColour( Ogre::ColourValue const & p_colour, bool p_updateOgre = true );
				void SetFontName( wxString const & p_fontName, bool p_updateOgre = true );
				void SetFontSize( float p_height, bool p_updateOgre = true );
				void SetTextHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre = true );
				void SetTextVerticalAlign( TEVerticAlign p_align, bool p_updateOgre = true );
				void SetVisible( bool p_visible );

				void SetBorderMaterial( wxString const & p_name, bool p_updateOgre = true );
				void SetBorderSize( float p_top, float p_bottom, float p_left, float p_right, bool p_updateOgre = true );
				void SetBorderTop( float p_top, bool p_updateOgre = true );
				void SetBorderBottom( float p_bottom, bool p_updateOgre = true );
				void SetBorderLeft( float p_left, bool p_updateOgre = true );
				void SetBorderRight( float p_right, bool p_updateOgre = true );

				void Write( wxTextOutputStream & p_stream );

				inline Elypse::Gui::EMOverlay * GetMuseOverlay()const
				{
					return m_museOverlay;
				}
				inline wxString const & GetClassName()const
				{
					return m_className;
				}
				inline wxString const & GetBaseMaterial()const
				{
					return m_baseMaterial;
				}
				inline wxString const & GetMouseOverMaterial()const
				{
					return m_mouseOverMaterial;
				}
				inline wxString const & GetClickedMaterial()const
				{
					return m_clickedMaterial;
				}
				inline Vector2 const & GetPosition()const
				{
					return m_position;
				}
				inline Vector2 const & GetSize()const
				{
					return m_size;
				}
				inline TrollOverlay * GetParent()const
				{
					return m_parent;
				}
				inline TEVerticAlign GetVerticalAlign()const
				{
					return m_verticalAlign;
				}
				inline TEHorizAlign GetHorizontalAlign()const
				{
					return m_horizontalAlign;
				}
				inline Media::TrollVideoObject * GetVideo()const
				{
					return m_video;
				}
				inline TextInfos * GetTextInfos()const
				{
					return m_textInfos;
				}
				inline BorderInfos * GetBorderInfos()const
				{
					return m_borderInfos;
				}
				inline void	SetVideo( Media::TrollVideoObject * p_video )
				{
					m_video = p_video;
				}

			private:
				GUI::Properties::ObjectProperty * DoCreateProperties()override;

			private:
				wxString m_className;
				wxString m_baseMaterial;
				wxString m_mouseOverMaterial;
				wxString m_clickedMaterial;
				TEVerticAlign m_verticalAlign;
				TEHorizAlign m_horizontalAlign;
				Vector2 m_position;
				Vector2 m_size;
				TrollOverlay * m_parent;
				TrollOverlayMap m_childs;
				Media::TrollVideoObject * m_video;
				int m_depth;
				wxString m_inFileName;

				BorderInfos * m_borderInfos;
				TextInfos * m_textInfos;

				Elypse::Gui::EMOverlay * m_museOverlay;
			};
		}
	}
}

#endif
