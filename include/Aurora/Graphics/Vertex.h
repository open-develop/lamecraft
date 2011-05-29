#ifndef AURORA_VERTEX_H
#define AURORA_VERTEX_H

#include <Aurora/Math/Vector3.h>

namespace Aurora
{
	namespace Graphics
	{	
		class Color
		{
		public:

			Color()
			{
				R = G = B = 0.0f;
			}

			Color(float r,float g,float b)
			{
				R = r;
				G = g;
				B = b;
			}

			float R,G,B;
		};

		class Face
		{
		public:

			Face(Color fColor,int v1,int v2, int v3)
			{
				color = fColor;
				V1 = v1;
				V2 = v2;
				V3 = v3;
			}

			Color color;
			int V1,V2,V3;
		};

		typedef struct
		{
			float x,y,z;
		}__attribute__((aligned(16))) BBVertex;

		typedef struct
		{
			unsigned int color;
			float x,y,z;
		}__attribute__((packed)) SimplePSPVertex;

		typedef struct
		{
			float u,v;
			unsigned int color;
			float x,y,z;
		}__attribute__((packed)) CraftPSPVertex;

		typedef struct
		{
			float u,v;
			float x,y,z;
		}__attribute__((packed)) TexturesPSPVertex;

		typedef struct
		{
			unsigned short u, v;
			short x, y, z;
		}__attribute__((packed)) TexturesPSPVertex16;

		typedef struct
		{
			unsigned char u, v;
			char x, y, z;
		}__attribute__((packed)) TexturesPSPVertex8;


		//{32767	, 32767	, 0xc21f, 32767, 32767, 32767}, // 5
		//sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_16BIT|GU_COLOR_4444|GU_VERTEX_16BIT|GU_TRANSFORM_3D,12*3,0,vertices16);


		struct MorphVertex
		{
			TexturesPSPVertex v0;
			TexturesPSPVertex v1;
		};

		/*class Vertex
		{
		public:

			Vertex(Math::Vector3 position)			  
			{
				Position = position;
			}

			Math::Vector3 Position;
			Math::Vector3 Normal;
			Math::Vector3 UV;
		};*/
	}
}

#endif
