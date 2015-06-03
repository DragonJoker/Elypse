#include "PrecompiledHeader.h"

#include "Displayable.h"

#include "GUI/ObjectProperties/ObjectProperties.h"

using namespace Troll;


TEDisplayable::TEDisplayable()
	:	m_propertiesPanel( NULL )
{
}



TEDisplayable::~TEDisplayable()
{
	if ( m_propertiesPanel != NULL )
	{
		delete m_propertiesPanel;
	}
}



void TEDisplayable::ShowProperties()
{
	if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->Show( true );
	}
}



void TEDisplayable::HideProperties()
{
	if ( m_propertiesPanel != NULL )
	{
		m_propertiesPanel->Show( false );
		m_propertiesPanel->GetParent()->RemoveChild( m_propertiesPanel );
		delete m_propertiesPanel;
		m_propertiesPanel = NULL;
	}
}


