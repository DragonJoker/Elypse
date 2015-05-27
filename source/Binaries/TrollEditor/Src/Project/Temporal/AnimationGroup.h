
#ifndef ___TrollAnimationGroup___
#define ___TrollAnimationGroup___

#include "Module_Temporal.h"
#include "TemporalObject.h"

namespace Troll
{
	namespace Temporal
	{
		class TrollAnimationGroup : public TemporalObject
		{
		private:
			TrollAnimatedObjectStrMap m_objects;
			StrStrMap m_chainedAnimations;

		public:
			TrollAnimationGroup( const wxString & p_name, const wxString & p_fileName );
			~TrollAnimationGroup();

			TrollAnimatedObject * CreateAnimatedObject( TrollObject * p_object, const wxString & p_fileName );

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void AddChainedAnimations( const wxString & p_anim1, const wxString & p_anim2 );

			void Write( wxTextOutputStream * p_stream );
		};
	}
}

#endif

