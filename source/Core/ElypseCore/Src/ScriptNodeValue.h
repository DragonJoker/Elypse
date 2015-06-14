/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

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
#ifndef ___SCRIPT_NODE_VALUE_H___
#define ___SCRIPT_NODE_VALUE_H___

#include "Module_Script.h"

#include <OgreVector3.h>
#include <OgreColourValue.h>
#include <OgreQuaternion.h>

#include "Module_Animation.h"
#include "Module_Physics.h"
#include "Module_SpecialEffects.h"
#include "Module_Gui.h"
#include "MapIterator.h"

#include <Macros.h>

namespace Elypse
{
	namespace Script
	{
		template<typename T>
		struct NodeTypeBinding;

		class NodeValueBase
		{
		public:
			VariableBaseType m_type;

		public:
			NodeValueBase( VariableBaseType p_type )
				: m_type( p_type )
			{}
			virtual ~NodeValueBase() {};

		public:
			inline bool IsType( VariableBaseType p_type )const
			{
				return m_type == p_type;
			}

			template <typename T>
			inline bool IsType()const
			{
				return static_cast<VariableBaseType>( NodeTypeBinding<T>::NType ) == m_type;
			}

			virtual void CopyValueFrom( NodeValueBase * p_origin ) = 0;
			virtual void CopyDeepValue( NodeValueBase * p_origin ) = 0;

			virtual NodeValueBase * clone()
			{
				return NULL;
			}
		};

		template<typename T>
		class NodeValue : public NodeValueBase
		{
			typedef T value_type;
		public:
			value_type m_value;

		public:
			NodeValue()
				: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType ) )
			{}

			virtual ~NodeValue() {}

			virtual NodeValueBase * clone()
			{
				NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
				l_temp->CopyDeepValue( this );
				return l_temp;
			}

			virtual void CopyDeepValue( NodeValueBase * p_origin )
			{
				CopyValueFrom( p_origin );
			}

			virtual void CopyValueFrom( NodeValueBase * p_origin )
			{
				if ( p_origin != NULL )
				{
					m_value = static_cast<NodeValue<value_type> *>( p_origin )->m_value;
				}
			}
		};



#define EMUSE_BIND_NODE_VALUE( p_type, p_nodeType)					\
	template<>															\
	struct NodeTypeBinding<p_type>										\
	{																	\
		enum {NType = p_nodeType};										\
	};

		EMUSE_BIND_NODE_VALUE( int, EMVT_INT );
		EMUSE_BIND_NODE_VALUE( Real, EMVT_REAL );

		EMUSE_BIND_NODE_VALUE( bool, EMVT_BOOL );
		EMUSE_BIND_NODE_VALUE( Vector3, EMVT_VECTOR3 );
		EMUSE_BIND_NODE_VALUE( ColourValue, EMVT_COLOUR );
		EMUSE_BIND_NODE_VALUE( Quaternion, EMVT_QUATERNION );

		EMUSE_BIND_NODE_VALUE( char, EMVT_CHAR );
		EMUSE_BIND_NODE_VALUE( String, EMVT_STRING );

		EMUSE_BIND_NODE_VALUE( Entity *, EMVT_ENTITY );
		EMUSE_BIND_NODE_VALUE( Light *, EMVT_LIGHT );
		EMUSE_BIND_NODE_VALUE( Camera *, EMVT_CAMERA );
		EMUSE_BIND_NODE_VALUE( SceneNode *, EMVT_NODE );
		EMUSE_BIND_NODE_VALUE( Material *, EMVT_OBJ_MAT );

		EMUSE_BIND_NODE_VALUE( EMOverlay *, EMVT_OVERLAY );
		EMUSE_BIND_NODE_VALUE( AnimatedObjectGroup *, EMVT_ANI_GRP );
		EMUSE_BIND_NODE_VALUE( PhysicsMaterial *, EMVT_PHY_MAT );
		EMUSE_BIND_NODE_VALUE( AnimatedObject *, EMVT_ANI_OBJ );
		EMUSE_BIND_NODE_VALUE( EMAnimation *, EMVT_ANIMATION );
		EMUSE_BIND_NODE_VALUE( PhysicsObject *, EMVT_PHY_OBJ );
		EMUSE_BIND_NODE_VALUE( ScriptTimer *, EMVT_TIMER );
		EMUSE_BIND_NODE_VALUE( CamTex *, EMVT_CAMTEX );
		EMUSE_BIND_NODE_VALUE( ObjectMaterial *, EMVT_OBJ_MAT );

		EMUSE_BIND_NODE_VALUE( NodeValueBaseArray, EMVT_ARRAY );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseIMap, EMVT_INTMAP );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseMap, EMVT_STRINGMAP );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseRMap, EMVT_REALMAP );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseIMap::iterator, EMVT_IMITER );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseMap::iterator, EMVT_SMITER );
		EMUSE_BIND_NODE_VALUE( NodeValueBaseRMap::iterator, EMVT_RMITER );

		EMUSE_BIND_NODE_VALUE( NodeValueBasePair, EMVT_PAIR );

		EMUSE_BIND_NODE_VALUE( StructInstance *, EMVT_STRUCT );




	}
}

#endif
