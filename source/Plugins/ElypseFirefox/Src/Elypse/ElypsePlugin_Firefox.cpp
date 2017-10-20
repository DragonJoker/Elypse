#include "stdafx.h"

#include "ElypsePlugin_Firefox.h"

#include "main.h"
#include "NpApi/npruntime.h"

using namespace Ogre;

#if defined( _WIN32 )
#	include <Windows.h>
#	include <objidl.h>
#	include <gdiplus.h>
#	define BACKGROUND_COLOUR Gdiplus::Color( 255, 255, 255, 255)
#	define FONT_COLOUR Gdiplus::Color( 255, 0 , 0 , 0 )
#endif

ElypsePlugin_Firefox :: ElypsePlugin_Firefox()
	:	m_instance( NULL ),
		m_handle( NULL )
{

}

ElypsePlugin_Firefox :: ~ElypsePlugin_Firefox()
{

}

bool ElypsePlugin_Firefox :: OpenURL( const String & p_url , bool p_newWindow , bool p_usePostMethod )
{
	String l_url( p_url );
	NPError l_error;

	if ( l_url [ 0 ] == '.' && ( l_url[ 1 ] == '/' || l_url[ 1 ] == '\\' ) )
	{
		l_url = l_url.substr( 2 , l_url.size() );
		l_url = m_baseUrl + l_url;
	}


	if ( p_usePostMethod )
	{
		const StringArray & l_sVec = string::split( l_url , "?" , 1 );
		String l_postParams;

		if ( l_sVec.size() > 1 )
		{
			l_url = l_sVec[0];
			l_postParams = l_sVec[1];
		}

		l_postParams = String( "Content-Type:\tapplication/x-www-form-urlencoded\t\nContent-Length:\t" ) + ToString( l_postParams.size() ) + String( "\n\n" ) + l_postParams;
		l_error = NPN_PostURLNotify( m_instance , l_url.c_str() , "_blank" , l_postParams.size() , l_postParams.c_str(), false, 0 );
	}
	else
	{
		l_error = NPN_GetURL( m_instance , l_url.c_str() , "_blank" );
	}

	return ( l_error == NPERR_NO_ERROR );
}

void ElypsePlugin_Firefox :: MajorError( const String & p_errorText , const String & p_title )
{
	MessageBoxA( m_handle, p_errorText.c_str(), p_title.c_str(), 0 );
}

void ElypsePlugin_Firefox :: Quit()
{
	exit( 0 );
}

void ElypsePlugin_Firefox :: SetImagePath( const String & p_installPath )
{
	m_imagePath.assign( p_installPath.begin() , p_installPath.end() );
}

// Type conversions
NPString ToNPString( const String & qstr )
{
	NPString npstring;

	npstring.UTF8Length = qstr.length();
	npstring.UTF8Characters = ( char * )NPN_MemAlloc( qstr.length() );
	memcpy( ( char * )npstring.UTF8Characters, qstr.c_str(), qstr.length() );

	return npstring;
}

struct JavascriptExecutionContext
{
	NPString script;
	NPP instance;

	JavascriptExecutionContext( const String & p_script, NPP p_instance )
		:	script( ToNPString( p_script ) ),
			instance( p_instance )
	{
	}
};
extern NPNetscapeFuncs NPNFuncs;
void
NPN_PluginThreadAsyncCall( NPP instance, void ( *func )( void * ), void
						   *userData )
{
	//CallNPN_PluginThreadAsyncCallProc (
	//    NPNFuncs.pluginthreadasynccall,
	//    instance,
	//    func,
	//    userData);
}

void _executeJavascript( void * p_context )
{
	JavascriptExecutionContext * l_context = reinterpret_cast<JavascriptExecutionContext *>( p_context );

	NPObject * l_window;
	NPVariant l_result;
	l_result.type = NPVariantType_Void;
	l_result.value.objectValue = NULL;
	NPN_GetValue( l_context->instance, NPNVWindowNPObject, & l_window );

	NPN_Evaluate( l_context->instance, l_window, & l_context->script, & l_result );

	delete l_context;
}

bool ElypsePlugin_Firefox :: ExecuteJavascript( const String & p_functionName )
{
	JavascriptExecutionContext * l_context = new JavascriptExecutionContext( p_functionName, m_instance );
	NPN_PluginThreadAsyncCall( m_instance, _executeJavascript, l_context );
	return true;
}

#if ELYPSE_WINDOWS

void ElypsePlugin_Firefox :: InitGDI()
{
	Gdiplus::GdiplusStartupInput l_gdiplusStartupInput;
	GdiplusStartup( & m_gdiplusToken, & l_gdiplusStartupInput, NULL );
}

void ElypsePlugin_Firefox :: CloseGDI()
{
	Gdiplus::GdiplusShutdown( m_gdiplusToken );
}

void ElypsePlugin_Firefox :: PostChangeGraphicalStatus()
{
	InvalidateRect( m_handle, NULL, FALSE );
}

void ElypsePlugin_Firefox :: _printImage( const String & p_imagePath )
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

void ElypsePlugin_Firefox :: _printText( const String & p_text )
{
	PAINTSTRUCT  l_paintStruct;

	HDC l_hdc = BeginPaint( m_handle, & l_paintStruct );

	Gdiplus::Graphics l_graphics( l_hdc );

	Gdiplus::Font * l_font = new Gdiplus::Font( L"Times New Roman", 16 );
	Gdiplus::SolidBrush l_brush( FONT_COLOUR );

	l_graphics.DrawString( ToWString( p_text ).c_str(), p_text.length(), l_font, Gdiplus::PointF( 100, 100 ), &l_brush );

	EndPaint( m_handle, & l_paintStruct );
}

void ElypsePlugin_Firefox :: Paint()
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
		_printText( "Elypse Player : \nUne erreur est survenue\nVeuillez reinstaller l'application !" );
		break;
	}

	case StatusErrorUpdating :
	{
		_printImage( m_installPath / "cfg" / "bandeau_miseajour.jpg" );
		break;
	}

	case StatusErrorUnknown :
	{
//			_printImage( m_installPath / "cfg" / "sc�ne-introuvable.jpg");
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
#endif

void ElypsePlugin_Firefox :: ChangeCursorTo( CursorType p_cursorType )
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

