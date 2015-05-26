#include "PrecompiledHeader.h"

//#include <afxinet.h>

#include "EMusePlugin.h"

#include "ScriptNode.h"
#include "ScriptEngine.h"
#include "EMuseFrameListener.h"
#include "EMuseInstance.h"
#include "EMuseLogs.h"

#include <Thread.h>
#include <NeoCurl.h>

struct StringFromUrlLauncher
{
	String url;
	String postParams;
	ScriptNode * caller;
	ScriptNode * execAtEnd;
	EMusePlugin * instance;

	void operator()()
	{
		String l_string = instance->GetStringFromUrl( url, postParams );
		caller->get<String>() = l_string;
		instance->GetInstance()->GetFL()->AddToScriptQueue( execAtEnd );
	}
};

EMusePlugin :: EMusePlugin()
	:	m_graphicalStatus( StatusNone ),
		m_curl( new CURLManager ),
		m_baseCursor( ArrowCursor )
{
//	m_curl->Initialise();
}

EMusePlugin :: ~EMusePlugin()
{
	delete m_curl;
}

void EMusePlugin :: SetSessionCookie( const String & p_cookieParams )
{
	GENLIB_AUTO_SCOPED_LOCK();
//	std::cout <<"SetSessionCookie : " << p_cookieParams << std::endl;
	CURLcode l_code =  m_curl->SetCookieString( p_cookieParams.c_str() );

	if ( l_code != CURLE_OK )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "EMusePlugin :: SetSessionCookie -> error in curl : " + CURLManager::CurlError( l_code ) );
	}
}

String EMusePlugin :: GetStringFromUrl( const String & p_url, const String & p_postParams )
{
	GENLIB_AUTO_SCOPED_LOCK();
	String l_contents;
	CURLcode l_code =  m_curl->GetStringFromUrl( p_url, l_contents, p_postParams );

	if ( l_code != CURLE_OK )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "EMusePlugin :: GetStringFromUrl -> error in curl : " + CURLManager::CurlError( l_code ) );
	}

	return l_contents;
}

void EMusePlugin :: ThreadedStringFromUrl(	const String & p_url, const String  & p_postParams,
		ScriptNode * p_caller, ScriptNode * p_execAtAnd )
{
	genlib_assert( p_caller != NULL );
	StringFromUrlLauncher l_launcher;
	l_launcher.url = p_url;
	l_launcher.postParams = p_postParams;
	l_launcher.caller = p_caller;
	l_launcher.execAtEnd = p_execAtAnd;
	l_launcher.instance = this;
	Thread * l_thread;
	l_thread = new Thread( l_launcher );
	delete l_thread;
}
