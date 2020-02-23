// ElypseOcx.cpp : Implementation of CElypseOcxApp and DLL registration.

#include "ElypseOcxPch.h"

#include "ElypseOcx.h"

#include "ElypseOcxCtrl.h"
#include "ElypseOcxPropPage.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#endif

CElypseOcxApp theApp;

// {EA1FD3FC-EBF5-4FBC-A9B8-910944657E97}
const GUID CDECL BASED_CODE _tlid =
{ 0XEA1FD3FC, 0XEBF5, 0X4FBC, { 0XA9, 0XB8, 0X91, 0X09, 0X44, 0X65, 0X7E, 0X97 } };

// {C528A14C-C066-4FDB-A9BF-D4B6625A0A61}
const GUID CDECL BASED_CODE _ctlid =
{ 0XC528A14C, 0XC066, 0X4FDB, { 0XA9, 0XBF, 0XD4, 0XB6, 0X62, 0X5A, 0X0A, 0X61 } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

const CATID CLSID_SafeItem =
{ 0X9064133A, 0XF54E, 0X48A8, {0XAB, 0XDD, 0XCE, 0XD1, 0XFF, 0X4A, 0X18, 0X76 } };
const CATID CATID_SafeForScripting =
{ 0X7DD95801, 0X9882, 0X11CF, {0X9F, 0XA9, 0X00, 0XAA, 0X00, 0X6C, 0X42, 0XC4 } };
const CATID CATID_SafeForInitializing  =
{ 0X7DD95802, 0X9882, 0X11CF, {0X9F, 0XA9, 0X00, 0XAA, 0X00, 0X6C, 0X42, 0XC4 } };

//********************************************************************************************************************

// CElypseOcxApp::InitInstance - DLL initialization

BOOL CElypseOcxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	return bInit;
}

//********************************************************************************************************************

// CElypseOcxApp::ExitInstance - DLL termination

int CElypseOcxApp::ExitInstance()
{
	return COleControlModule::ExitInstance();
}

//********************************************************************************************************************

// HRESULT CreateComponentCategory -

HRESULT CreateComponentCategory( CATID catid, WCHAR * catDescription )
{
	ICatRegister * pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance( CLSID_StdComponentCategoriesMgr,
						   NULL,
						   CLSCTX_INPROC_SERVER,
						   IID_ICatRegister,
						   ( void ** )&pcr );

	if ( FAILED( hr ) )
	{
		return hr;
	}

	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english

	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is
	int len = ( int )wcslen( catDescription );

	if ( len > 127 )
	{
		len = 127;
	}

	wcsncpy_s( catinfo.szDescription, catDescription, len );
	// Make sure the description is null terminated
	catinfo.szDescription[len] = '\0';

	hr = pcr->RegisterCategories( 1, &catinfo );
	pcr->Release();

	return hr;
}

//********************************************************************************************************************

// HRESULT RegisterCLSIDInCategory - Add entries to registry

HRESULT RegisterCLSIDInCategory( REFCLSID clsid, CATID catid )
{
	// Register your component categories information.
	ICatRegister * pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance( CLSID_StdComponentCategoriesMgr,
						   NULL,
						   CLSCTX_INPROC_SERVER,
						   IID_ICatRegister,
						   ( void ** )&pcr );

	if ( SUCCEEDED( hr ) )
	{
		// Register this category as being "implemented" by
		// the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories( clsid, 1, rgcatid );
	}

	if ( pcr != NULL )
	{
		pcr->Release();
	}

	return hr;
}

//********************************************************************************************************************

// HRESULT UnRegisterCLSIDInCategory - Remove entries from the registry

HRESULT UnRegisterCLSIDInCategory( REFCLSID clsid, CATID catid )
{
	ICatRegister * pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance( CLSID_StdComponentCategoriesMgr,
						   NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, ( void ** )&pcr );

	if ( SUCCEEDED( hr ) )
	{
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories( clsid, 1, rgcatid );
	}


	if ( pcr != NULL )
	{
		pcr->Release();
	}

	return hr;
}

//********************************************************************************************************************

// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer( void )
{
	AFX_MANAGE_STATE( _afxModuleAddrThis );

	/*
		if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid,_T("./EMuse.tlb"),NULL))
			return ResultFromScode(SELFREG_E_TYPELIB);
	*/
	if ( !COleObjectFactoryEx::UpdateRegistryAll( TRUE ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}


	if ( FAILED( CreateComponentCategory(
					 CATID_SafeForScripting,
					 L"Controls that are safely scriptable" ) ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}

	if ( FAILED( CreateComponentCategory(
					 CATID_SafeForInitializing,
					 L"Controls safely initializable from persistent data" ) ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}

	if ( FAILED( RegisterCLSIDInCategory(
					 _ctlid, CATID_SafeForScripting ) ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}

	if ( FAILED( RegisterCLSIDInCategory(
					 _ctlid, CATID_SafeForInitializing ) ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}


	return NOERROR;
}

//********************************************************************************************************************

// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer( void )
{
	AFX_MANAGE_STATE( _afxModuleAddrThis );

	if ( !AfxOleUnregisterTypeLib( _tlid, _wVerMajor, _wVerMinor ) )
	{
		return ResultFromScode( SELFREG_E_TYPELIB );
	}

	if ( !COleObjectFactoryEx::UpdateRegistryAll( FALSE ) )
	{
		return ResultFromScode( SELFREG_E_CLASS );
	}

	return NOERROR;
}

//********************************************************************************************************************

IMPLEMENT_DYNCREATE( ElypseOcxCtrl, COleControl )

BEGIN_MESSAGE_MAP( ElypseOcxCtrl, COleControl )
	ON_OLEVERB( AFX_IDS_VERB_PROPERTIES, OnProperties )
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SYSKEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_CHAR()
	ON_WM_DEADCHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_MESSAGE( WM_USER + 1, OnExecJS )
END_MESSAGE_MAP()


IMPLEMENT_OLECREATE_EX( ElypseOcxCtrl, "ELYPSE.ElypsePlayerCtrl.1",
						0XC528A14C, 0XC066, 0X4FDB, 0XA9, 0XBF, 0XD4, 0XB6, 0X62, 0X5A, 0X0A, 0X61 )

BEGIN_DISPATCH_MAP( ElypseOcxCtrl, COleControl )
	DISP_FUNCTION( ElypseOcxCtrl, "ExecuteScript", DoExecuteScript, VTS_NONE, VTS_BSTR )
//DISP_FUNCTION_ID(ElypseOcxCtrl, "StartAnimation", 11, ExecuteScript, VT_I4, VTS_BSTR)
END_DISPATCH_MAP()

BEGIN_EVENT_MAP( ElypseOcxCtrl, COleControl )
END_EVENT_MAP()

BEGIN_PROPPAGEIDS( ElypseOcxCtrl, 1 )
	PROPPAGEID( CElypseOcxPropPage::guid )
END_PROPPAGEIDS( ElypseOcxCtrl )

IMPLEMENT_OLETYPELIB( ElypseOcxCtrl, _tlid, _wVerMajor, _wVerMinor )

static const DWORD BASED_CODE _dwElypseOcxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;



IMPLEMENT_OLECTLTYPE( ElypseOcxCtrl, IDS_ELYPSEPLAYER, _dwElypseOcxOleMisc )


BOOL ElypseOcxCtrl::ElypseOcxCtrlFactory::UpdateRegistry( BOOL bRegister )
{
//	EMUSE_MESSAGE_DEBUG( "ElypseOcxCtrl::ElypseOcxCtrlFactory::UpdateRegistry" );
	if ( bRegister )
	{
		return AfxOleRegisterControlClass(
				   AfxGetInstanceHandle(),
				   m_clsid,
				   m_lpszProgID,
				   IDS_ELYPSEPLAYER,
				   IDS_ELYPSEPLAYER,
				   afxRegApartmentThreading,
				   _dwElypseOcxOleMisc,
				   _tlid,
				   _wVerMajor,
				   _wVerMinor );
	}
	else
	{
		return AfxOleUnregisterClass( m_clsid, m_lpszProgID );
	}
}


DWORD ElypseOcxCtrl::GetControlFlags()
{
	return COleControl::GetControlFlags() | noFlickerActivate & ~clipPaintDC;
}

void ElypseOcxCtrl::OnResetState()
{
	COleControl::OnResetState();
}
