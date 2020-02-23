/*
See LICENSE file in root folder
*/
#include "GuiCommonPch.h"

namespace GuiCommon
{
	namespace
	{
		const wxString CONVERSION_ERROR = _( "Conversion Error" );
		const wxString RANGE_ERROR = _( "Range Error" );

		const wxString ERROR_FLOATING_POINT = _( "Please enter a floating point value" );
		const wxString ERROR_FLOATING_POINT_RANGE = _( "Please enter a floating point value in the range" );
		const wxString ERROR_INTEGER = _( "Please enter a numeric value" );
		const wxString ERROR_INTEGER_RANGE = _( "Please enter a numeric value in the range" );
	}

	wxString make_wxString( String const & p_value )
	{
		return make_wxString( p_value.c_str() );
	}

	wxString make_wxString( char const * p_value )
	{
		return wxString( p_value, wxMBConvLibc() );
	}

	String make_string( wxString const & p_in )
	{
		return p_in.char_str().data();
	}

	bool GetReal( wxString const & p_value, float & p_res )
	{
		bool l_return = false;
		double l_res = 0.0;
		wxString l_value = p_value;
		l_value.Replace( wxT( "," ), wxT( "." ) );

		if ( l_value.ToDouble( & l_res ) )
		{
			p_res = static_cast <float>( l_res );
			l_return = true;
		}
		else
		{
			wxMessageBox( ERROR_FLOATING_POINT, CONVERSION_ERROR );
		}

		return l_return;
	}

	bool GetReal( wxString const & p_value, float p_min, float p_max, float & p_res )
	{
		bool l_return = GetReal( p_value, p_res );

		if ( l_return )
		{
			l_return = p_res >= p_min && p_res <= p_max;

			if ( !l_return )
			{
				wxMessageBox( wxString( ERROR_FLOATING_POINT_RANGE ) << wxT( " [" ) << p_min << wxT( ", " ) << p_max << wxT( "]" ), RANGE_ERROR );
			}
		}

		return l_return;
	}

	bool GetInt( wxString const & p_value, int & p_res )
	{
		bool l_return = false;
		long l_res = 0;

		if ( p_value.ToLong( & l_res ) )
		{
			p_res = static_cast< int >( l_res );
			l_return = true;
		}
		else
		{
			wxMessageBox( ERROR_INTEGER, CONVERSION_ERROR );
		}

		return l_return;
	}

	bool GetInt( wxString const & p_value, int p_min, int p_max, int & p_res )
	{
		bool l_return = GetInt( p_value, p_res );

		if ( l_return )
		{
			l_return = p_res >= p_min && p_res <= p_max;

			if ( !l_return )
			{
				wxMessageBox( wxString( ERROR_INTEGER_RANGE ) << wxT( " [" ) << p_min << wxT( ", " ) << p_max << wxT( "]" ), RANGE_ERROR );
			}
		}

		return l_return;
	}

	bool GetReal( String const & p_value, float & p_res )
	{
		return GetReal( make_wxString( p_value ), p_res );
	}

	bool GetReal( String const & p_value, float p_min, float p_max, float & p_res )
	{
		return GetReal( make_wxString( p_value ), p_min, p_max, p_res );
	}

	bool GetInt( String const & p_value, int & p_res )
	{
		return GetInt( make_wxString( p_value.c_str() ), p_res );
	}

	bool GetInt( String const & p_value, int p_min, int p_max, int & p_res )
	{
		return GetInt( make_wxString( p_value ), p_min, p_max, p_res );
	}
}
