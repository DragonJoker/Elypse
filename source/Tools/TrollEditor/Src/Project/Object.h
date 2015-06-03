
#ifndef ___TEOBJECT_H___
#define ___TEOBJECT_H___

#include "Module_Project.h"
#include "Displayable.h"

namespace Troll
{
	class TrollObject
		: public TEDisplayable
	{
	protected:
		wxString m_name;
		State m_state;
		bool m_visible;
		wxString m_fileName;
		ObjectType m_objectType;

	public:
		TrollObject( const wxString & p_name, const wxString & p_fileName, ObjectType p_type );

	public:
		inline ObjectType		GetObjectType()const
		{
			return m_objectType;
		}

		inline const wxString & GetName()const
		{
			return m_name;
		}
		inline State			GetState()const
		{
			return m_state;
		}
		inline bool				IsVisible()const
		{
			return m_visible;
		}
		inline const wxString &	GetFileName()const
		{
			return m_fileName;
		}

		inline void SetName( const wxString & p_name )
		{
			m_name = p_name;
		}
		inline void SetState( State p_state )
		{
			m_state = p_state;
		}
		inline void SetVisible( bool p_visible )
		{
			m_visible = p_visible;
		}
		inline void SetFileName( const wxString & p_name )
		{
			m_fileName = p_name;
		}
	};
}

#endif

