//***********************************************************************************************************
#ifndef _TrollMesh_H_
#define _TrollMesh_H_
//***********************************************************************************************************
#include "Module_3D.h"
//***********************************************************************************************************
namespace Troll
{
	namespace Objects3D
	{
		class TrollMesh
		{
		private:
			wxString m_meshName;
			wxStringUIntMap m_subMaterials;

		public:
			TrollMesh( const wxString & p_meshName );
			~TrollMesh();

			void AddSubMaterial( const wxString & p_name );
			void SetSubMaterial( const wxString & p_name, unsigned int p_index );

			wxString GetMaterial( unsigned int p_index );

			inline const wxString &	GetName()const
			{
				return m_meshName;
			}
			inline unsigned int		GetNumSubMaterials()const
			{
				return m_subMaterials.size();
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************