/*
See LICENSE file in root folder
*/
#ifndef ___REGISTRY_KEY_H___
#define ___REGISTRY_KEY_H___

#include "Config.h"
#if GENLIB_WINDOWS
#	include "RegistryManager.h"
#	include "Macros.h"
#	include <memory>

namespace General
{
	namespace Computer
	{
		template< typename T >
		class RegistryKeyBaseImpl
		{
		};

		template< typename T >
		class d_dll RegistryKeyBase
		{
		private:
			RegistryKeyBase();
			RegistryKeyBase( RegistryKeyBase const & );
		};

		typedef RegistryKeyBase< std::string > RegistryKey;
		typedef RegistryKeyBase< std::wstring > RegistryWKey;

		template<>
		class d_dll RegistryKeyBase< std::string >
		{
			friend class RegistryKeyBaseImpl< std::string >;

		private:
			typedef std::string _string_t;

		public:
			RegistryKeyBase( RegistryFolder p_folder, _string_t const & p_keyName );
			~RegistryKeyBase();

		public:
			bool Open();
			void Close();
			bool IsValid();
			bool Create();
			bool Delete();
			bool SetStringValue( _string_t const & p_name, _string_t const & p_value );
			bool SetStringExpandValue( _string_t const & p_name, _string_t const & p_value );
			_string_t const & GetName()const;
			_string_t GetStringValue( _string_t const & p_valueName );

		private:
			std::shared_ptr< RegistryKeyBaseImpl< _string_t > > m_impl;
			bool m_exists;
		};

		template<>
		class d_dll RegistryKeyBase< std::wstring >
		{
			friend class RegistryKeyBaseImpl< std::wstring >;

		private:
			typedef std::wstring _string_t;

		public:
			RegistryKeyBase( RegistryFolder p_folder, _string_t const & p_keyName );
			~RegistryKeyBase();

		public:
			bool Open();
			void Close();
			bool IsValid();
			bool Create();
			bool Delete();
			bool SetStringValue( _string_t const & p_name, _string_t const & p_value );
			bool SetStringExpandValue( _string_t const & p_name, _string_t const & p_value );
			_string_t const & GetName()const;
			_string_t GetStringValue( _string_t const & p_valueName );

		private:
			std::shared_ptr< RegistryKeyBaseImpl< _string_t > > m_impl;
			bool m_exists;
		};
	}
}

#	endif //  GENLIB_WINDOWS
#endif
