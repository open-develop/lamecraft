#ifndef AURORA_NODE_H
#define AURORA_NODE_H

#include <Aurora\Math\Vector3.h>
#include <string>

namespace Aurora
{
	namespace Graphics
	{	
		class Node
		{
		public:

			Node();
			Node(std::string name,Math::Vector3 position);
			Node(std::string name,Math::Vector3 position,Math::Vector3 scale);

			std::string Name;

			Math::Vector3 Position;
			Math::Vector3 Scale;
			Math::Vector3 Rotation;
		};
	}
}

#endif