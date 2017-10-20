#include "stdafx.h"

#include "np_class.h"

#include <string>
#include <iostream>

#include "Elypse/main.h"


using namespace Elypse::Firefox;

NPClass Private_Class =
{
	NP_CLASS_STRUCT_VERSION, //structVersion
	EM_NP_Allocate, //allocate
	NULL, //deallocate
	NULL, //invalidate
	EM_NP_HasMethod, //hasMethod
	EM_NP_Invoke, //invoke
	EM_NP_InvokeDefault, //invokeDefault
	NULL, //Private_HasProperty, //hasProperty
	NULL, //getProperty
	NULL, //setProperty
	NULL //removeProperty
};

NPObject * Elypse::Firefox::EM_NP_Allocate( NPP inst, NPClass * )
{
	// We initialize the structure in NPP_New() below
	EM_NPObject * l_obj = static_cast<EM_NPObject *>( malloc( sizeof( EM_NPObject ) ) );
	l_obj->m_instance = static_cast<ElypsePluginInstance *>( inst->pdata );
	return ( NPObject * )l_obj;
}


bool Elypse::Firefox::EM_NP_Invoke( NPObject * npobj, NPIdentifier p_methodName,
								   const NPVariant * args, uint32_t argCount,
								   NPVariant * result )
{
	if	( npobj != NULL
			&&	NPN_IdentifierIsString( p_methodName ) )
	{
		char * l_name = NPN_UTF8FromIdentifier( p_methodName );

		if ( strcmp( l_name, "ExecuteScript" ) == 0 )
		{
			if ( argCount != 1 )
			{
				BOOLEAN_TO_NPVARIANT( FALSE, *result );
				return false;
			}
			else
			{
				std::string l_param = NPVARIANT_TO_STRING( args[0] ).UTF8Characters;
				EM_NPObject * l_obj = static_cast<EM_NPObject * >( npobj );

				if ( l_obj != NULL )
				{
					l_obj->m_instance->GetInstance()->GetFL()->AddToScriptQueue( l_param );
				}

//				std::cout << "ExecuteScript -> param : " << l_param << std::endl;

				return true;
			}
		}

		NPN_MemFree( l_name );
	}

	return false;
}

bool Elypse::Firefox::EM_NP_InvokeDefault( NPObject * npobj,
		const NPVariant * args, uint32_t argCount,
		NPVariant * result )
{
	return true;
}

bool Elypse::Firefox::EM_NP_HasMethod( NPObject * npobj, NPIdentifier p_methodName )
{
	if ( NPN_IdentifierIsString( p_methodName ) )
	{
		char * l_name = NPN_UTF8FromIdentifier( p_methodName );
		bool l_res = ( strcmp( l_name, "ExecuteScript" ) == 0 );
		NPN_MemFree( l_name );
		return l_res;
	}

	return false;
}
