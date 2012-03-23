#ifndef SPRITE_H_
#define SPRITE_H_

#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/Vertex.h>

#include <string>

namespace Aurora
{
	namespace Graphics
	{
		class Sprite
		{
		public:

			Sprite();
			Sprite(const char* filename);
			Sprite(const char* filename,int startW,int startH,int endW,int endH);
			~Sprite();


			void SetPosition(float x,float y);
			void Scale(float x,float y);
			void Draw();
			void RemoveImage();

		private:

			std::string imageName;

			TexturesPSPVertex *vertices;

			float scaleX,scaleY;
			float posX,posY;

			int width,height;
		};

	}

}

#endif
