
#include "PrecompiledHeader.h"

#include "GUI/FunctionsTree.h"
#include "GUI/main.h"
#include "GUI/MainFrame.h"
#include "GUI/StcTextEditor.hpp"
#include "GUI/TextPanel.h"
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

/*
#include <map>
#include <vector>

#include <Data/ConfigFile.h>

#include <General/Memory.h>
#include <General/STLMacros.h>
#include <wx/dnd.h>
*/

using namespace Troll;
using namespace Troll::GUI;
using namespace Troll::Temporal;
using namespace Troll::Objects3D;
using namespace Troll::Objects2D;
using namespace Troll::Media;

BEGIN_EVENT_TABLE( FunctionsTree, wxTreeCtrl )
	EVT_TREE_ITEM_ACTIVATED(	TreeFunctions_Ctrl,	FunctionsTree::_onItemActivated )
	EVT_TREE_ITEM_RIGHT_CLICK(	TreeFunctions_Ctrl,	FunctionsTree::_onItemRClick )
	EVT_TREE_BEGIN_DRAG(	TreeFunctions_Ctrl,	FunctionsTree::_onDragBegin )
	EVT_TREE_END_DRAG(	TreeFunctions_Ctrl,	FunctionsTree::_onDragEnd )
	EVT_TREE_ITEM_GETTOOLTIP(	TreeFunctions_Ctrl, FunctionsTree::_onShowToolTip )
END_EVENT_TABLE()

extern MainFrame	* g_mainFrame;
//DECLARE_APP( TEApplication);


FunctionsTree::FunctionsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
	:	wxTreeCtrl( parent, TreeFunctions_Ctrl, pos, size, style | wxTR_HIDE_ROOT )
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
	wxIcon l_icons[7];
	l_icons[0] = wxIcon( icon1_xpm );
	l_icons[1] = wxIcon( icon2_xpm );
	l_icons[2] = wxIcon( icon3_xpm );
	l_icons[3] = wxIcon( icon4_xpm );
	l_icons[4] = wxIcon( icon5_xpm );
	l_icons[5] = wxIcon( icon6_xpm );
	l_icons[6] = wxIcon( icon7_xpm );
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



void FunctionsTree::CreateTitreFunction( const wxString & p_idCat )
{
	StrPairArray l_functions, l_classFunctions;
	m_functions.insert( StrPairArrayStrMap::value_type( p_idCat, l_functions ) );
	m_classFunctions.insert( StrPairArrayStrMap::value_type( p_idCat, l_classFunctions ) );
	m_currentGroup = p_idCat;
	m_functionsGroups.push_back( p_idCat );
}



void FunctionsTree::CreateTitreConstant( const wxString & p_idCat )
{
	StrPairArray l_constants;
	m_constants.insert( StrPairArrayStrMap::value_type( p_idCat, l_constants ) );
	m_currentGroup = p_idCat;
	m_constantsGroups.push_back( p_idCat );
}



void FunctionsTree::CreaFunc( const wxString & p_idfunc, VariableBaseType p_returnValue,
							  std::vector <VariableBaseType> p_params )
{
	StrPair l_function;
	l_function.first = p_idfunc;
	wxString l_data = _buildFunctionHelper( p_idfunc, p_returnValue, p_params );
	l_function.second = l_data;
	StrPairArrayStrMap::iterator l_it = m_functions.find( m_currentGroup );

	if ( l_it != m_functions.end() )
	{
		l_it->second.push_back( l_function );
		g_mainFrame->m_map.insert( MapType::value_type( p_idfunc, l_data ) );
	}
}



void FunctionsTree::ClassFunc( const wxString & p_idfunc, VariableBaseType p_returnValue,
							   std::vector <VariableBaseType> p_params )
{
	StrPair l_function;
	l_function.first = p_idfunc;
	wxString l_data = _buildFunctionHelper( p_idfunc, p_returnValue, p_params );
	l_function.second = l_data;
	StrPairArrayStrMap::iterator l_it = m_classFunctions.find( m_currentGroup );

	if ( l_it != m_classFunctions.end() )
	{
		l_it->second.push_back( l_function );
		g_mainFrame->m_map.insert( MapType::value_type( p_idfunc, l_data ) );
	}
}



void FunctionsTree::CreateConstant( const wxString & p_id, VariableBaseType p_type )
{
	StrPair l_constant;
	l_constant.first = p_id;
	l_constant.second = wxString( VariableTypeManager::GetBaseTypeName( p_type ).c_str(), wxMBConvLibc() );
	StrPairArrayStrMap::iterator l_it = m_constants.find( m_currentGroup );

	if ( l_it != m_constants.end() )
	{
		l_it->second.push_back( l_constant );
		g_mainFrame->m_map.insert( MapType::value_type( l_constant.first, l_constant.second ) );
	}
}



void FunctionsTree::AddFunctionsToTree()
{
//	int l_image = (wxGetApp().ShowImages() ? FunctionsTreeIcon_Folder : -1);
	int l_image = FunctionsTreeIcon_Folder;
	m_rootId = AddRoot( _T( "Root" ), l_image, l_image, new TETreeItemData( _T( " Root item" ), TITFunctionsRoot ) );
	m_functionsId = AppendItem( m_rootId, _T( "Fonctions" ) );
	m_constantsId = AppendItem( m_rootId, _T( "Constantes" ) );

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

	for ( size_t i = 0 ; i < m_functionsGroups.size() ; i++ )
	{
		l_id = AppendItem( m_functionsId, m_functionsGroups[i], l_groupImage, l_groupImageSel,
						   new TETreeItemData( wxT( "Fonctions pour les " ) + m_functionsGroups[i] + wxT( "s" ), TITFunctionTitle ) );
		m_functionGroupsItems.insert( ItemStrMap::value_type( m_functionsGroups[i], l_id ) );
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

	for ( size_t i = 0 ; i < m_constantsGroups.size() ; i++ )
	{
		l_id = AppendItem( m_constantsId, m_constantsGroups[i], l_groupImage, l_groupImageSel,
						   new TETreeItemData( wxT( "Constantes pour les " ) + m_constantsGroups[i] + wxT( "s" ), TITConstantTitle ) );
		m_constantGroupsItems.insert( ItemStrMap::value_type( m_constantsGroups[i], l_id ) );
	}

	wxString l_section;
	wxTreeItemId l_idParent;
	StrPairArray l_constants;
	int l_image = FunctionsTreeIcon_File;
	int l_selImage = FunctionsTreeIcon_File + 1;
	StrPairArrayStrMap::const_iterator l_constantIt = m_constants.begin();

	for ( ; l_constantIt != m_constants.end() ; ++ l_constantIt )
	{
		l_section = l_constantIt->first;
		l_idParent = m_constantGroupsItems.find( l_section )->second;
		l_constants = l_constantIt->second;
		std::sort( l_constants.begin(), l_constants.end() );

		for ( size_t i = 0 ; i < l_constants.size() ; i++ )
		{
			l_id = AppendItem( l_idParent, l_constants[i].first, l_image, l_selImage,
							   new TETreeItemData( l_constants[i].second, TITConstant ) );
		}

		/*
				if (p_image != -1)
				{
					SetItemImage( l_idParent, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded);
				}
		*/
	}
}



void FunctionsTree::_buildFunctionsList( const StrPairArrayStrMap & p_list, int p_image, int p_imageSel )
{
	wxString l_section;
	wxTreeItemId l_id, l_idParent;
	StrPairArray l_functions;
	StrPairArrayStrMap::const_iterator l_funcIt = p_list.begin();

	for ( ; l_funcIt != p_list.end() ; ++ l_funcIt )
	{
		l_section = l_funcIt->first;
		l_idParent = m_functionGroupsItems.find( l_section )->second;
		l_functions = l_funcIt->second;
		std::sort( l_functions.begin(), l_functions.end() );

		for ( size_t i = 0 ; i < l_functions.size() ; i++ )
		{
			l_id = AppendItem( l_idParent, l_functions[i].first, p_image, p_imageSel,
							   new TETreeItemData( l_functions[i].second, TITFunction ) );
		}

		/*
				if (p_image != -1)
				{
					SetItemImage( l_idParent, FunctionsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded);
				}
		*/
	}
}



wxString FunctionsTree::_buildFunctionHelper( const wxString & p_function, VariableBaseType p_returnValue,
		std::vector <VariableBaseType> p_params )
{
	wxString l_data = ( p_returnValue != EMVT_NULL ? wxString( VariableTypeManager::GetBaseTypeName( p_returnValue ).c_str(), wxMBConvLibc() ) : wxString( wxT( "" ) ) ) + wxT( " " )  + p_function + wxT( "( " );

	if ( p_params.size() > 0 )
	{
		l_data << ( p_params[0] != EMVT_NULL ? wxString( VariableTypeManager::GetBaseTypeName( p_params[0] ).c_str(), wxMBConvLibc() ) : wxString( wxT( "" ) ) );

		for ( size_t i = 1 ; i < p_params.size() ; i++ )
		{
			if ( p_params[i] != EMVT_NULL )
			{
				l_data << wxT( ", " ) << wxString( VariableTypeManager::GetBaseTypeName( p_params[i] ).c_str(), wxMBConvLibc() );
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

	g_mainFrame->LogDebugMessage( wxString( p_name ) + wxT( "(" ) + l_text + wxT( ")" ) );
}



void FunctionsTree::_onItemActivated( wxTreeEvent & p_event )
{
//	std::cout << "FunctionsTree::_onItemActivated\n";
	if ( g_mainFrame->m_editText == NULL )
	{
		return;
	}

	m_selectedItem = p_event.GetItem();
	SelectItem( m_selectedItem );
	size_t l_lastnull;
	wxString l_ajout;
	wxTreeItemId l_itemId = p_event.GetItem();
	TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( l_itemId ) );
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

//TODO	g_mainFrame->m_editText->WriteText( l_itemdesc + l_ajout + wxT( "\r\n"));
}



void FunctionsTree::_onItemRClick( wxTreeEvent & p_event )
{
	m_selectedItem = p_event.GetItem();
	SelectItem( m_selectedItem );
}



void FunctionsTree::_onDragBegin( wxTreeEvent & p_event )
{
	/*
		wxTextDataObject my_data( "This text will be dragged.");
		wxDropSource dragSource( this);
		dragSource.SetData( my_data);
		dragSource.DoDragDrop( TRUE);
	*/
}



void FunctionsTree::_onDragEnd( wxTreeEvent & p_event )
{
}



void FunctionsTree::_onShowToolTip( wxTreeEvent & p_event )
{
	wxTreeItemId l_item = p_event.GetItem();
	MapType::iterator l_it = g_mainFrame->m_map.find( GetItemText( l_item ) );

	if ( l_it != g_mainFrame->m_map.end() )
	{
//		std::cout << "FunctionsTree::_onShowToolTip - [" << l_it->second << "]\n";
		p_event.SetToolTip( l_it->second );
	}
}
