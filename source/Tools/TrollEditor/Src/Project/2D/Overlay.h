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

BEGIN_TROLL_PROJECT_2D_NAMESPACE
{
	class TrollOverlay
		: public Object
	{
	public:
		TrollOverlay( const wxString & p_name, TrollOverlay * p_parent, const wxString & p_fileName );
		~TrollOverlay();

		void Reinitialise( const wxString & p_templateName );

		virtual void AddChild( TrollOverlay * p_child, bool p_updateElypse );
		virtual void RemoveChild( const wxString & p_name );

		virtual int BuildPanel( wxWindow * p_parent, int p_width );

		void SetMuseOverlay( Elypse::Gui::EMOverlay * p_museOverlay );
		void SetOverlayClass( const wxString & p_className );
		void SetBaseMaterial( const wxString & p_matName, bool p_updateOgre = true );
		void SetMouseOverMaterial( const wxString & p_matName, bool p_updateOgre = true );
		void SetClickedMaterial( const wxString & p_matName, bool p_updateOgre = true );
		void SetTop( float p_top, bool p_updateOgre = true );
		void SetLeft( float p_left, bool p_updateOgre = true );
		void SetWidth( float p_width, bool p_updateOgre = true );
		void SetHeight( float p_height, bool p_updateOgre = true );
		void SetVerticalAlign( TEVerticAlign p_align, bool p_updateOgre = true );
		void SetHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre = true );

		void SetCaption( const wxString & p_caption, bool p_updateOgre = true );
		void SetTextColour( const Ogre::ColourValue & p_colour, bool p_updateOgre = true );
		void SetFontName( const wxString & p_fontName, bool p_updateOgre = true );
		void SetFontSize( float p_height, bool p_updateOgre = true );
		void SetTextHorizontalAlign( TEHorizAlign p_align, bool p_updateOgre = true );
		void SetTextVerticalAlign( TEVerticAlign p_align, bool p_updateOgre = true );
		void SetVisible( bool p_visible );

		void SetBorderMaterial( const wxString & p_name, bool p_updateOgre = true );
		void SetBorderSize( float p_top, float p_bottom, float p_left, float p_right, bool p_updateOgre = true );
		void SetBorderTop( float p_top, bool p_updateOgre = true );
		void SetBorderBottom( float p_bottom, bool p_updateOgre = true );
		void SetBorderLeft( float p_left, bool p_updateOgre = true );
		void SetBorderRight( float p_right, bool p_updateOgre = true );

		void Write( wxTextOutputStream * p_stream );

		inline Elypse::Gui::EMOverlay * GetMuseOverlay()const
		{
			return m_museOverlay;
		}
		inline const wxString & GetClassName()const
		{
			return m_className;
		}
		inline const wxString & GetBaseMaterial()const
		{
			return m_baseMaterial;
		}
		inline const wxString & GetMouseOverMaterial()const
		{
			return m_mouseOverMaterial;
		}
		inline const wxString & GetClickedMaterial()const
		{
			return m_clickedMaterial;
		}
		inline float GetTop()const
		{
			return m_top;
		}
		inline float GetLeft()const
		{
			return m_left;
		}
		inline float GetWidth()const
		{
			return m_width;
		}
		inline float GetHeight()const
		{
			return m_height;
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
		wxString m_className;
		wxString m_baseMaterial;
		wxString m_mouseOverMaterial;
		wxString m_clickedMaterial;
		TEVerticAlign m_verticalAlign;
		TEHorizAlign m_horizontalAlign;
		float m_top;
		float m_left;
		float m_width;
		float m_height;
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
END_TROLL_PROJECT_2D_NAMESPACE

#endif

