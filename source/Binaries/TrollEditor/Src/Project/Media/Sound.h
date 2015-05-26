//***********************************************************************************************************
#ifndef _TrollSound_H_
#define _TrollSound_H_
//***********************************************************************************************************
#include "Module_Media.h"
#include "MediaObject.h"
#include "../3D/Module_3D.h"
#include <Module_Media.h>
//***********************************************************************************************************
namespace Troll
{
	namespace Media
	{
		class TrollSound : public MediaObject
		{
		private:
			wxString m_filePath;
			TrollSoundType m_soundType;
			bool m_isGlobal;
			TrollSoundObjectMap m_objects;
			EMuse::Media::SoundObject * m_museSound;

			//if global
			Troll::Media::SoundInstance * m_globalInstance;

		public:
			TrollSound( const wxString & p_name, const wxString & p_fileName );
			~TrollSound();

			TrollSoundObject * AddObject( Objects3D::TrollSceneNode * p_node );

			virtual int BuildPanel( wxWindow * p_parent, int p_width );

			void SetMuseSound( EMuse::Media::SoundObject * p_sound );
			void SetUrl( const wxString & p_filePath );
			void SetSoundType( TrollSoundType p_type );
			void SetLocal();
			void SetGlobalAttenuation( float p_att );
			void SetGlobalLooped( bool p_looped );
			void SetGlobalTimer( float p_time );
			void SetGlobalFadeIn( float p_time );
			void SetGlobalFadeOut( float p_time );

			void Write( wxTextOutputStream * p_stream );

		public:
			inline wxString							GetUrl()const
			{
				return m_filePath;
			}
			inline TrollSoundType						GetSoundType()const
			{
				return m_soundType;
			}
			inline bool								IsGlobal()const
			{
				return m_isGlobal;
			}
			inline Troll::Media::SoundInstance *	GetGlobal()const
			{
				return m_globalInstance;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
