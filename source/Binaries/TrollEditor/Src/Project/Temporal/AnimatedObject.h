
#ifndef ___TrollAnimatedObject___
#define ___TrollAnimatedObject___

#include "Module_Temporal.h"
#include "TemporalObject.h"

namespace Troll
{
	namespace Temporal
	{
		class TrollAnimatedObject : public TemporalObject
		{
		private:
			TrollAnimationGroup * m_group;
			TrollObject * m_object;
			TrollAnimationStrMap m_animations;

		public:
			TrollAnimatedObject( TrollObject * p_object, TrollAnimationGroup * p_group, const wxString & p_fileName );
			~TrollAnimatedObject();

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void AddAnimation( TrollAnimation * p_anim );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline TrollObject 	*		GetTrollObject()const
			{
				return m_object;
			}
			inline TrollAnimationGroup *	GetGroup()const
			{
				return m_group;
			}
		};
	}
}

#endif

