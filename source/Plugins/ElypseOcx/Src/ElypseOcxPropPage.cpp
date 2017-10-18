// ElypseOcxPropPage.cpp : Implementation of the CElypseOcxPropPage property page class.
#include "ElypseOcxPch.h"

#include "ElypseOcx.h"
#include "ElypseOcxPropPage.h"


#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE( CElypseOcxPropPage, COlePropertyPage )

// Message map

BEGIN_MESSAGE_MAP( CElypseOcxPropPage, COlePropertyPage )
END_MESSAGE_MAP()

// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX( CElypseOcxPropPage, "Elypse.ElypseOcxPropPage.1", 0X50CF853E, 0X9C78, 0X4F68, 0X86, 0XBA, 0X19, 0X4D, 0X6C, 0X78, 0X31, 0X05 )

// CElypseOcxPropPage::CElypseOcxPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CElypseOcxPropPage
BOOL CElypseOcxPropPage::CElypseOcxPropPageFactory::UpdateRegistry( BOOL bRegister )
{
	if ( bRegister )
	{
		return AfxOleRegisterPropertyPageClass( AfxGetInstanceHandle(), m_clsid, IDS_ELYPSEPLAYER_PPG );
	}
	else
	{
		return AfxOleUnregisterClass( m_clsid, NULL );
	}
}

// CElypseOcxPropPage::CElypseOcxPropPage - Constructor
CElypseOcxPropPage::CElypseOcxPropPage()
	: COlePropertyPage( IDD, IDS_ELYPSEPLAYER_PPG_CAPTION )
{
}

// CElypseOcxPropPage::DoDataExchange - Moves data between page and properties
void CElypseOcxPropPage::DoDataExchange( CDataExchange * pDX )
{
	DDP_PostProcessing( pDX );
}

// CElypseOcxPropPage message handlers
