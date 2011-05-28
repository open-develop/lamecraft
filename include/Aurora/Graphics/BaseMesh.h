#ifndef AURORA_BASEMESH_H
#define AURORA_BASEMESH_H

#include <Aurora/Math/BoundingBox.h>

namespace Aurora
{
	namespace Graphics
	{
		class BaseMesh
		{
		public:

			int id;

			ScePspFMatrix4 matrix;

			Math::BoundingBox aabb;

			void *vertices;
		};
	}
}

#endif
