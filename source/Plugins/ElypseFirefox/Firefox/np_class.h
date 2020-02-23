#ifndef ___NP_CLASS_H___
#define ___NP_CLASS_H___

#include "NpApi/npapi.h"
#include "NpApi/npruntime.h"

class ElypsePluginInstance;

namespace Elypse
{
	namespace Firefox
	{
		NPObject * EM_NP_Allocate( NPP inst, NPClass * );
		bool EM_NP_Invoke( NPObject * npobj, NPIdentifier p_methodName, const NPVariant * args, uint32_t argCount, NPVariant * result );
		bool EM_NP_InvokeDefault( NPObject * npobj, const NPVariant * args, uint32_t argCount, NPVariant * result );
		bool EM_NP_HasMethod( NPObject * npobj, NPIdentifier p_methodName );

		struct EM_NPObject : public NPObject
		{
			ElypsePluginInstance * m_instance;
		};
	}
}

#endif
