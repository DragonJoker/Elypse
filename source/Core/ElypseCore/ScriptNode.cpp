/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptNode.h"
#include "ScriptCompiler.h"
#include "VariableType.h"
#include "Structure.h"

#include "ElypseLogs.h"

uint32_t ScriptNode::sm_numNodes = 0;

ScriptNode::ScriptNode( ConfigFile * p_file, uint32_t p_lineNum )
	: m_value( nullptr )
	, m_function( nullptr )
	, m_file( p_file )
	, m_userFunction( nullptr )
	, m_numUses( 0 )
	, m_type( nullptr )
	, m_ref( false )
	, m_createdAtLine( p_lineNum )
{
	sm_numNodes ++;
//	std::cout << "Create Node ( " << sm_numNodes <<  " ) -> " << this << std::endl;
}

ScriptNode::~ScriptNode()
{
//	std::cout << "Delete Node ( " << sm_numNodes <<  " ) -> " << this << std::endl;
	sm_numNodes --;
}

void ScriptNode::_reinit()
{
	if ( m_value != nullptr )
	{
		delete m_value;
	}

	m_function = nullptr;
	m_file = nullptr;
	m_userFunction = nullptr;
	m_numUses = 0;
	m_type = nullptr;
	m_ref = false;
	m_createdAtLine = 0;
}

void ScriptNode::_delete()
{
	if ( m_value != nullptr && ! m_ref )
	{
		delete m_value;
	}

	m_value = nullptr;
	m_childs.clear();
}

namespace Elypse
{
	namespace Script
	{
		template<> NodeValue<NodeValueBaseArray>::~NodeValue()
		{
			vector::deleteAll( m_value );
		}
		template<> NodeValue<NodeValueBaseIMap>::~NodeValue()
		{
			map::deleteAll( m_value );
		}
		template<> NodeValue<NodeValueBaseMap>::~NodeValue()
		{
			map::deleteAll( m_value );
		}
		template<> NodeValue<NodeValueBaseRMap>::~NodeValue()
		{
			map::deleteAll( m_value );
		}

		template<> void NodeValue<NodeValueBaseIMap>::CopyDeepValue( NodeValueBase * p_origin )
		{
			map::deleteAll( m_value );
			NodeValue<NodeValueBaseIMap> * l_value = static_cast <NodeValue <NodeValueBaseIMap> *>( p_origin );
			const NodeValueBaseIMap::const_iterator & iend = l_value->m_value.end();
			NodeValueBaseIMap::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i )
			{
				m_value.insert( std::make_pair( i->first, i->second->clone() ) );
			}
		}

		template<> void NodeValue<NodeValueBaseRMap>::CopyDeepValue( NodeValueBase * p_origin )
		{
			map::deleteAll( m_value );
			NodeValue<NodeValueBaseRMap> * l_value = static_cast <NodeValue <NodeValueBaseRMap> *>( p_origin );
			const NodeValueBaseRMap::const_iterator & iend = l_value->m_value.end();
			NodeValueBaseRMap::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i )
			{
				m_value.insert( std::make_pair( i->first, i->second->clone() ) );
			}
		}

		template<> void NodeValue<NodeValueBaseMap>::CopyDeepValue( NodeValueBase * p_origin )
		{
			map::deleteAll( m_value );
			NodeValue<NodeValueBaseMap> * l_value = static_cast <NodeValue <NodeValueBaseMap> *>( p_origin );
			const NodeValueBaseMap::const_iterator & iend = l_value->m_value.end();
			NodeValueBaseMap::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i )
			{
				m_value.insert( std::make_pair( i->first, i->second->clone() ) );
			}
		}

		template<> void NodeValue<NodeValueBasePair>::CopyDeepValue( NodeValueBase * p_origin )
		{
			NodeValue<NodeValueBasePair> * l_value = static_cast<NodeValue<NodeValueBasePair>*>( p_origin );

			if ( m_value.first != nullptr )
			{
				m_value.first->CopyDeepValue( l_value->m_value.first );
			}
			else
			{
				m_value.first = l_value->m_value.first->clone();
			}

			if ( m_value.second != nullptr )
			{
				m_value.second->CopyDeepValue( l_value->m_value.second );
			}
			else
			{
				m_value.second = l_value->m_value.second->clone();
			}
		}

		template<> void NodeValue<NodeValueBaseArray>::CopyDeepValue( NodeValueBase * p_origin )
		{
			vector::deleteAll( m_value );
			NodeValue<NodeValueBaseArray> * l_value = static_cast<NodeValue<NodeValueBaseArray>*>( p_origin );

			for ( auto & l_val : l_value->m_value )
			{
				m_value.push_back( l_val->clone() );
			}
		}

		template<> void NodeValue<StructInstance *>::CopyDeepValue( NodeValueBase * p_origin )
		{
			NodeValue<StructInstance *> * l_value = static_cast<NodeValue<StructInstance *>*>( p_origin );
			m_value->Copy( l_value->m_value );
		}

		template<> NodeValueBase  * NodeValue<StructInstance *>::clone()
		{
			NodeValue <StructInstance *> * l_temp = new NodeValue <StructInstance *> ();
			l_temp->m_value = new StructInstance( m_value->m_definition );
			l_temp->CopyDeepValue( this );
			return l_temp;
		}
	}
}
/*
namespace Elypse
{ namespace Script
{
	template<>
	class NodeValue<NodeValueBaseArray> : public NodeValueBase
	{
		typedef NodeValueBaseArray value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue()
		{
			General::Utils::vector::deleteAll( m_value);
		}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			General::Utils::vector::deleteAll( m_value);

			NodeValue<value_type> * l_value = static_cast<NodeValue<value_type>*>( p_origin);
			_times( l_value->m_value.size())
			{
				m_value.push_back( l_value->m_value[i]->clone());
			}
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};

	template<>
	class NodeValue<NodeValueBaseIMap> : public NodeValueBase
	{
		typedef NodeValueBaseIMap value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue()
		{
			General::Utils::map::deleteAll( m_value);
		}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			General::Utils::map::deleteAll( m_value);

			NodeValue<value_type> * l_value = static_cast <NodeValue <value_type> *>( p_origin);
			const value_type::const_iterator & iend = l_value->m_value.end();
			value_type::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i)
			{
				m_value.insert( std::make_pair( i->first, i->second->clone()));
			}
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};

	template<>
	class NodeValue<NodeValueBaseMap> : public NodeValueBase
	{
		typedef NodeValueBaseMap value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue()
		{
			General::Utils::map::deleteAll( m_value);
		}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			General::Utils::map::deleteAll( m_value);

			NodeValue<value_type> * l_value = static_cast <NodeValue <value_type> *>( p_origin);
			const value_type::const_iterator & iend = l_value->m_value.end();
			value_type::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i)
			{
				m_value.insert( std::make_pair( i->first, i->second->clone()));
			}
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};

	template<>
	class NodeValue<NodeValueBaseRMap> : public NodeValueBase
	{
		typedef NodeValueBaseRMap value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue()
		{
			General::Utils::map::deleteAll( m_value);
		}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			General::Utils::map::deleteAll( m_value);

			NodeValue<value_type> * l_value = static_cast <NodeValue <value_type> *>( p_origin);
			const value_type::const_iterator & iend = l_value->m_value.end();
			value_type::const_iterator i = l_value->m_value.begin();

			for ( ; i != iend; ++ i)
			{
				m_value.insert( std::make_pair( i->first, i->second->clone()));
			}
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};

	template<>
	class NodeValue<NodeValueBasePair> : public NodeValueBase
	{
		typedef NodeValueBasePair value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue() {}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			NodeValue<value_type> * l_value = static_cast<NodeValue<value_type>*>( p_origin);

			if (m_value.first != nullptr)
			{
				m_value.first->CopyDeepValue( l_value->m_value.first);
			}
			else
			{
				m_value.first = l_value->m_value.first->clone();
			}
			if (m_value.second != nullptr)
			{
				m_value.second->CopyDeepValue( l_value->m_value.second);
			}
			else
			{
				m_value.second = l_value->m_value.second->clone();
			}
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};

	template<>
	class NodeValue<StructInstance *> : public NodeValueBase
	{
		typedef StructInstance * value_type;
	public:
		value_type m_value;

	public:
		NodeValue()
			: NodeValueBase( static_cast<VariableBaseType>( NodeTypeBinding<value_type>::NType))
		{}

		virtual ~NodeValue() {}

		virtual NodeValueBase * clone()
		{
			NodeValue <value_type> * l_temp = new NodeValue <value_type> ();
			l_temp->m_value = new StructInstance( m_value->m_definition);
			l_temp->CopyDeepValue( this);
			return l_temp;
		}

		virtual void CopyDeepValue( NodeValueBase * p_origin)
		{
			NodeValue<value_type> * l_value = static_cast<NodeValue<value_type>*>( p_origin);
			m_value->Copy( l_value->m_value);
		}

		virtual void CopyValueFrom( NodeValueBase * p_origin)
		{
			if (p_origin != nullptr)
			{
				m_value = static_cast<NodeValue<value_type> *>( p_origin)->m_value;
			}
		}
	};
}
}
*/

void ScriptNode::Use()
{
	ScriptNodeArray l_callStack;
	_use( l_callStack );
}

void ScriptNode::Delete()
{
	ScriptNodeArray l_callStack;
	_deleteDetails( l_callStack );
}

bool ScriptNode::IsBaseType( VariableType * p_type )const
{
	genlib_assert( m_type != nullptr );
	genlib_assert( p_type != nullptr );
	return m_type->GetBase() == p_type->GetBase();
}

void ScriptNode::_deleteDetails( ScriptNodeArray & p_callStack )
{
	if ( General::Utils::vector::has( p_callStack, this ) )
	{
		return;
	}

	if ( ! m_childs.empty() )
	{
		p_callStack.push_back( this );
		General::Utils::vector::cycle( m_childs, & ScriptNode::_deleteDetails, p_callStack );
		p_callStack.pop_back();
	}

	-- m_numUses;

	if ( m_numUses == 0 )
	{
		ScriptCompiler::ReleaseScriptNode( this );
//		delete this;
	}
}

void ScriptNode::_use( ScriptNodeArray & p_callStack )
{
	if ( General::Utils::vector::has( p_callStack, this ) )
	{
		return;
	}

	if ( m_userFunction != nullptr && ! m_userFunction->m_initialised )
	{
		p_callStack.push_back( this );
		m_userFunction->DeferUse( p_callStack );
		p_callStack.pop_back();
	}
	else if ( ! m_childs.empty() )
	{
		p_callStack.push_back( this );
		General::Utils::vector::cycle( m_childs, & ScriptNode::_use, p_callStack );
		p_callStack.pop_back();
	}

	++ m_numUses;
}

void ScriptNode::CopyNode( ScriptNode * p_node )
{
	m_childs = p_node->m_childs;
	m_createdAtLine = p_node->m_createdAtLine;
	m_file = p_node->m_file;
	m_function = p_node->m_function;
	m_value = p_node->m_value;
	p_node->m_value = nullptr;
	p_node->m_childs.clear();

	if ( m_userFunction != nullptr && m_userFunction->m_node == this )
	{
		for ( size_t i = 0 ; i < m_userFunction->m_deferredUses.size() ; i ++ )
		{
			General::Utils::vector::cycle( m_childs, & ScriptNode::_use, m_userFunction->m_deferredUses[i] );
		}

		m_userFunction->m_initialised = true;
	}
}

void ScriptNode::ExecChild( size_t p_index )
{
	if ( m_childs.size() <= p_index )
	{
		return;
	}

	ScriptNode * l_node = m_childs[p_index];

	if ( l_node->HasFunction() )
	{
		l_node->Execute();
	}
}

VariableBaseType ScriptNode::GetBaseType()const
{
	return m_type->GetBase();
	/*
	if (m_value == nullptr)
	{
		return EMVT_NULL;
	}
	return m_value->m_type;
	*/
}

NodeValueBase * ScriptNode::CreateNodeValue( VariableType * p_type )
{
	switch ( p_type->GetBase() )
	{
	case EMVT_CODE:
		return nullptr;

	case EMVT_NULL:
		return nullptr;

	case EMVT_NULLVALUE:
		return nullptr;

	case EMVT_INT:
		return _createValueBase <int>	( 0 );

	case EMVT_CHAR:
		return _createValueBase <char>	( 0 );

	case EMVT_BOOL:
		return _createValueBase <bool>	( false );

	case EMVT_REAL:
		return _createValueBase <Real>	( 0.0 );

	case EMVT_STRING:
		return _createValueBase <String>	();

	case EMVT_CAMTEX:
		return _createValueBase <CamTex *>	( nullptr );

	case EMVT_TIMER:
		return _createValueBase <ScriptTimer *>	( nullptr );

	case EMVT_PHY_OBJ:
		return _createValueBase <PhysicsObject *>	( nullptr );

	case EMVT_ANIMATION:
		return _createValueBase <EMAnimation *>	( nullptr );

	case EMVT_ANI_OBJ:
		return _createValueBase <AnimatedObject *>	( nullptr );

	case EMVT_ANI_GRP:
		return _createValueBase <AnimatedObjectGroup *>	( nullptr );

	case EMVT_OVERLAY:
		return _createValueBase <EMOverlay *>	( nullptr );

	case EMVT_PHY_MAT:
		return _createValueBase <PhysicsMaterial *>	( nullptr );

	case EMVT_NODE:
		return _createValueBase <SceneNode *>	( nullptr );

	case EMVT_CAMERA:
		return _createValueBase <Camera *>	( nullptr );

	case EMVT_LIGHT:
		return _createValueBase <Light *>	( nullptr );

	case EMVT_ENTITY:
		return _createValueBase <Entity *>	( nullptr );

	case EMVT_OBJ_MAT:
		return _createValueBase <Material *>	( nullptr );

	case EMVT_QUATERNION:
		return _createValueBase <Quaternion>	( Quaternion::IDENTITY );

	case EMVT_COLOUR:
		return _createValueBase <ColourValue>	( ColourValue::Black );

	case EMVT_VECTOR3:
		return _createValueBase <Vector3>	( Vector3::ZERO );

	case EMVT_ARRAY:
		return _createValueBase <NodeValueBaseArray>	();

	case EMVT_STRINGMAP:
		return _createValueBase <NodeValueBaseMap>	();

	case EMVT_INTMAP:
		return _createValueBase <NodeValueBaseIMap>	();

	case EMVT_REALMAP:
		return _createValueBase <NodeValueBaseRMap>	();

	case EMVT_PAIR:
		return _createValueBase <NodeValueBasePair>	( NodeValueBasePair( nullptr, nullptr ) );

	case EMVT_STRUCT:
		return _createValueBase <StructInstance *>	( new StructInstance( p_type ) );

	case EMVT_SMITER:
		return _createValueBase <NodeValueBaseMap::iterator>	();

	case EMVT_RMITER:
		return _createValueBase <NodeValueBaseRMap::iterator>	();

	case EMVT_IMITER:
		return _createValueBase <NodeValueBaseIMap::iterator>	();
	}

	return nullptr;
}

void ScriptNode::SetType( VariableType * p_type )
{
	if ( m_value == nullptr )
	{
		m_type = p_type;
		m_value = CreateNodeValue( p_type );
	}
}

void ScriptNode::CopyValue( ScriptNode * p_originalNode )
{
	if ( m_value != nullptr && p_originalNode != nullptr && p_originalNode->m_value != nullptr )
	{
		m_value->CopyDeepValue( p_originalNode->m_value );
	}
}

void ScriptNode::CopyValue_Deep( ScriptNode * p_originalNode )
{
	if ( m_value != nullptr && p_originalNode != nullptr && p_originalNode->m_value != nullptr )
	{
		m_value->CopyDeepValue( p_originalNode->m_value );
	}
}

void ScriptNode::CopyValue_Ref( NodeValueBase * p_value )
{
	if ( m_value != nullptr && ! m_ref )
	{
		delete m_value;
	}

	m_value = p_value;
	m_ref = true;
}
