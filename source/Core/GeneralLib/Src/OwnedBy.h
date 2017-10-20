/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_OWNED_BY_H___
#define ___GENERAL_OWNED_BY_H___

namespace General
{
	namespace Theory
	{
		template< typename T >
		class owned_by
		{
		public:
			owned_by( T & p_owner )
				: m_owner{ &p_owner }
			{
			}

			~owned_by()
			{
			}

			inline T * GetOwner()const
			{
				return m_owner;
			}

			inline void ChangeOwner( T & p_owner )
			{
				m_owner = &p_owner;
			}

		protected:
			T * m_owner;
		};
	}
}

#endif
