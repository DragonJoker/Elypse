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
#include "PrecompiledHeader.h"

//#include <afxinet.h>

#include "ElypsePlugin.h"

#include "ScriptNode.h"
#include "ScriptEngine.h"
#include "ElypseFrameListener.h"
#include "ElypseInstance.h"
#include "ElypseLogs.h"

#include <thread>
#include <NeoCurl.h>

struct StringFromUrlLauncher
{
	String url;
	String postParams;
	ScriptNode * caller;
	ScriptNode * execAtEnd;
	ElypsePlugin * instance;

	void operator()()
	{
		String l_string = instance->GetStringFromUrl( url, postParams );
		caller->get<String>() = l_string;
		instance->GetInstance()->GetFL()->AddToScriptQueue( execAtEnd );
	}
};

ElypsePlugin::ElypsePlugin()
	: m_graphicalStatus( StatusNone )
	, m_curl( new CURLManager )
	, m_baseCursor( ArrowCursor )
{
//	m_curl->Initialise();
}

ElypsePlugin::~ElypsePlugin()
{
	delete m_curl;
}

void ElypsePlugin::SetSessionCookie( String const & p_cookieParams )
{
	auto l_lock = make_unique_lock( m_mutex );
//	std::cout <<"SetSessionCookie : " << p_cookieParams << std::endl;
	CURLcode l_code = m_curl->SetCookieString( p_cookieParams.c_str() );

	if ( l_code != CURLE_OK )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "ElypsePlugin::SetSessionCookie -> error in curl : " + CURLManager::CurlError( l_code ) );
	}
}

String ElypsePlugin::GetStringFromUrl( String const & p_url, String const & p_postParams )
{
	auto l_lock = make_unique_lock( m_mutex );
	String l_contents;
	CURLcode l_code = m_curl->GetStringFromUrl( p_url, l_contents, p_postParams );

	if ( l_code != CURLE_OK )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "ElypsePlugin::GetStringFromUrl -> error in curl : " + CURLManager::CurlError( l_code ) );
	}

	return l_contents;
}

void ElypsePlugin::ThreadedStringFromUrl( String const & p_url, const String  & p_postParams,
		ScriptNode * p_caller, ScriptNode * p_execAtAnd )
{
	genlib_assert( p_caller != NULL );
	StringFromUrlLauncher l_launcher;
	l_launcher.url = p_url;
	l_launcher.postParams = p_postParams;
	l_launcher.caller = p_caller;
	l_launcher.execAtEnd = p_execAtAnd;
	l_launcher.instance = this;
	std::thread * l_thread;
	l_thread = new std::thread( l_launcher );
	delete l_thread;
}
