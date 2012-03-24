#include <Aurora/Misc/DanzeffOsk.h>

namespace Aurora
{
	namespace Misc
	{
		const char *guiStrings[] =
		{
			PICS_BASEDIR "keys.png", PICS_BASEDIR "keys_t.png", PICS_BASEDIR "keys_s.png",
			PICS_BASEDIR "keys_c.png", PICS_BASEDIR "keys_c_t.png", PICS_BASEDIR "keys_s_c.png",
			PICS_BASEDIR "nums.png", PICS_BASEDIR "nums_t.png", PICS_BASEDIR "nums_s.png",
			PICS_BASEDIR "nums_c.png", PICS_BASEDIR "nums_c_t.png", PICS_BASEDIR "nums_s_c.png"
		};

		//this is the layout of the keyboard
		char modeChar[MODE_COUNT*2][3][3][5] =
		{
			{	//standard letters
				{ ",abc",  ".def","!ghi" },
				{ "-jkl","\010m n", "?opq" },
				{ "(rst",  ":uvw",")xyz" }
			},

			{	//capital letters
				{ "^ABC",  "@DEF","*GHI" },
				{ "_JKL","\010M N", "\"OPQ" },
				{ "=RST",  ";UVW","/XYZ" }
			},

			{	//numbers
				{ "\0\0\0001","\0\0\0002","\0\0\0003" },
				{ "\0\0\0004",  "\010\0 5","\0\0\0006" },
				{ "\0\0\0007","\0\0\0008", "\0\00009" }
			},

			{	//special characters
				{ ",(.)",  "\"<'>","-[_]" },
				{ "!{?}","\010\0 \0", "+\\=/" },
				{ ":@;#",  "~$`%","*^|&" }
			}
		};

		// Rotated 90 degrees CW
		char modeCharRight[MODE_COUNT*2][3][3][5] =
		{
			{	//standard letters
				{ "rst(", "jkl-"   , "abc," },
				{ "uvw:", "m n\010", "def." },
				{ "xyz)", "opq?"   , "ghi!" }

			},

			{	//capital letters
				{ "RST=", "JKL_"   , "ABC^" },
				{ "UVW;", "M N\010", "DEF@"},
				{ "XYZ/", "OPQ\""  , "GHI*"}
			},

			{	//numbers
				{ "\0\0007\0", "\0\0004\0","\0\0001\0"},
				{ "\0\0008\0", "\0 5\010" ,"\0\0002\0"},
				{ "\00009\0", "\0\0006\0","\0\0003\0"}
			},

			{	//special characters
				{ "@;#:", "{?}!"     , "(.),"},
				{ "$`%~", "\0 \0\010", "<'>\""},
				{ "^|&*", "\\=/+"    , "[_]-"}
			}
		};

		DanzeffOsk::DanzeffOsk()
		{
			holding = false;     //user is holding a button
			dirty = true;        //keyboard needs redrawing
			shifted = false;     //user is holding shift
			mode = 0;            //charset selected. (0 - letters or 1 - numbers)
			initialized = false; //keyboard is initialized
			orientation = DANZEFF_ORIENT_UP;

			//Position on the 3-3 grid the user has selected (range 0-2)
			selected_x = 1;
			selected_y = 1;

			// location that we are moved to
			moved_x = 0;
			moved_y = 0;
		}

		DanzeffOsk::~DanzeffOsk()
		{
			Delete();
		}

		bool DanzeffOsk::IsInitialized()
		{
			return initialized;
		}

		unsigned int DanzeffOsk::ProcessInput(SceCtrlData pspctrl)
		{
			//For rotated keyboards
			u32	up_key, down_key, left_key, right_key;

			if (orientation == DANZEFF_ORIENT_RIGHT)
			{
				up_key    = PSP_CTRL_RIGHT;
				down_key  = PSP_CTRL_LEFT;
				left_key  = PSP_CTRL_UP;
				right_key = PSP_CTRL_DOWN;
			}
			else
			{
				up_key    = PSP_CTRL_UP;
				down_key  = PSP_CTRL_DOWN;
				left_key  = PSP_CTRL_LEFT;
				right_key = PSP_CTRL_RIGHT;
			}

			//Work out where the analog stick is selecting
			int x = 1;
			int y = 1;
			if (pspctrl.Lx < 85)     x -= 1;
			else if (pspctrl.Lx > 170) x += 1;

			if (pspctrl.Ly < 85)     y -= 1;
			else if (pspctrl.Ly > 170) y += 1;

			if (selected_x != x || selected_y != y) //If they've moved, update dirty
			{
				dirty = true;
				selected_x = x;
				selected_y = y;
			}
			//if they are changing shift then that makes it dirty too
			if ((!shifted && (pspctrl.Buttons & PSP_CTRL_RTRIGGER)) || (shifted && !(pspctrl.Buttons & PSP_CTRL_RTRIGGER)))
				dirty = true;

			unsigned int pressed = 0; //character they have entered, 0 as that means 'nothing'
			shifted = (pspctrl.Buttons & PSP_CTRL_RTRIGGER)?true:false;

			if (!holding)
			{
				if (pspctrl.Buttons& (PSP_CTRL_CROSS|PSP_CTRL_CIRCLE|PSP_CTRL_TRIANGLE|PSP_CTRL_SQUARE)) //pressing a char select button
				{
					int innerChoice = 0;
					if      (pspctrl.Buttons & PSP_CTRL_TRIANGLE)
						innerChoice = 0;
					else if (pspctrl.Buttons & PSP_CTRL_SQUARE)
						innerChoice = 1;
					else if (pspctrl.Buttons & PSP_CTRL_CROSS)
						innerChoice = 2;
					else //if (pspctrl.Buttons & PSP_CTRL_CIRCLE)
						innerChoice = 3;

					//Now grab the value out of the array
					if (orientation == DANZEFF_ORIENT_RIGHT)
					{
						pressed = modeCharRight[ mode*2 + shifted][y][x][innerChoice];
					}
					else
					{
						pressed = modeChar[ mode*2 + shifted][y][x][innerChoice];
					}
				}
				else if (pspctrl.Buttons& PSP_CTRL_LTRIGGER) //toggle mode
				{
					dirty = true;
					mode++;
					mode %= MODE_COUNT;
				}
				else if (pspctrl.Buttons& down_key)
				{
					pressed = '\n';
				}
				else if (pspctrl.Buttons& up_key)
				{
					pressed = 8; //backspace
				}
				else if (pspctrl.Buttons& left_key)
				{
					pressed = DANZEFF_LEFT; //LEFT
				}
				else if (pspctrl.Buttons& right_key)
				{
					pressed = DANZEFF_RIGHT; //RIGHT
				}
				else if (pspctrl.Buttons& PSP_CTRL_SELECT)
				{
					pressed = DANZEFF_SELECT; //SELECT
				}
				else if (pspctrl.Buttons& PSP_CTRL_START)
				{
					pressed = DANZEFF_START; //START
				}
			}

			holding = pspctrl.Buttons & ~PSP_CTRL_RTRIGGER; //RTRIGGER doesn't set holding

			return pressed;
		}

		void DanzeffOsk::Load()
		{
			int a;
			for (a = 0; a < guiStringsSize; a++)
			{
				if (!((a-1)%3)) //skip loading the _t files
				{
					continue;
				}else
				{
					Graphics::TextureManager::Instance()->LoadTexture(guiStrings[a]);
					_img[a] = Graphics::TextureManager::Instance()->GetImage(guiStrings[a]);
				}
			}
		}

		void DanzeffOsk::Delete()
		{
			int a;
			for (a = 0; a < guiStringsSize; a++)
			{
				if (!((a-1)%3)) //skip loading the _t files
				{
					continue;
				}else
				{
					Graphics::TextureManager::Instance()->RemoveTexture(guiStrings[a]);
				}
			}
		}

		void DanzeffOsk::Move(int newX, int newY)
		{
			moved_x = newX;
			moved_y = newY;
		}

		void DanzeffOsk::_setVertex(Graphics::TexturesPSPVertex &vert,float u,float v,float x, float y,float z)
		{
			vert.u = u;
			vert.v = v;
			vert.x = moved_x + x;
			vert.y = moved_y + y;
			vert.z = z;
		}



		void DanzeffOsk::_draw(Graphics::Image* img, int screenX, int screenY, int offsetX, int offsetY, int Width, int Height, int transperant)
		{
			Graphics::TexturesPSPVertex *vertices = (Graphics::TexturesPSPVertex*)sceGuGetMemory(4 * sizeof(Graphics::TexturesPSPVertex) );

			float hPercent = (float)img->Height / (float)img->power2Height;
			float wPercent = (float)img->Width / (float)img->power2Width;

			_setVertex(vertices[0],0.0f,0.0f,0,0,0.0f);
			_setVertex(vertices[1],0.0f,hPercent,0, img->Height,0.0f);
			_setVertex(vertices[2],wPercent,0.0f,img->Width,0,0.0f);
			_setVertex(vertices[3],wPercent,hPercent,img->Width, img->Height,0.0f);

			sceGumPushMatrix();

			ScePspFVector3 loc = {screenX,screenY,0.0f};
			sceGumTranslate(&loc);

			sceGuEnable(GU_TEXTURE_2D);
			Graphics::TextureManager::Instance()->SetTexture(img,GU_NEAREST,GU_NEAREST);

			if (transperant == true)
				sceGuColor(0x80FFFFFF);
			else
				sceGuColor(0xFFFFFFFF);

			sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);

			sceGuDisable(GU_TEXTURE_2D);
			sceGumPopMatrix();
		}

		void DanzeffOsk::_draw2(Graphics::Image* img, int screenX, int screenY, int offsetX, int offsetY, int Width, int Height, int transperant)
		{
			Graphics::TexturesPSPVertex *vertices = (Graphics::TexturesPSPVertex*)sceGuGetMemory(4 * sizeof(Graphics::TexturesPSPVertex) );

			float hPercent = (float)img->Height / (float)img->power2Height;
			float wPercent = (float)img->Width / (float)img->power2Width;

			float startU = ((float)offsetX / (float)img->Width) * wPercent;
			float endU = ((float)(offsetX + Width) / (float)img->Width) * wPercent;

			float startV = ((float)offsetY / (float)img->Height) * hPercent;
			float endV = ((float)(offsetY + Height) / (float)img->Height) * hPercent;

			_setVertex(vertices[0],startU,startV,0,0,0.0f);
			_setVertex(vertices[1],startU,endV,0, Height,0.0f);
			_setVertex(vertices[2],endU,startV,Width,0,0.0f);
			_setVertex(vertices[3],endU,endV,Width, Height,0.0f);

			sceGumPushMatrix();

			ScePspFVector3 loc = {screenX,screenY,0.0f};
			sceGumTranslate(&loc);

			sceGuEnable(GU_TEXTURE_2D);
			Graphics::TextureManager::Instance()->SetTexture(img,GU_NEAREST,GU_NEAREST);

			if (transperant == true)
				sceGuColor(0x80FFFFFF);
			else
				sceGuColor(0xFFFFFFFF);

			sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);

			sceGuDisable(GU_TEXTURE_2D);
			sceGumPopMatrix();
		}

		void DanzeffOsk::Render()
		{
			int	transperant;
			int	x = selected_x*64;
			int y = selected_y*64;

			if (selected_x == 1 && selected_y == 1)
				transperant = false;
			else
				transperant = true;

			//draw background
			_draw(_img[6*mode + shifted*3], 0, 0, 0, 0, _img[6*mode + shifted*3]->Width,_img[6*mode + shifted*3]->Height,transperant);

			//draw the current Highlighted Selector (orange bit) based on the orientation
			_draw2(_img[6*mode + shifted*3 + 2],
				//Offset from the current draw position to render at
				selected_x*43, selected_y*43,
				//internal offset of the image
				x,y,
				//size to render (always the same)
				64, 64,
				false);
		}
	}
}
