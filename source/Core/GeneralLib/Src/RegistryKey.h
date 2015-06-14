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
			RegistryKeyBase( const RegistryKeyBase & );
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

		private:
			std::unique_ptr< RegistryKeyBaseImpl< _string_t > > m_impl;
			bool m_exists;
		};

		template<>
		class d_dll RegistryKeyBase< std::wstring >
		{
			friend class RegistryKeyBaseImpl< std::wstring >;

		private:
			typedef std::wstring _string_t;

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

		private:
			std::unique_ptr< RegistryKeyBaseImpl< _string_t > > m_impl;
			bool m_exists;
		};
	}
}

#	endif //  GENLIB_WINDOWS
#endif
