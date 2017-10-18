#include "SequenceProperties.h"

#include "Project/Temporal/Sequence.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			SequenceProperties::SequenceProperties( ProjectComponents::Temporal::TrollSequence & p_sequence )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_SEQUENCE }
				, m_sequence{ p_sequence }
			{
			}

			SequenceProperties::~SequenceProperties()
			{
			}

			void SequenceProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_SEQUENCE{ _( "Sequence: " ) };
				wxString const PROPERTY_SEQUENCE_NAME{ _( "Name" ) };

				DoAddCategory( PROPERTY_CATEGORY_SEQUENCE );
				DoAddProperty( PROPERTY_SEQUENCE_NAME, m_sequence.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_sequence.SetObjectName( p_property->GetValue() );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_sequence.GetObjectName() );
				} );
			}
		}
	}
}
