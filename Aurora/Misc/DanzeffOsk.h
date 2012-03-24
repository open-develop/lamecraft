#ifndef DANZEFFOSK_H_
#define DANZEFFOSK_H_

#include <pspctrl.h>
#include <malloc.h>
#include <pspgu.h>
#include <png.h>

#define DANZEFF_LEFT   1
#define DANZEFF_RIGHT  2
#define DANZEFF_SELECT 3
#define DANZEFF_START  4

#define DANZEFF_ORIENT_UP     1
#define DANZEFF_ORIENT_RIGHT  2

//amount of modes (non shifted), each of these should have a corresponding shifted mode.
#define MODE_COUNT 2

#define guiStringsSize 12 /* size of guistrings array */

//images dir
#define PICS_BASEDIR "Assets/Danzeff/"

#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/Vertex.h>

namespace Aurora
{
	namespace Misc
	{

		class DanzeffOsk
		{
		private:

			int holding;
			int dirty;
			int shifted;
			int mode;
			int initialized;
			int orientation;
			int selected_x;
			int selected_y;
			int moved_x;
			int moved_y;

			Graphics::Image* _img[guiStringsSize];

		private:

			void _setVertex(Graphics::TexturesPSPVertex &vert,float u,float v,float x, float y,float z);
			void _draw(Graphics::Image* img, int screenX, int screenY, int offsetX, int offsetY, int Width, int Height, int transperant);
			void _draw2(Graphics::Image* img, int screenX, int screenY, int offsetX, int offsetY, int Width, int Height, int transperant);

		public:

			DanzeffOsk();
			~DanzeffOsk();

			bool IsInitialized();
			unsigned int ProcessInput(SceCtrlData pspctrl);
			void Move(int newX, int newY);

			void Load();
			void Delete();

			void Render();
		};

	}

}

#endif

