/*
See LICENSE file in root folder
*/
#ifndef ___TEOBJECT_H___
#define ___TEOBJECT_H___

#include "Module_Project.h"

#include "Displayable.h"

namespace Troll
{
	namespace ProjectComponents
	{
		class Object
			: public Displayable
		{
		public:
			Object( wxString const & p_name, wxString const & p_fileName, ObjectType p_type );

			inline ObjectType GetObjectType()const
			{
				return m_objectType;
			}

			inline wxString const & GetObjectName()const
			{
				return m_name;
			}

			inline State GetState()const
			{
				return m_state;
			}

			inline bool IsVisible()const
			{
				return m_visible;
			}

			inline wxString const & GetFileName()const
			{
				return m_fileName;
			}

			inline void SetObjectName( wxString const & p_name )
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

			inline void SetFileName( wxString const & p_name )
			{
				m_fileName = p_name;
			}

		protected:
			wxString m_name;
			State m_state;
			bool m_visible;
			wxString m_fileName;
			ObjectType m_objectType;
		};
	}
}

#endif
