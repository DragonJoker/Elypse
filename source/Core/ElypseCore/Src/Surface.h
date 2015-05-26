#ifndef ___SURFACE_H___
#define ___SURFACE_H___

#include "Module_Simulation.h"


namespace EMuse
{
	namespace Simulation
	{
		class Surface : d_noncopyable
		{
		protected:
			ManualObject * m_object;
			String m_name;
			SceneManager * m_manager;

		public:
			Surface();
			Surface( const String & p_name, SceneManager * p_manager );
			~Surface();

		public:
			void CreateFromHeightMap( Real * p_heightMap, unsigned int p_width, unsigned int p_height, const String & p_materialName );
			void CreateFromSpaceMap(	Vector3 * p_spaceMap,
										unsigned int p_width,
										unsigned int p_height,
										const String & p_materialName,
										bool p_seamNorth = false,
										bool p_seamEast = false,
										bool p_seamSouth = false,
										bool p_seamWest = false );
			void Clear();

		public:
			inline const String & GetName() const
			{
				return m_name;
			}
			inline ManualObject * GetObject() const
			{
				return m_object;
			}
		};
	}
}

#endif
