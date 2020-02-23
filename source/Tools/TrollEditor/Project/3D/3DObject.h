/*
See LICENSE file in root folder
*/
#ifndef ___TE3DOBJECT_H___
#define ___TE3DOBJECT_H___

#include "Module_3D.h"

#include "Project/Object.h"

namespace Troll
{
	namespace ProjectComponents
	{
		namespace Objects3D
		{
			class Troll3DObject
				: public Object
			{
			public:
				Troll3DObject( wxString const & p_name, wxString const & p_fileName, Troll3DObjectType p_type );
				virtual ~Troll3DObject();

				virtual void AttachTo( TrollSceneNode * p_node );
				void Detach();

				inline Troll3DObjectType Get3DObjectType()const
				{
					return m_3DObjectType;
				}
				inline TrollSceneNode * GetParent()const
				{
					return m_parent;
				}
				inline void SetParent( TrollSceneNode * p_parent )
				{
					m_parent = p_parent;
				}

			protected:
				TrollSceneNode * m_parent;
				Troll3DObjectType m_3DObjectType;
			};
		}
	}
}

#endif
