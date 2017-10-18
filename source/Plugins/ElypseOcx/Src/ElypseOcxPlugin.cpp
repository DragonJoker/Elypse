#include "ElypseOcxPch.h"

#include "ElypseOcxPlugin.h"

#include <winerror.h>
#include <afxdisp.h>
#include <afxinet.h>
#include <Mshtml.h>

#include <ScriptNode.h>
#include <VariableType.h>

#include <NeoCurl.h>
#include <StringConverter.h>
#include <StringUtils.h>

using namespace General::Utils;

#define BACKGROUND_COLOUR Gdiplus::Color( 255, 255, 255, 255)
#define FONT_COLOUR Gdiplus::Color( 255, 0 , 0 , 0 )

namespace Elypse
{
	namespace OCX
	{
		ElypseOcxPlugin::ElypseOcxPlugin()
			: m_handle( 0 )
			, m_protectedMode( false )
			, m_jsScriptExcutionQueue( 20 )
		{
		}

		ElypseOcxPlugin::~ElypseOcxPlugin()
		{

		}

		bool ElypseOcxPlugin::OpenURL( const String & p_url, bool p_newWindow , bool p_usePostMethod )
		{
			DWORD dwFlags = 0;
			HRESULT hr;
			IWebBrowser2 * l_browser;
			String l_postParams, l_url;
			BSTR bstrURL;
			COleSafeArray vPostData;

			l_url = p_url;

			if ( l_url [ 0 ] == '.' && ( l_url[ 1 ] == '/' || l_url[ 1 ] == '\\' ) )
			{
				l_url = l_url.substr( 2 , l_url.size() );
				l_url = m_baseUrl + l_url;
			}

			if ( p_usePostMethod )
			{
				std::vector< String > l_sVec = string::split( l_url , "?" , 1 );

				if ( l_sVec.size() > 1 )
				{
					l_url = l_sVec[0];
					l_postParams = l_sVec[1];
				}
			}

			CString strURL( l_url.c_str() );

			bstrURL = strURL.AllocSysString();

			if ( p_usePostMethod && !l_postParams.empty() )
			{
				vPostData.CreateOneDim( VT_UI1, static_cast <DWORD>( l_postParams.size() ), ( void * )l_postParams.c_str() );
			}

			hr = CoCreateInstance( CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, ( LPVOID * )&l_browser );

			if ( !SUCCEEDED( hr ) )
			{
				//MajorError("","two");
				return false;
			}


			hr = l_browser->Navigate( bstrURL, COleVariant
									  ( ( long ) dwFlags, VT_I4 ),
									  COleVariant( "_self",
												   VT_BSTR ), vPostData,
									  COleVariant
									  ( "Content-Type: application/x-www-form-urlencoded\n\r", VT_BSTR ) );

			if ( !SUCCEEDED( hr ) )
			{
				return false;
			}

			hr = l_browser->put_Visible( VARIANT_TRUE );
			l_browser->Release();
			OleUninitialize();
			SysFreeString( bstrURL );

			return true;
		}

		void ElypseOcxPlugin::MajorError( const String & p_errorText , const String & p_title )
		{
			MessageBox( m_handle , p_errorText.c_str() , p_title.c_str() , MB_ICONEXCLAMATION );
		}

		void ElypseOcxPlugin::Quit()
		{
			exit( 0 );
		}

		void ElypseOcxPlugin::InitGDI()
		{
			GdiplusStartup( & m_gdiplusToken, & m_gdiplusStartupInput, NULL );
		}

		void ElypseOcxPlugin::CloseGDI()
		{
			Gdiplus::GdiplusShutdown( m_gdiplusToken );
		}

		void ElypseOcxPlugin::PostChangeGraphicalStatus()
		{
			InvalidateRect( m_handle, NULL, FALSE );
		}

		void ElypseOcxPlugin::SetClientSite( IOleClientSite * p_clientSite )
		{
			m_clientSite = p_clientSite;

			CComQIPtr<IServiceProvider, & IID_IServiceProvider> spDisp( m_clientSite );

			HRESULT hr = spDisp->QueryService( IID_IWebBrowserApp, IID_IWebBrowser, reinterpret_cast <void **>( & m_spDoc ) );

			if ( FAILED( hr ) )
			{
				std::cout << "ooooooooohhh::fuck " << std::endl;
			}
		}

		void ElypseOcxPlugin::_convertNodeToVariant( ScriptNode * p_node, VARIANT * p_variant )
		{
			switch ( p_node->GetType()->GetBase() )
			{
			/*
			case EMVT_INT:
			{
				p_variant.intVal = p_node->get<int>();
				p_variant.vt = VT_I4;
			}
			break;
			case EMVT_REAL:
			{
				p_variant.fltVal = p_node->get<Real>();
				p_variant.vt = VT_R4;
			}
			break;
			*/
			case EMVT_STRING:
			{
				CString l_l( "yayaya" );
				CComBSTR bstr = l_l;
				std::wcout << bstr << std::endl;
				bstr.CopyTo( & p_variant->bstrVal );
				p_variant->vt = VT_BSTR;
			}
			break;
			}
		}

		void ElypseOcxPlugin::ExecJS()
		{
			if ( m_jsScriptExcutionQueue.IsEmpty() )
			{
				return;
			}

			CComPtr<IDispatch> disp;
			CComPtr<IHTMLDocument2> document;
			IHTMLWindow2 * l_win;
			VARIANT l_resultat;

			HRESULT hr = m_spDoc->get_Document( & disp );

			hr = disp->QueryInterface( & document );

			if ( FAILED( hr ) )
			{
				std::cout << "failed " << std::hex << hr << std::dec << " \n";
			}

			document->get_parentWindow( & l_win );


			while ( ! m_jsScriptExcutionQueue.IsEmpty() )
			{
				String l_script = m_jsScriptExcutionQueue.Pop();
				hr = l_win->execScript( ( BSTR )( ToWString( l_script ).c_str() ), L"JScript", &l_resultat );

				if ( FAILED( hr ) )
				{
					std::cout << "FAILED : " << std::hex << hr << std::dec << std::endl;
				}
				else
				{
					std::cout << "SUCCESS : " << hr << std::endl;
				}
			}
		}

		bool ElypseOcxPlugin::ExecuteJavascript( const String & p_functionName )
		{
			if ( m_jsScriptExcutionQueue.IsFull() )
			{
				return false;
			}

			m_jsScriptExcutionQueue.Push( p_functionName );

			PostMessage( m_handle, WM_USER + 1, 0, 0 );

			return true;
		}

		void ElypseOcxPlugin::_printImage( const String & p_imagePath )
		{
			PAINTSTRUCT  l_paintStruct;

			HDC l_hdc = BeginPaint( m_handle, & l_paintStruct );

			Gdiplus::Graphics l_graphics( l_hdc );

			Gdiplus::SolidBrush l_backgroundBrush( BACKGROUND_COLOUR );

			l_graphics.FillRectangle( & l_backgroundBrush, 0, 0, m_width, m_height );

			Gdiplus::Image l_image( ToWString( p_imagePath ).c_str() );

			Gdiplus::PointF l_imageStart( ( m_width - l_image.GetWidth() ) / 2.0f, ( m_height - l_image.GetHeight() ) / 2.0f );

			l_graphics.DrawImage( & l_image, l_imageStart );

			EndPaint( m_handle, & l_paintStruct );
		}

		void ElypseOcxPlugin::_printText( const String & p_text )
		{
			PAINTSTRUCT  l_paintStruct;

			HDC l_hdc = BeginPaint( m_handle, & l_paintStruct );

			Gdiplus::Graphics l_graphics( l_hdc );

			Gdiplus::Font * l_font = new Gdiplus::Font( L"Times New Roman", 16 );
			Gdiplus::SolidBrush l_brush( FONT_COLOUR );

			l_graphics.DrawString( ToWString( p_text ).c_str(), INT( p_text.length() ), l_font, Gdiplus::PointF( 100, 100 ), &l_brush );

			EndPaint( m_handle, & l_paintStruct );
		}

		void ElypseOcxPlugin::Paint()
		{
			switch ( m_graphicalStatus )
			{
			case StatusErrorRenderer :
			{
				_printImage( m_installPath / "cfg" / "bandeau_probgraph.jpg" );
				break;
			}

			case StatusErrorOgre :
			{
				_printImage( m_installPath / "cfg" / "bandeau_reinstal.jpg" );
				break;
			}

			case StatusErrorDirectories :
			{
				_printText( "Elypse Player : \nUne erreur est survenue\nVeuillez reinstaller l'application !\nwww.fdssoftmedia.com" );
				break;
			}

			case StatusErrorUnknown :
			{
				_printImage( m_installPath / "cfg" / "sc�ne-introuvable.jpg" );
				break;
			}

			case StatusErrorUpdating :
			{
				_printImage( m_installPath / "cfg" / "bandeau_miseajour.jpg" );
				break;
			}

			/*
			case StatusLoading :
			{
				_printImage( m_installPath / "cfg" / "chargement-en-cours-vide.jpg");
				break;
			}*/
			case StatusNone :
			default:
				break;
			}
		}

		void ElypseOcxPlugin::ChangeCursorTo( CursorType p_cursorType )
		{
			switch ( p_cursorType )
			{
			case TextCursor:
				SetCursor( LoadCursor( NULL, IDC_IBEAM ) );
				break;

			case HandCursor:
				SetCursor( LoadCursor( NULL, IDC_HAND ) );
				break;

			case SizeNSCursor:
				SetCursor( LoadCursor( NULL, IDC_SIZENS ) );
				break;

			case SizeWECursor:
				SetCursor( LoadCursor( NULL, IDC_SIZEWE ) );
				break;

			default:
				SetCursor( LoadCursor( NULL, IDC_ARROW ) );
				break;
			}
		}
	}
}
