#ifndef ___NAMED_UNIQUE_H___
#define ___NAMED_UNIQUE_H___

#include <map>
#include <string>

#include "NonCopyable.h"

#include "STLMapMacros.h"
#include "AutoSingleton.h"

namespace General
{
	namespace Theory
	{
		template <typename TObj>
		class NamedUnique : AutoSingleton <NamedUnique <TObj> >
		{
		private:
			std::map <std::string, TObj *> m_objects;

		protected:
			NamedUnique() {}
			~NamedUnique() {}

		private:
			static inline TypeMap & _getTypeMap()
			{
				static TypeMap sm_singletonMap;
				return sm_singletonMap;
			}

		public:
			static TObj * Get( const std::string & p_name )
			{
				return General::Utils::map::insert( _getTypeMap(), p_name );
			}

			static void DestroyUniqueNamed( const std::string & p_name )
			{
				General::Utils::map::deleteValue( _getTypeMap(), p_name );
			}

			static void DestroyAllNamedUnique()
			{
				General::Utils::map::deleteAll( _getTypeMap() );
			}

			static TObj * GetUniqueNamed( const std::string & p_name )
			{
				return General::Utils::map::findOrNull( _getTypeMap(), p_name );
			}
		};
	}
}

#endif
