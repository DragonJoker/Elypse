#ifndef ___EVENT_FACTORY_H___
#define ___EVENT_FACTORY_H___

#include "Module_Sequences.h"

namespace EMuse
{
	namespace Sequences
	{
		class BasePonctualEventFactory
		{
		public:
			BasePonctualEventFactory() {}
			virtual ~BasePonctualEventFactory() {}

		public:
			virtual BasePonctualEvent * CreateEvent( const StringArray & p_params ) = 0;
		};

		class BaseContinuousEventFactory
		{
		public:
			BaseContinuousEventFactory() {}
			virtual ~BaseContinuousEventFactory() {}

		public:
			virtual BaseContinuousEvent * CreateEvent() = 0;
		};

#define TEMP_MACRO_DEFINE_PONCTUAL_FACTORY(X)										\
		class X : public BasePonctualEventFactory										\
		{																				\
		public:																			\
			X(){}																		\
			virtual ~X(){}																\
		public:																			\
			virtual BasePonctualEvent * CreateEvent( const StringArray & p_params);		\
		}

#define TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY(X)										\
		class X : public BaseContinuousEventFactory										\
		{																				\
		public:																			\
			X(){}																		\
			virtual ~X(){}																\
		public:																			\
			virtual BaseContinuousEvent * CreateEvent();								\
		}

		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( SceneNode_SetPosition_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( SceneNode_SetScale_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Overlay_Hide_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Overlay_Show_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( ScriptNode_Execution_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Sequence_Starting_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Sequence_Pausing_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Sequence_Unpausing_Factory );
		TEMP_MACRO_DEFINE_PONCTUAL_FACTORY( Sequence_Stopping_Factory );

		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( SceneNode_Translation_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( SceneNode_Rotation_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( SceneNode_Scale_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( Overlay_Translation_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( Overlay_Rotation_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( Overlay_Resizing_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( Overlay_RelativeTranslation_Factory );
		TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY( Overlay_RelativeResizing_Factory );


#undef TEMP_MACRO_DEFINE_PONCTUAL_FACTORY
#undef TEMP_MACRO_DEFINE_CONTINUOUS_FACTORY

	}
}

#endif
