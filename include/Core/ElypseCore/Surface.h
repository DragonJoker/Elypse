/*
See LICENSE file in root folder
*/
#ifndef ___SURFACE_H___
#define ___SURFACE_H___

#include "Module_Simulation.h"


namespace Elypse
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
			Surface( String const & p_name, SceneManager * p_manager );
			~Surface();

		public:
			void CreateFromHeightMap( Real * p_heightMap, uint32_t p_width, uint32_t p_height, String const & p_materialName );
			void CreateFromSpaceMap( Vector3 * p_spaceMap,
									 uint32_t p_width,
									 uint32_t p_height,
									 String const & p_materialName,
									 bool p_seamNorth = false,
									 bool p_seamEast = false,
									 bool p_seamSouth = false,
									 bool p_seamWest = false );
			void Clear();

		public:
			inline String const & GetName() const
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
