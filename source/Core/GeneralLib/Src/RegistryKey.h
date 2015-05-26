#ifndef ___REGISTRY_KEY_H___
#define ___REGISTRY_KEY_H___

#include "Config.h"
#if GENLIB_WINDOWS
#	include "RegistryManager.h"
#	include "Macros.h"

namespace General
{
	namespace Computer
	{
		template <typename T>
		class RegistryKeyBaseImpl
		{
		};

		template <typename T>
		class d_dll RegistryKeyBase
		{
		private:
			RegistryKeyBase();
			RegistryKeyBase( const RegistryKeyBase & );
		};

		typedef RegistryKeyBase <std::string> RegistryKey;
		typedef RegistryKeyBase <std::wstring> RegistryWKey;

		template <>
		class d_dll RegistryKeyBase <std::string>
		{
			friend class RegistryKeyBaseImpl <std::string>;

		private:
			typedef std::string _string_t;

		private:
			RegistryKeyBaseImpl <_string_t> * m_impl;
			bool l_exists;

		public:
			RegistryKeyBase( RegistryFolder p_folder, const _string_t & p_keyName );
			~RegistryKeyBase();

		public:
			bool Open();
			void Close();
			bool IsValid();
			bool Create();
			bool Delete();
			bool SetStringValue( const _string_t & p_name, const _string_t & p_value );
			bool SetStringExpandValue( const _string_t & p_name, const _string_t & p_value );
			const _string_t & GetName()const;
			_string_t GetStringValue( const _string_t & p_valueName );
		};


		template <>
		class d_dll RegistryKeyBase <std::wstring>
		{
			friend class RegistryKeyBaseImpl <std::wstring>;

		private:
			typedef std::wstring _string_t;

		private:
			RegistryKeyBaseImpl <_string_t> * m_impl;
			bool l_exists;

		public:
			RegistryKeyBase( RegistryFolder p_folder, const _string_t & p_keyName );
			~RegistryKeyBase();

		public:
			bool Open();
			void Close();
			bool IsValid();
			bool Create();
			bool Delete();
			bool SetStringValue( const _string_t & p_name, const _string_t & p_value );
			bool SetStringExpandValue( const _string_t & p_name, const _string_t & p_value );
			const _string_t & GetName()const;
			_string_t GetStringValue( const _string_t & p_valueName );
		};
	}
}

#	endif //  GENLIB_WINDOWS
#endif
