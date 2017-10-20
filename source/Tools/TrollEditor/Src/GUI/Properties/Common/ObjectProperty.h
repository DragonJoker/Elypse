/*
This source file is part of Castor3D (http://castor3d.developpez.com/castor3d.htm)

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
#ifndef ___TROLL_GUI_PROPERTIES_OBJECT_PROPERTY_H___
#define ___TROLL_GUI_PROPERTIES_OBJECT_PROPERTY_H___

#include "PointProperties.h"
#include "QuaternionProperties.h"
#include "SizeProperties.h"
#include "VectorProperties.h"

#include <wx/event.h>
#include <wx/propgrid/propgrid.h>

namespace Troll
{
	namespace GUI
	{
		namespace Properties
		{
			/*!
			\author 	Sylvain DOREMUS
			\date 		24/08/2015
			\version	0.8.0
			\~english
			\brief		The supported object types, for display, and properties
			\~french
			\brief		Les type d'objets supportés, dans les propriétés, et à l'affichage
			*/
			enum class DataType
			{
				Project,
				Overlay,
				Camera,
				ContinuousEvent,
				Entity,
				KeyFrame,
				Light,
				PonctualEvent,
				SceneNode,
				Sequence,
				SoundObject,
				Sound,
				VideoObject,
				Video,
			};
			/*!
			\author 	Sylvain DOREMUS
			\date 		24/08/2015
			\version	0.8.0
			\~english
			\brief		Helper class to communicate between Scene objects or Materials lists and PropertiesHolder
			\~french
			\brief		Classe d'aide facilitant la communication entre la liste des objets de scène, ou la liste de matériaux, et PropertiesHolder
			*/
			class ObjectProperty
				: public wxEvtHandler
			{
			public:
				using PropertyChangeAction = std::function< void( wxPGProperty * ) >;
				using ValueUpdateAction = std::function< void( wxPGProperty * ) >;
				/**
				 *\~english
				 *\brief		Constructor
				 *\param[in]	p_engine	The engine, to post events to.
				 *\param[in]	p_editable	Tells if the properties are modifiable
				 *\param[in]	p_type		The object type
				 *\~french
				 *\brief		Constructeur
				 *\param[in]	p_engine	Le moteur, auquel on va poster les évènements.
				 *\param[in]	p_editable	Dit si les propriétés sont modifiables
				 *\param[in]	p_type		Le type d'objet
				 */
				ObjectProperty( DataType p_type );
				/**
				 *\~english
				 *\brief		Destructor
				 *\~french
				 *\brief		Destructeur
				 */
				virtual ~ObjectProperty();
				/**
				 *\~english
				 *\brief		Creates and fills the item properties, in the given wxPropertyGrid
				 *\param[in]	p_editor	The button editor, for properties that need it
				 *\param[in]	p_grid		The target wxPropertyGrid
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'objet, dans la wxPropertyGrid donnée
				 *\param[in]	p_editor	L'éditeur bouton, pour les propriétés en ayant besoin
				 *\param[in]	p_grid		La wxPropertyGrid cible
				 */
				void CreateProperties( wxPGEditor * p_editor, wxPropertyGrid * p_grid );
				/**
				 *\~english
				 *\brief		Call when a property grid property is changed
				 *\param[in]	p_event	The event
				 *\~french
				 *\brief		Appelée lorsqu'une propriété est changée
				 *\param[in]	p_event	L'évènement
				 */
				void OnPropertyChange( wxPropertyGridEvent & p_event );
				/**
				 *\~english
				 *\brief		Call to update properties from values.
				 *\~french
				 *\brief		Appelée pour mettre les propriétés à jour depuis les valeurs.
				 */
				void UpdateProperties();
				/**
				 *\~english
				 *\brief		Retrieves the object type
				 *\return		The value
				 *\~french
				 *\brief		Récupère le type d'objet
				 *\return		La valeur
				 */
				inline DataType GetType()const
				{
					return m_type;
				}
				/**
				 *\~english
				 *\brief		Called when modifications are validated.
				 *\param[in]	p_parent	The window from which the validation is made.
				 *\return		\p true if the validation can be made.
				 *\~french
				 *\brief		Appelée lorsque les modifications sont validées.
				 *\param[in]	p_parent	La fenêtre à partir de laquelle la validation est effectuée.
				 *\return		\p true si la validation peut être effectuée.
				 */
				virtual bool OnValidate( wxWindow * p_parent )
				{
					return true;
				}
				/**
				 *\~english
				 *\brief		Called when modifications are cancelled.
				 *\param[in]	p_parent	The window from which the cancellation is made.
				 *\~french
				 *\brief		Appelée lorsque les modifications sont annulées.
				 *\param[in]	p_parent	La fenêtre à partir de laquelle l'annulation est effectuée.
				 */
				virtual void OnCancel( wxWindow * p_parent )
				{
				}

			protected:
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 */
				void DoAddCategory( wxString const & p_name );
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				template< typename T >
				inline wxPGProperty * DoAddProperty( wxString const & p_name, T const & p_value, PropertyChangeAction p_onPropertyChange, ValueUpdateAction p_onValueChange = ValueUpdateAction{} )
				{
					auto l_property = CreateProperty( p_name, p_value );

					if ( p_onPropertyChange )
					{
						m_actions.insert( { l_property->GetName(), p_onPropertyChange } );
					}

					if ( p_onPropertyChange )
					{
						m_updates.insert( { l_property, p_onValueChange } );
					}

					return m_grid->Append( l_property );
				}
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				inline wxPGProperty * DoAddProperty( wxString const & p_name, wxPGChoices const & p_choices, wxString const & p_value, PropertyChangeAction p_onPropertyChange, ValueUpdateAction p_onValueChange = ValueUpdateAction{} )
				{
					auto l_property = CreateProperty( p_name, p_choices, p_value );

					if ( p_onPropertyChange )
					{
						m_actions.insert( { l_property->GetName(), p_onPropertyChange } );
					}

					if ( p_onPropertyChange )
					{
						m_updates.insert( { l_property, p_onValueChange } );
					}

					return m_grid->Append( l_property );
				}
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				template< Vector2Names NamesT >
				inline wxPGProperty * DoAddProperty( wxString const & p_name, Vector2 const & p_value, PropertyChangeAction p_onPropertyChange, ValueUpdateAction p_onValueChange = ValueUpdateAction{} )
				{
					auto l_property = CreateProperty< NamesT >( p_name, p_value );

					if ( p_onPropertyChange )
					{
						m_actions.insert( { l_property->GetName(), p_onPropertyChange } );
					}

					if ( p_onPropertyChange )
					{
						m_updates.insert( { l_property, p_onValueChange } );
					}

					return m_grid->Append( l_property );
				}
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				template< Vector3Names NamesT >
				inline wxPGProperty * DoAddProperty( wxString const & p_name, Vector3 const & p_value, PropertyChangeAction p_onPropertyChange, ValueUpdateAction p_onValueChange = ValueUpdateAction{} )
				{
					auto l_property = CreateProperty< NamesT >( p_name, p_value );

					if ( p_onPropertyChange )
					{
						m_actions.insert( { l_property->GetName(), p_onPropertyChange } );
					}

					if ( p_onPropertyChange )
					{
						m_updates.insert( { l_property, p_onValueChange } );
					}

					return m_grid->Append( l_property );
				}
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				template< Vector4Names NamesT >
				inline wxPGProperty * DoAddProperty( wxString const & p_name, Vector4 const & p_value, PropertyChangeAction p_onPropertyChange, ValueUpdateAction p_onValueChange = ValueUpdateAction{} )
				{
					auto l_property = CreateProperty< NamesT >( p_name, p_value );

					if ( p_onPropertyChange )
					{
						m_actions.insert( { l_property->GetName(), p_onPropertyChange } );
					}

					if ( p_onPropertyChange )
					{
						m_updates.insert( { l_property, p_onValueChange } );
					}

					return m_grid->Append( l_property );
				}
				/**
				 *\~english
				 *\brief		Adds a property in the given wxPropertyGrid.
				 *\param[in]	p_property			The property to add.
				 *\param[in]	p_onPropertyChange	Action applied if the property is changed.
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée.
				 *\param[in]	p_property			La propriété à ajouter.
				 *\param[in]	p_onPropertyChange	Action à appliquer lors que la propriété est modifiée.
				 */
				inline wxPGProperty * DoAddProperty( wxString const & p_name, ButtonEventMethod const & p_buttonEvent, wxEvtHandler * p_handler, wxPGEditor * p_editor )
				{
					return m_grid->Append( CreateProperty( p_name, p_name, p_buttonEvent, p_handler, p_editor ) );
				}

			private:
				/**
				 *\~english
				 *\brief		Creates and fills the properties, in the given wxPropertyGrid
				 *\param[in]	p_editor	The button editor, for properties that need it
				 *\~french
				 *\brief		Construit et remplit les propriétés, dans la wxPropertyGrid donnée
				 *\param[in]	p_editor	Le bouton d'édition, pour les propriétés en ayant besoin
				 */
				virtual void DoCreateProperties( wxPGEditor * p_editor ) = 0;
				/**
				 *\~english
				 *\brief		Creates and fills the overlay properties, in the given wxPropertyGrid
				 *\param[in]	p_editor	The button editor, for properties that need it
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée
				 *\param[in]	p_editor	Le bouton d'édition, pour les propriétés en ayant besoin
				 */
				virtual void DoPreCreateProperties( wxPGEditor * p_editor )
				{
				}
				/**
				 *\~english
				 *\brief		Creates and fills the overlay properties, in the given wxPropertyGrid
				 *\param[in]	p_editor	The button editor, for properties that need it
				 *\~french
				 *\brief		Construit et remplit les propriétés de l'incrustation, dans la wxPropertyGrid donnée
				 *\param[in]	p_editor	Le bouton d'édition, pour les propriétés en ayant besoin
				 */
				virtual void DoPostCreateProperties( wxPGEditor * p_editor )
				{
				}

			private:
				DataType const m_type;
				std::map< wxString, PropertyChangeAction > m_actions;
				std::map< wxPGProperty *, ValueUpdateAction > m_updates;
				wxPropertyGrid * m_grid{ nullptr };
			};
		}
	}
}

#endif
