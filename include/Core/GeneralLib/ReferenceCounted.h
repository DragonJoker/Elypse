/*
See LICENSE file in root folder
*/
#ifndef ___REFERENCE_COUNTED___
#define ___REFERENCE_COUNTED___

namespace General
{
	namespace Theory
	{
		template< typename T >
		class ReferenceCounted
		{
		protected:
			ReferenceCounted()
				: m_count{ 0 }
			{
			}

		public:
			void Use()
			{
				m_count++;
			}
			void Release()
			{
				m_count--;

				if ( m_count == 0 )
				{
					delete static_cast< T * >( this );
				}
			}

		protected:
			uint32_t m_count;
		};

		template< typename T >
		class ReferenceHolder
		{
		public:
			ReferenceHolder( ReferenceCounted< T > * p_ref )
				: m_ref{ p_ref }
			{
				m_ref->Use();
			}

			~ReferenceHolder()
			{
				m_ref->Release();
				m_ref = nullptr;
			}

		protected:
			ReferenceCounted< T > * m_ref;
		};
	}
}

#endif
