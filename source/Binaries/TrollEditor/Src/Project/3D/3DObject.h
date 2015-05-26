//***********************************************************************************************************
#ifndef ___TE3DOBJECT_H___
#define ___TE3DOBJECT_H___
//***********************************************************************************************************
#include "Module_3D.h"
#include "../Object.h"
//***********************************************************************************************************
namespace Troll
{
	namespace Objects3D
	{
		class Troll3DObject : public Troll::TrollObject
		{
		protected:
			TrollSceneNode * m_parent;
			Troll3DObjectType m_3DObjectType;

		public:
			Troll3DObject( const wxString & p_name, const wxString & p_fileName, Troll3DObjectType p_type );
			virtual ~Troll3DObject();

			virtual void AttachTo( TrollSceneNode * p_node );
			void Detach();

		public:
			inline Troll3DObjectType	Get3DObjectType()const
			{
				return m_3DObjectType;
			}
			inline TrollSceneNode *	GetParent()const
			{
				return m_parent;
			}

			inline void	SetParent( TrollSceneNode * p_parent )
			{
				m_parent = p_parent;
			}
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
