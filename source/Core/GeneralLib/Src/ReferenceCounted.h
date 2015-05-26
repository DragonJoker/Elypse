#ifndef ___REFERENCE_COUNTED___
#define ___REFERENCE_COUNTED___

namespace General
{
	namespace Theory
	{
		template <typename T>
		class ReferenceCounted
		{
		protected:
			unsigned int m_count;

		protected:
			ReferenceCounted()
				:	m_count( 0 )
			{}

		public:
			void Use()
			{
				m_count ++;
			}
			void Release()
			{
				m_count --;

				if ( m_count == 0 )
				{
					delete static_cast <T *>( this );
				}
			}
		};

		template <typename T>
		class ReferenceHolder
		{
		protected:
			ReferenceCounted <T> * m_ref;

		public:
			ReferenceHolder( ReferenceCounted <T> * p_ref )
				:	m_ref( p_ref )
			{
				m_ref->Use();
			}

			~ReferenceHolder()
			{
				m_ref->Release();
				m_ref = NULL;
			}
		};
	}
}

#endif
