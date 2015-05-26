#ifndef ___GENERAL_OWNED_BY_H___
#define ___GENERAL_OWNED_BY_H___

namespace General
{
	namespace Theory
	{
		template <typename T>
		class owned_by
		{
		protected:
			T * m_owner;

		public:
			owned_by( T * p_owner = NULL )
				:	m_owner( p_owner )
			{
			}
			~owned_by() {}

		public:
			inline T * GetOwner()const
			{
				return m_owner;
			}
			inline void SetOwner( T * p_owner )
			{
				m_owner = p_owner;
			}
		};
	}
}

#endif
