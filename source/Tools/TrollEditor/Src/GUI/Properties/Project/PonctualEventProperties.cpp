#include "PonctualEventProperties.h"

#include "Project/Temporal/PonctualEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"

#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			PonctualEventProperties::PonctualEventProperties( ProjectComponents::Temporal::TrollPonctualEvent & p_event )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_PONCTUAL_EVENT }
				, m_event{ p_event }
			{
			}

			PonctualEventProperties::~PonctualEventProperties()
			{
			}

			void PonctualEventProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_PONCTUAL_EVENT{ _( "Ponctual Event: " ) };
				wxString const PROPERTY_PONCTUAL_EVENT_ACTION{ _( "Action" ) };
				wxString const PROPERTY_PONCTUAL_EVENT_TARGET_TYPE{ _( "Target Type" ) };
				wxString const PROPERTY_PONCTUAL_EVENT_TIME{ _( "Time" ) };
				wxString const PROPERTY_PONCTUAL_EVENT_TARGET{ _( "Target" ) };

				wxArrayString l_actions{ make_wxArrayString( ScriptEngine::GetContext()->sequenceManager->GetAvailablePonctualEvents() ) };
				wxArrayString l_targetTypes{ DoGetTargetTypes( m_event.GetObjectName() ) };

				DoAddCategory( PROPERTY_CATEGORY_PONCTUAL_EVENT );
				m_targetType = DoAddProperty( PROPERTY_PONCTUAL_EVENT_TIME, m_event.GetFireTime(), [this]( wxPGProperty * p_property )
				{
					m_event.SetFireTime( float( double(  p_property->GetValue() ) ) );
				},
				[this]( wxPGProperty * p_property )
				{
					p_property->SetValue( m_event.GetFireTime() );
				} );
				DoAddProperty( PROPERTY_PONCTUAL_EVENT_ACTION, l_actions, m_event.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_event.SetActionName( p_property->GetValue() );
					DoUpdateTargetType();
				} );
				m_targetType = DoAddProperty( PROPERTY_PONCTUAL_EVENT_TARGET_TYPE, l_targetTypes, m_event.GetTargetType(), [this]( wxPGProperty * p_property )
				{
					m_event.SetTargetType( p_property->GetValue() );
					DoUpdateTarget();
				} );
				m_target = DoAddProperty( PROPERTY_PONCTUAL_EVENT_TARGET, DoGetTargetsList(), m_event.GetTarget(), [this]( wxPGProperty * p_property )
				{
					m_event.SetTarget( p_property->GetValue() );
				} );
				DoCreateValueProperties();
			}

			wxArrayString PonctualEventProperties::DoGetTargetTypes( wxString const & p_action )
			{
				wxArrayString l_result;

				if ( p_action.find( wxT( "SceneNode" ) ) != wxString::npos )
				{
					l_result.push_back( wxT( "scene_node" ) );
				}
				else if ( p_action.find( wxT( "Function" ) ) != wxString::npos )
				{
					l_result.push_back( wxT( "function" ) );
				}
				else if ( p_action.find( wxT( "Overlay_Hide" ) ) != wxString::npos || p_action.find( wxT( "Overlay_Show" ) ) != wxString::npos )
				{
					l_result.push_back( wxT( "overlay" ) );
				}
				else if ( p_action.find( wxT( "Overlay" ) ) != wxString::npos )
				{
					l_result.push_back( wxT( "overlayElement" ) );
				}
				else if ( p_action.find( wxT( "Sequence" ) ) != wxString::npos )
				{
					l_result.push_back( wxT( "sequence" ) );
				}

				return l_result;
			}

			void PonctualEventProperties::DoCreateValueProperties()
			{
				wxString const PROPERTY_PONCTUAL_EVENT_VALUE{ _( "Value" ) };
				wxString const l_targetType = m_event.GetTargetType();

				if ( l_targetType == wxT( "scene_node" ) )
				{
					Vector3 l_value;
					auto l_params = StringUtil::split( make_string( m_event.GetActionArgs() ), ", \t" );

					if ( l_params.size() >= 3 && GetReal( l_params[0], l_value.x ) && GetReal( l_params[1], l_value.y ) && GetReal( l_params[2], l_value.z ) )
					{
						DoAddProperty< Vector3Names::Point >( PROPERTY_PONCTUAL_EVENT_VALUE, l_value, [this]( wxPGProperty * p_property )
						{
							Vector3 l_value = Vector3RefFromVariant( p_property->GetValue() );
							m_event.SetActionArgs( wxString() << l_value.x << " " << l_value.y << " " << l_value.z );
						},
						[this]( wxPGProperty * p_property )
						{
							Vector3 l_value;
							Ogre::StringVector l_split = Ogre::StringUtil::split( make_string( m_event.GetActionArgs() ), ", \t", 2 );
							if ( l_split.size() >= 3 && GetReal( l_split[0], l_value.x ) && GetReal( l_split[1], l_value.y ) && GetReal( l_split[1], l_value.z ) )
							{
								p_property->SetValue( WXVARIANT( l_value ) );
							}
						} );
					}
				}
			}

			void PonctualEventProperties::DoUpdateTargetType()
			{
				if ( m_targetType )
				{
					wxArrayString l_targetTypes = DoGetTargetTypes( m_event.GetObjectName() );
					wxString l_value;

					if ( !l_targetTypes.empty() )
					{
						l_value = l_targetTypes.front();
					}

					wxPGChoices l_choices{ l_targetTypes };
					m_targetType->SetChoices( l_choices );
					m_targetType->SetValue( l_value );
					m_event.SetTarget( l_value );
					DoUpdateTarget();
				}
			}

			void PonctualEventProperties::DoUpdateTarget()
			{
				if ( m_target )
				{
					auto l_array = DoGetTargetsList();
					wxString l_value;

					if ( !l_array.empty() )
					{
						l_value = l_array.front();
					}

					wxPGChoices l_choices{ l_array };
					m_target->SetChoices( l_choices );
					m_target->SetValue( l_value );
					m_event.SetTarget( l_value );
				}
			}

			wxArrayString PonctualEventProperties::DoGetTargetsList()
			{
				wxString const l_targetType = m_event.GetTargetType();
				wxArrayString l_targets;

				if ( l_targetType == wxT( "function" ) )
				{
					l_targets.push_back( m_event.GetTarget() );
					// TODO Get functions list
				}
				else
				{
					if ( l_targetType == wxT( "scene_node" ) )
					{
						wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetNodesNames( l_targets );
					}
					else if ( l_targetType == wxT( "overlay" ) )
					{
						auto l_it = OverlayManager::getSingletonPtr()->getOverlayIterator();

						while ( l_it.hasMoreElements() )
						{
							auto l_name = make_wxString( l_it.getNext()->getName() );

							if ( l_name != wxT( "Core/DebugOverlay" ) && l_name != wxT( "Core/LensFlare" ) && l_name != wxT( "Core/LoadOverlay" ) )
							{
								l_targets.Add( l_name );
							}
						}
					}
					else if ( l_targetType == wxT( "overlayElement" ) )
					{
						for ( auto && l_overlay : ScriptEngine::GetContext()->gui->GetAllOverlays() )
						{
							wxString l_name = make_wxString( l_overlay.first );

							if ( l_name != wxT( "Core/DebugOverlay" ) + make_wxString( ScriptEngine::GetContext()->instanceNum )
								 && l_name != wxT( "Core/LensFlare" ) + make_wxString( ScriptEngine::GetContext()->instanceNum )
								 && l_name != wxT( "Core/LoadOverlay" ) + make_wxString( ScriptEngine::GetContext()->instanceNum ) )
							{
								for ( auto && l_element : l_overlay.second->GetElementMap() )
								{
									l_name = make_wxString( l_element.first );
									l_targets.Add( l_name + make_wxString( ScriptEngine::GetContext()->instanceNum.c_str() ) );
								}
							}
						}
					}
					else if ( l_targetType == wxT( "sequence" ) )
					{
						wxGetApp().GetProjectFrame()->GetProject()->GetMainScene()->GetSequencesNames( l_targets );
					}
				}

				return l_targets;
			}
		}
	}
}
