#include "ContinuousEventProperties.h"

#include "GUI/TimeSequence/Module_Time.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "GUI/ProjectFrame.h"

#include <Context.h>
#include <EMGui.h>
#include <EMOverlayGroup.h>
#include <ScriptEngine.h>
#include <SequenceManager.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			ContinuousEventProperties::ContinuousEventProperties( ProjectComponents::Temporal::TrollContinuousEvent & p_event )
				: ObjectProperty{ ePROPERTY_DATA_TYPE_CONTINUOUS_EVENT }
				, m_event{ p_event }
			{
			}

			ContinuousEventProperties::~ContinuousEventProperties()
			{
			}

			void ContinuousEventProperties::DoCreateProperties( wxPGEditor * p_editor )
			{
				wxString const PROPERTY_CATEGORY_CONTINUOUS_EVENT{ _( "Continuous Event: " ) };
				wxString const PROPERTY_CONTINUOUS_EVENT_ACTION{ _( "Action" ) };
				wxString const PROPERTY_CONTINUOUS_EVENT_TARGET_TYPE{ _( "Target Type" ) };

				wxArrayString l_actions{ make_wxArrayString( ScriptEngine::GetContext()->sequenceManager->GetAvailableContinuousEvents() ) };
				wxArrayString l_targetTypes{ DoGetTargetTypes( m_event.GetObjectName() ) };

				DoAddCategory( PROPERTY_CATEGORY_CONTINUOUS_EVENT );
				DoAddProperty( PROPERTY_CONTINUOUS_EVENT_ACTION, l_actions, m_event.GetObjectName(), [this]( wxPGProperty * p_property )
				{
					m_event.SetActionName( p_property->GetValue() );
					DoUpdateTargetType();
				} );
				m_targetType = DoAddProperty( PROPERTY_CONTINUOUS_EVENT_TARGET_TYPE, l_targetTypes, m_event.GetTargetType(), [this]( wxPGProperty * p_property )
				{
					m_event.SetTargetType( p_property->GetValue() );
					DoUpdateTarget();
				} );
				DoCreateTargetProperties();
				DoCreateInterpolationProperties();
				DoCreateKeyFramesProperties();
			}

			wxArrayString ContinuousEventProperties::DoGetTargetTypes( wxString const & p_action )
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

			void ContinuousEventProperties::DoCreateTargetProperties()
			{
				wxString const PROPERTY_CONTINUOUS_EVENT_TARGET{ _( "Target" ) };

				wxString const l_targetType = m_event.GetTargetType();
				wxString const l_action = m_event.GetObjectName();
				wxArrayString l_targets;
				wxString l_target;

				if ( l_targetType == wxT( "function" ) )
				{
					l_targets.push_back( m_event.GetTarget() );
					l_target = m_event.GetTarget();
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

					l_target = m_event.GetTarget();
				}

				DoAddProperty( PROPERTY_CONTINUOUS_EVENT_TARGET, l_targets, l_target, [this]( wxPGProperty * p_property )
				{
					m_event.SetTarget( p_property->GetValue() );
				} );
			}

			void ContinuousEventProperties::DoCreateInterpolationProperties()
			{
				wxString const PROPERTY_CONTINUOUS_EVENT_INTERPOLATION{ _( "Interpolation" ) };

				wxString l_action = m_event.GetObjectName();
				wxArrayString l_interpolations;
				wxString l_interpolation;

				if ( l_action.find( wxT( "Translate" ) ) || l_action.find( wxT( "Scale" ) ) || l_action.find( wxT( "Resize" ) ) )
				{
					l_interpolations.push_back( wxT( "linear" ) );
					l_interpolations.push_back( wxT( "sinus" ) );
					l_interpolations.push_back( wxT( "bezier3" ) );
					l_interpolations.push_back( wxT( "bezier4" ) );
					l_interpolations.push_back( wxT( "bezier5" ) );
					l_interpolation = m_event.GetInterpolation();
				}
				else if ( l_action == wxT( "Overlay_Rotate" ) )
				{
					l_interpolations.push_back( wxT( "linear" ) );
					l_interpolation = wxT( "linear" );
				}
				else if ( l_action.find( wxT( "Rotate" ) ) )
				{
					l_interpolations.push_back( wxT( "linear" ) );
					l_interpolation = wxT( "linear" );
				}

				DoAddProperty( PROPERTY_CONTINUOUS_EVENT_INTERPOLATION, l_interpolations, l_interpolation, [this]( wxPGProperty * p_property )
				{
					m_event.SetInterpolation( p_property->GetValue() );
				} );
			}

			void ContinuousEventProperties::DoCreateKeyFramesProperties()
			{
				wxString const PROPERTY_CONTINUOUS_EVENT_KEYFRAMES{ _( "KeyFrames" ) };
				wxString const PROPERTY_CONTINUOUS_EVENT_KEYFRAMES_NEW{ _( "New..." ) };

				wxArrayString l_list;
				m_event.GetKeyFrames( l_list );
				wxString l_selected = PROPERTY_CONTINUOUS_EVENT_KEYFRAMES_NEW;
				l_list.push_back( l_selected );

				DoAddProperty( PROPERTY_CONTINUOUS_EVENT_KEYFRAMES, l_list, l_selected, [this, PROPERTY_CONTINUOUS_EVENT_KEYFRAMES_NEW]( wxPGProperty * p_property )
				{
					wxString l_value = p_property->GetValue();
					auto l_old = m_viewedKeyFrame;
					DoRemoveKeyFrameProperties();

					if ( l_value == PROPERTY_CONTINUOUS_EVENT_KEYFRAMES_NEW )
					{
						m_viewedKeyFrame = m_event.AddKeyFrame( 0.0, wxEmptyString );
					}
					else
					{
						std::cout << "ContinuousEventProperties::_onSelectKey - " << l_value.c_str() << "\n";
						float l_tmp;

						if ( GetReal( l_value, l_tmp ) )
						{
							m_viewedKeyFrame = m_event.GetKeyFrame( l_tmp );
						}
					}

					if ( m_viewedKeyFrame )
					{
						//m_viewedKeyFrame->BuildPanel( m_keyPanel, m_keyPanel->GetClientSize().x );
					}
				} );
			}

			void ContinuousEventProperties::DoRemoveKeyFrameProperties()
			{
				// TODO
			}

			void ContinuousEventProperties::DoUpdateTargetType()
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
					m_event.SetTargetType( l_value );
					DoUpdateTarget();
				}
			}

			void ContinuousEventProperties::DoUpdateTarget()
			{
				auto l_array = DoGetTargetTypes( m_event.GetTargetType() );

				if ( l_array.empty() )
				{
					m_event.SetTarget( wxEmptyString );
				}
				else
				{
					m_event.SetTarget( l_array.front() );
				}
			}
		}
	}
}
