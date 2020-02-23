/*
See LICENSE file in root folder
*/
#ifndef ___EVENT_TARGET_H___
#define ___EVENT_TARGET_H___

#include "Module_Sequences.h"
#include "Module_Gui.h"
#include "Module_Script.h"

namespace Elypse
{
	namespace Sequences
	{
		template< typename T >
		struct EventTargetEnum;

		template<>
		struct EventTargetEnum< Ogre::SceneNode >
		{
			static EventTargetType const value = EventTargetType::SceneNode;
		};

		template<>
		struct EventTargetEnum< Script::ScriptNode >
		{
			static EventTargetType const value = EventTargetType::ScriptNode;
		};

		template<>
		struct EventTargetEnum< Gui::EMOverlayGroup >
		{
			static EventTargetType const value = EventTargetType::OverlayGroup;
		};

		template<>
		struct EventTargetEnum< Gui::EMOverlay >
		{
			static EventTargetType const value = EventTargetType::Overlay;
		};

		template<>
		struct EventTargetEnum< Ogre::OverlayElement >
		{
			static EventTargetType const value = EventTargetType::OverlayElement;
		};

		template<>
		struct EventTargetEnum< Sequence >
		{
			static EventTargetType const value = EventTargetType::Sequence;
		};

		template< EventTargetType Type >
		struct EventTargetTyper;

		template<>
		struct EventTargetTyper< EventTargetType::SceneNode >
		{
			using type = Ogre::SceneNode;
		};

		template<>
		struct EventTargetTyper< EventTargetType::ScriptNode >
		{
			using type = Script::ScriptNode;
		};

		template<>
		struct EventTargetTyper< EventTargetType::OverlayGroup >
		{
			using type = Gui::EMOverlayGroup;
		};

		template<>
		struct EventTargetTyper< EventTargetType::Overlay >
		{
			using type = Gui::EMOverlay;
		};

		template<>
		struct EventTargetTyper< EventTargetType::OverlayElement >
		{
			using type = Ogre::OverlayElement;
		};

		template<>
		struct EventTargetTyper< EventTargetType::Sequence >
		{
			using type = Sequence;
		};

		class EventTargetBase
		{
		public:
			EventTargetBase( EventTargetType p_type )
				: m_type{ p_type }
			{
			}

			~EventTargetBase()
			{
			}

			template< typename T >
			T * GetAs()const
			{
				genlib_assert( m_type == EventTargetEnum< T >::value );
				return reinterpret_cast< T * >( DoGet() );
			}

		private:
			virtual void * DoGet()const = 0;

		protected:
			EventTargetType const m_type;
		};

		template< EventTargetType Type >
		class EventTarget
			: public EventTargetBase
		{
		public:
			using TargetType = typename EventTargetTyper< Type >::type;

		public:
			EventTarget( TargetType * p_target )
				: EventTargetBase{ Type }
				, m_target{ p_target }
			{
			}

			~EventTarget()
			{
			}

		private:
			virtual void * DoGet()const
			{
				return m_target;
			}

		private:
			TargetType * m_target;
		};

		template< typename T >
		std::unique_ptr< EventTarget< EventTargetEnum< T >::value > > make_target( T * p_target )
		{
			return std::make_unique< EventTarget< EventTargetEnum< T >::value > >( p_target );
		}
	}
}

#endif
