/*
See LICENSE file in root folder
*/
#ifndef ___MUSE_READER_PCH___
#define ___MUSE_READER_PCH___

#include <cstdint>
#include <memory>

#pragma warning( push )
#pragma warning( disable:4996 )

#include <wx/app.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/listbox.h>
#include <wx/protocol/ftp.h>
#include <wx/protocol/http.h>
#include <wx/socket.h>
#include <wx/stattext.h>
#include <wx/stream.h>
#include <wx/textctrl.h>

wxString make_wxString( std::string const & p_value );
wxString make_wxString( char const * p_value );

std::string make_string( wxString const & p_value );
std::string make_string( wxChar const * p_value );

#pragma warning( pop )

#endif
