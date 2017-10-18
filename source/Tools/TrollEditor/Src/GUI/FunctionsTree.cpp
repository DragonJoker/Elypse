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
#include "TrollEditorPch.h"

#include "GUI/FunctionsTree.h"

#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"
#include "GUI/StcTextEditor.h"

#include "TrollEditorApplication.h"

#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/3D/3DObject.h"
#include "Project/Temporal/TemporalObject.h"
#include "Project/Temporal/AnimationGroup.h"
#include "Project/Temporal/AnimatedObject.h"
#include "Project/Media/MediaObject.h"
#include "Project/Media/Sound.h"
#include "Project/Media/SoundObject.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"

#include <VariableType.h>

#include "xpm/icon1.xpm"
#include "xpm/icon2.xpm"
#include "xpm/icon3.xpm"
#include "xpm/icon4.xpm"
#include "xpm/icon5.xpm"
#include "xpm/icon6.xpm"
#include "xpm/icon7.xpm"

namespace Troll
{
	namespace GUI
	{
		enum FunctionsTreeIDs
		{
			FunctionsTreeIcon_File = 0,
			FunctionsTreeIcon_FileSelected = 1,
			FunctionsTreeIcon_Folder = 2,
			FunctionsTreeIcon_FolderSelected = 3,
			FunctionsTreeIcon_FolderOpened = 4,
			FunctionsTreeIcon_ClassFunction = 5,

			TreeFunctions_Ctrl = 1000,
		};

		FunctionsTree::FunctionsTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style )
			: wxTreeCtrl{ parent, TreeFunctions_Ctrl, pos, size, style | wxTR_HIDE_ROOT }
			, m_projectFrame{ projectFrame }
		{
			CreateImageList();
		}

		FunctionsTree::~FunctionsTree()
		{
			Cleanup();
		}

		void FunctionsTree::Cleanup()
		{
			m_functionGroupsItems.clear();
			m_constantGroupsItems.clear();
			m_functions.clear();
			m_classFunctions.clear();
			m_constants.clear();
		}

		void FunctionsTree::CreateImageList( int p_size )
		{
			if ( p_size == -1 )
			{
				SetImageList( NULL );
				return;
			}

			if ( p_size == 0 )
			{
				p_size = m_imageSize;
			}
			else
			{
				m_imageSize = p_size;
			}

			wxImageList * l_images = new wxImageList( p_size, p_size, true );
			wxBusyCursor l_wait;
			wxIcon l_icons[7] =
			{
				wxIcon( icon1_xpm ),
				wxIcon( icon2_xpm ),
				wxIcon( icon3_xpm ),
				wxIcon( icon4_xpm ),
				wxIcon( icon5_xpm ),
				wxIcon( icon6_xpm ),
				wxIcon( icon7_xpm ),
			};

			int l_sizeOrig = l_icons[0].GetWidth();

			for ( size_t i = 0; i < WXSIZEOF( l_icons ); i++ )
			{
				if ( p_size == l_sizeOrig )
				{
					l_images->Add( l_icons[i] );
				}
				else
				{
					l_images->Add( wxBitmap( wxBitmap( l_icons[i] ).ConvertToImage().Rescale( p_size, p_size ) ) );
				}
			}

			AssignImageList( l_images );
		}

		void FunctionsTree::CreateTitreFunction( wxString const & p_idCat )
		{
			StrPairArray l_functions, l_classFunctions;
			m_functions.insert( std::make_pair( p_idCat, l_functions ) );
			m_classFunctions.insert( std::make_pair( p_idCat, l_classFunctions ) );
			m_currentGroup = p_idCat;
			m_functionsGroups.push_back( p_idCat );
		}

		void FunctionsTree::CreateTitreConstant( wxString const & p_idCat )
		{
			StrPairArray l_constants;
			m_constants.insert( std::make_pair( p_idCat, l_constants ) );
			m_currentGroup = p_idCat;
			m_constantsGroups.push_back( p_idCat );
		}

		void FunctionsTree::CreaFunc( wxString const & p_idfunc, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			StrPair l_function;
			l_function.first = p_idfunc;
			wxString l_data = _buildFunctionHelper( p_idfunc, p_returnValue, p_params );
			l_function.second = l_data;
			auto && l_it = m_functions.find( m_currentGroup );

			if ( l_it != m_functions.end() )
			{
				l_it->second.push_back( l_function );
				m_map.insert( std::make_pair( p_idfunc, l_data ) );
			}
		}

		void FunctionsTree::ClassFunc( wxString const & p_idfunc, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			StrPair l_function;
			l_function.first = p_idfunc;
			wxString l_data = _buildFunctionHelper( p_idfunc, p_returnValue, p_params );
			l_function.second = l_data;
			auto && l_it = m_classFunctions.find( m_currentGroup );

			if ( l_it != m_classFunctions.end() )
			{
				l_it->second.push_back( l_function );
				m_map.insert( std::make_pair( p_idfunc, l_data ) );
			}
		}

		void FunctionsTree::CreateConstant( wxString const & p_id, VariableBaseType p_type )
		{
			StrPair l_constant;
			l_constant.first = p_id;
			l_constant.second = make_wxString( VariableTypeManager::GetBaseTypeName( p_type ) );
			auto && l_it = m_constants.find( m_currentGroup );

			if ( l_it != m_constants.end() )
			{
				l_it->second.push_back( l_constant );
				m_map.insert( std::make_pair( l_constant.first, l_constant.second ) );
			}
		}

		void FunctionsTree::AddFunctionsToTree()
		{
			//int l_image = (wxGetApp().ShowImages() ? FunctionsTreeIcon_Folder : -1);
			int l_image = FunctionsTreeIcon_Folder;
			m_rootId = AddRoot( _( "Root" ), l_image, l_image, new TETreeItemData( wxT( " Root item" ), TITFunctionsRoot ) );
			m_functionsId = AppendItem( m_rootId, _( "Functions" ) );
			m_constantsId = AppendItem( m_rootId, _( "Constants" ) );

			if ( l_image != -1 )
			{
				SetItemImage( m_rootId, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
				SetItemImage( m_functionsId, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
				SetItemImage( m_constantsId, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
				SetItemImage( m_functionsId, FunctionsTreeIcon_Folder );
				SetItemImage( m_constantsId, FunctionsTreeIcon_Folder );
				SetItemImage( m_functionsId, FunctionsTreeIcon_FolderSelected, wxTreeItemIcon_Selected );
				SetItemImage( m_constantsId, FunctionsTreeIcon_FolderSelected, wxTreeItemIcon_Selected );
			}
		}

		void FunctionsTree::CreateFunctionsTree()
		{
			int l_groupImage = FunctionsTreeIcon_Folder;
			int l_groupImageSel = l_groupImage + 1;
			std::sort( m_functionsGroups.begin(), m_functionsGroups.end() );
			wxTreeItemId l_id;

			for ( size_t i = 0; i < m_functionsGroups.size(); i++ )
			{
				l_id = AppendItem( m_functionsId, m_functionsGroups[i], l_groupImage, l_groupImageSel, new TETreeItemData( wxString() << _( "Functions for" ) << wxT( "[" ) << m_functionsGroups[i] << wxT( "]" ), TITFunctionTitle ) );
				m_functionGroupsItems.insert( std::make_pair( m_functionsGroups[i], l_id ) );
			}

			_buildFunctionsList( m_functions, FunctionsTreeIcon_File, FunctionsTreeIcon_File + 1 );
			_buildFunctionsList( m_classFunctions, FunctionsTreeIcon_ClassFunction, FunctionsTreeIcon_ClassFunction + 1 );
		}

		void FunctionsTree::CreateConstantsTree()
		{
			int l_groupImage = FunctionsTreeIcon_Folder;
			int l_groupImageSel = l_groupImage + 1;
			std::sort( m_constantsGroups.begin(), m_constantsGroups.end() );
			wxTreeItemId l_id;

			for ( size_t i = 0; i < m_constantsGroups.size(); i++ )
			{
				l_id = AppendItem( m_constantsId, m_constantsGroups[i], l_groupImage, l_groupImageSel, new TETreeItemData( wxString() << _( "Constants for" ) << wxT( " [" ) << m_constantsGroups[i] << wxT( "]" ), TITConstantTitle ) );
				m_constantGroupsItems.insert( std::make_pair( m_constantsGroups[i], l_id ) );
			}

			wxString l_section;
			wxTreeItemId l_idParent;
			StrPairArray l_constants;
			int l_image = FunctionsTreeIcon_File;
			int l_selImage = FunctionsTreeIcon_File + 1;
			auto && l_constantIt = m_constants.begin();

			for ( ; l_constantIt != m_constants.end(); ++l_constantIt )
			{
				l_section = l_constantIt->first;
				l_idParent = m_constantGroupsItems.find( l_section )->second;
				l_constants = l_constantIt->second;
				std::sort( l_constants.begin(), l_constants.end() );

				for ( size_t i = 0; i < l_constants.size(); i++ )
				{
					l_id = AppendItem( l_idParent, l_constants[i].first, l_image, l_selImage, new TETreeItemData( l_constants[i].second, TITConstant ) );
				}

				//if (p_image != -1)
				//{
				//	SetItemImage( l_idParent, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded);
				//}
			}
		}

		void FunctionsTree::_buildFunctionsList( const StrPairArrayStrMap & p_list, int p_image, int p_imageSel )
		{
			wxString l_section;
			wxTreeItemId l_id, l_idParent;
			StrPairArray l_functions;
			auto && l_funcIt = p_list.begin();

			for ( ; l_funcIt != p_list.end(); ++l_funcIt )
			{
				l_section = l_funcIt->first;
				l_idParent = m_functionGroupsItems.find( l_section )->second;
				l_functions = l_funcIt->second;
				std::sort( l_functions.begin(), l_functions.end() );

				for ( size_t i = 0; i < l_functions.size(); i++ )
				{
					l_id = AppendItem( l_idParent, l_functions[i].first, p_image, p_imageSel, new TETreeItemData( l_functions[i].second, TITFunction ) );
				}

				//if (p_image != -1)
				//{
				//	SetItemImage( l_idParent, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded);
				//}
			}
		}

		wxString FunctionsTree::_buildFunctionHelper( wxString const & p_function, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			wxString l_data = ( p_returnValue != EMVT_NULL ? make_wxString( VariableTypeManager::GetBaseTypeName( p_returnValue ) ) : wxString( wxT( "" ) ) ) + wxT( " " ) + p_function + wxT( "( " );

			if ( p_params.size() > 0 )
			{
				l_data << ( p_params[0] != EMVT_NULL ? make_wxString( VariableTypeManager::GetBaseTypeName( p_params[0] ) ) : wxString( wxT( "" ) ) );

				for ( size_t i = 1; i < p_params.size(); i++ )
				{
					if ( p_params[i] != EMVT_NULL )
					{
						l_data << wxT( ", " ) << make_wxString( VariableTypeManager::GetBaseTypeName( p_params[i] ) );
					}
				}
			}

			l_data += wxT( ")" );
			return l_data;
		}

		void FunctionsTree::_logEvent( const wxChar * p_name, const wxTreeEvent & p_event )
		{
			wxTreeItemId l_item = p_event.GetItem();
			wxString l_text;

			if ( l_item.IsOk() )
			{
				l_text << wxT( '"' ) << GetItemText( l_item ).c_str() << wxT( '"' );
			}
			else
			{
				l_text = wxT( "invalid item" );
			}

			std::clog << p_name << "(" << l_text << ")" << std::endl;
		}

		BEGIN_EVENT_TABLE( FunctionsTree, wxTreeCtrl )
			EVT_TREE_ITEM_ACTIVATED( TreeFunctions_Ctrl, FunctionsTree::OnItemActivated )
			EVT_TREE_ITEM_RIGHT_CLICK( TreeFunctions_Ctrl, FunctionsTree::OnItemRClick )
			EVT_TREE_BEGIN_DRAG( TreeFunctions_Ctrl, FunctionsTree::OnDragBegin )
			EVT_TREE_END_DRAG( TreeFunctions_Ctrl, FunctionsTree::OnDragEnd )
			EVT_TREE_ITEM_GETTOOLTIP( TreeFunctions_Ctrl, FunctionsTree::OnShowToolTip )
		END_EVENT_TABLE()

		void FunctionsTree::OnItemActivated( wxTreeEvent & p_event )
		{
			auto && l_textCtrl = m_projectFrame->GetCurrentEdit();

			if ( l_textCtrl )
			{
				m_selectedItem = p_event.GetItem();
				SelectItem( m_selectedItem );
				size_t l_lastnull;
				wxString l_ajout;
				wxTreeItemId l_itemId = p_event.GetItem();
				TETreeItemData * l_item = reinterpret_cast< TETreeItemData * >( GetItemData( l_itemId ) );

				if ( l_item )
				{
					wxString l_itemdesc = l_item->GetDesc();

					while ( l_itemdesc.Contains( wxT( ",null" ) ) )
					{
						l_lastnull = l_itemdesc.find( wxT( ",null" ) );
						l_itemdesc = l_itemdesc.replace( l_lastnull, 5, wxT( "" ) );
					}

					if ( l_itemdesc.Contains( wxT( "null" ) ) )
					{
						l_lastnull = l_itemdesc.find( wxT( "null" ) );
						l_itemdesc = l_itemdesc.replace( l_lastnull, 4, wxT( "" ) );
					}

					while ( l_itemdesc.Contains( wxT( ",," ) ) )
					{
						l_lastnull = l_itemdesc.find( wxT( ",," ) );
						l_itemdesc = l_itemdesc.replace( l_lastnull, 2, wxT( "," ) );
					}

					if ( l_itemdesc.Contains( wxT( ",)" ) ) )
					{
						l_lastnull = l_itemdesc.find( wxT( ",)" ) );
						l_itemdesc = l_itemdesc.replace( l_lastnull, 2, wxT( ")" ) );
					}

					if ( l_itemdesc.Contains( wxT( "code)" ) ) )
					{
						l_ajout = wxT( "{\r\n\tcode;\r\n}\r\n" );
					}

					//TODO	l_textCtrl->WriteText( l_itemdesc + l_ajout + wxT( "\r\n"));
				}
			}
		}

		void FunctionsTree::OnItemRClick( wxTreeEvent & p_event )
		{
			m_selectedItem = p_event.GetItem();
			SelectItem( m_selectedItem );
		}

		void FunctionsTree::OnDragBegin( wxTreeEvent & p_event )
		{
			//wxTextDataObject my_data( "This text will be dragged.");
			//wxDropSource dragSource( this);
			//dragSource.SetData( my_data);
			//dragSource.DoDragDrop( TRUE);
		}

		void FunctionsTree::OnDragEnd( wxTreeEvent & p_event )
		{
		}

		void FunctionsTree::OnShowToolTip( wxTreeEvent & p_event )
		{
			wxTreeItemId l_item = p_event.GetItem();
			auto && l_it = m_map.find( GetItemText( l_item ) );

			if ( l_it != m_map.end() )
			{
				p_event.SetToolTip( l_it->second );
			}
		}
	}
}
