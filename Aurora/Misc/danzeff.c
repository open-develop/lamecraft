#include "danzeff.h"
#include "pspkernel.h"

// headerfiles needed for the GU and VRAM versions
#if defined (DANZEFF_SCEGU) || defined (DANZEFF_VRAM)
#include <malloc.h>
#include "pspgu.h"
#include "png.h"
#endif // defined (DANZEFF_SCEGU) || defined (DANZEFF_VRAM)

#define false 0
#define true 1

// structures used for drawing the keyboard
#if defined (DANZEFF_SCEGU) || defined (DANZEFF_VRAM)
	struct danzeff_vertex
	{
		float u, v;
		unsigned int color;
		float x,y,z;
	};

	struct danzeff_gu_surface
	{
		u32		surface_width;
		u32		surface_height;
		u32		texture_width;
		u32		texture_height;
		u32		*texture;
	};
#endif //defined (DANZEFF_SCEGU) || defined (DANZEFF_VRAM)


/*bool*/ int holding = false;     //user is holding a button
/*bool*/ int dirty = true;        //keyboard needs redrawing
/*bool*/ int shifted = false;     //user is holding shift
int mode = 0;             //charset selected. (0 - letters or 1 - numbers)
/*bool*/ int initialized = false; //keyboard is initialized
int orientation = DANZEFF_ORIENT_UP;

//Position on the 3-3 grid the user has selected (range 0-2)
int selected_x = 1;
int selected_y = 1;

// location that we are moved to
int moved_x = 0;
int moved_y = 0;

//Variable describing where each of the images is
#define guiStringsSize 12 /* size of guistrings array */
#define PICS_BASEDIR "./graphics/"
char *guiStrings[] =
{
	PICS_BASEDIR "keys.png", PICS_BASEDIR "keys_t.png", PICS_BASEDIR "keys_s.png",
	PICS_BASEDIR "keys_c.png", PICS_BASEDIR "keys_c_t.png", PICS_BASEDIR "keys_s_c.png",
	PICS_BASEDIR "nums.png", PICS_BASEDIR "nums_t.png", PICS_BASEDIR "nums_s.png",
	PICS_BASEDIR "nums_c.png", PICS_BASEDIR "nums_c_t.png", PICS_BASEDIR "nums_s_c.png"
};

//amount of modes (non shifted), each of these should have a corresponding shifted mode.
#define MODE_COUNT 2
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

/*bool*/ int danzeff_isinitialized()
{
	return initialized;
}

/*bool*/ int danzeff_dirty()
{
	return dirty;
}

/** Attempts to read a character from the controller
* If no character is pressed then we return 0
* Other special values: 1 = move left, 2 = move right, 3 = select, 4 = start
* Every other value should be a standard ascii value.
* An unsigned int is returned so in the future we can support unicode input
*/
unsigned int danzeff_readInput(SceCtrlData pspctrl)
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

///-------------------------------------------------------------------------------
///These are specific to the implementation, they should have the same behaviour across implementations.
///-------------------------------------------------------------------------------


///-------------------------------------------------------------------------------
///This is the original SDL implementation
#ifdef DANZEFF_SDL

SDL_Surface* keyBits[guiStringsSize];
int keyBitsSize = 0;

///variable needed for rendering in SDL, the screen surface to draw to, and a function to set it!
SDL_Surface* danzeff_screen;
SDL_Rect danzeff_screen_rect;
void danzeff_set_screen(SDL_Surface* screen)
{
	danzeff_screen = screen;
	danzeff_screen_rect.x = 0;
	danzeff_screen_rect.y = 0;
	danzeff_screen_rect.h = screen->h;
	danzeff_screen_rect.w = screen->w;
}


///Internal function to draw a surface internally offset
//Render the given surface at the current screen position offset by screenX, screenY
//the surface will be internally offset by offsetX,offsetY. And the size of it to be drawn will be intWidth,intHeight
void surface_draw_offset(SDL_Surface* pixels, int screenX, int screenY, int offsetX, int offsetY, int intWidth, int intHeight)
{
	//move the draw position
	danzeff_screen_rect.x = moved_x + screenX;
	danzeff_screen_rect.y = moved_y + screenY;

	//Set up the rectangle
	SDL_Rect pixels_rect;
	pixels_rect.x = offsetX;
	pixels_rect.y = offsetY;
	pixels_rect.w = intWidth;
	pixels_rect.h = intHeight;

	SDL_BlitSurface(pixels, &pixels_rect, danzeff_screen, &danzeff_screen_rect);
}

///Draw a surface at the current moved_x, moved_y
void surface_draw_sdl(SDL_Surface* pixels)
{
	surface_draw_offset(pixels, 0, 0, 0, 0, pixels->w, pixels->h);
}

/* load all the guibits that make up the OSK */
void danzeff_load()
{
	if (initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		keyBits[a] = IMG_Load(guiStrings[a]);
		if (keyBits[a] == NULL)
		{
			//ERROR! out of memory.
			//free all previously created surfaces and set initialized to false
			int b;
			for (b = 0; b < a; b++)
			{
				SDL_FreeSurface(keyBits[b]);
				keyBits[b] = NULL;
			}
			initialized = false;
			return;
		}
	}
	initialized = true;
}

void danzeff_load_lite()
{
	if (initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		if (!((a-1)%3)) //skip loading the _t files
		{
			keyBits[a] = NULL;
			continue;
		}
		keyBits[a] = IMG_Load(guiStrings[a]);
		if (keyBits[a] == NULL)
		{
			//ERROR! out of memory.
			//free all previously created surfaces and set initialized to false
			int b;
			for (b = 0; b < a; b++)
			{
				SDL_FreeSurface(keyBits[b]);
				keyBits[b] = NULL;
			}
			initialized = false;
			return;
		}
	}
	initialized = true;
}


/* remove all the guibits from memory */
void danzeff_free()
{
	if (!initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		SDL_FreeSurface(keyBits[a]);
		keyBits[a] = NULL;
	}
	initialized = false;
}

/* draw the keyboard at the current position */
void danzeff_render()
{
	printf("Drawing Keyboard at %i,%i\n", selected_x, selected_y);
	dirty = false;

	///Draw the background for the selected keyboard either transparent or opaque
	///this is the whole background image, not including the special highlighted area
	//if centered or loaded lite then draw the whole thing opaque
	if ((selected_x == 1 && selected_y == 1) || keyBits[6*mode + shifted*3 + 1] == NULL)
		surface_draw_sdl(keyBits[6*mode + shifted*3]);
	else
		surface_draw_sdl(keyBits[6*mode + shifted*3 + 1]);

	///Draw the current Highlighted Selector (orange bit)
	surface_draw_offset(keyBits[6*mode + shifted*3 + 2],
	//Offset from the current draw position to render at
	selected_x*43, selected_y*43,
	//internal offset of the image
	selected_x*64,selected_y*64,
	//size to render (always the same)
	64, 64);
}

#endif //DANZEFF_SDL


///-------------------------------------------------------------------------------
///This is the GU implementation
#ifdef DANZEFF_SCEGU

struct danzeff_gu_surface	keyTextures[guiStringsSize];
int private_gu_Bpp = 4;
int private_gu_pixel_color_format = GU_PSM_8888;
int private_gu_texture_color_format = GU_COLOR_8888;

void danzeff_set_screen(int Bpp, int PixelColorFormat, int TextureColorFormat)
{
	private_gu_Bpp = Bpp;
	private_gu_pixel_color_format = PixelColorFormat;
	private_gu_texture_color_format = TextureColorFormat;
}

///Internal function to draw a surface internally offset
//Render the given surface at the current screen position offset by screenX, screenY
//the surface will be internally offset by offsetX,offsetY. And the size of it to be drawn will be intWidth,intHeight
void surface_draw_offset(struct danzeff_gu_surface* surface, int screenX, int screenY, int offsetX, int offsetY, int intWidth, int intHeight, int transperant)
{
	float	temp_u, temp_v;

	sceGuEnable( GU_TEXTURE_2D );
	sceGuAlphaFunc( GU_GREATER, 0, 0xff );
	sceGuEnable( GU_ALPHA_TEST );
	sceGuTexFunc(GU_TFX_BLEND,GU_TCC_RGBA);
	sceGuTexEnvColor(0xFF000000);
	sceGuBlendFunc( GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0 );
	sceGuEnable( GU_BLEND );
	sceGuTexMode(private_gu_pixel_color_format,0,0,GU_FALSE);
	sceGuTexImage(0,surface->surface_width, surface->surface_height,surface->surface_width, surface->texture);
	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);

	struct danzeff_vertex* c_vertices = (struct danzeff_vertex*)sceGuGetMemory(6 * sizeof(struct danzeff_vertex));

	c_vertices[0].u 		= offsetX;
	c_vertices[0].v 		= offsetY;
	c_vertices[0].x 		= moved_x + screenX;
	c_vertices[0].y 		= moved_y + screenY;
	c_vertices[0].z 		= 0;

	c_vertices[1].u 		= offsetX + intWidth;
	c_vertices[1].v 		= offsetY;
	c_vertices[1].x 		= moved_x + screenX + intWidth;
	c_vertices[1].y 		= moved_y + screenY;
	c_vertices[1].z 		= 0;

	c_vertices[2].u 		= offsetX;
	c_vertices[2].v 		= offsetY + intHeight;
	c_vertices[2].x 		= moved_x + screenX;
	c_vertices[2].y 		= moved_y + screenY + intHeight;
	c_vertices[2].z 		= 0;

	memcpy(&c_vertices[3], &c_vertices[2], sizeof(struct danzeff_vertex));
	memcpy(&c_vertices[4], &c_vertices[1], sizeof(struct danzeff_vertex));

	c_vertices[5].u 		= offsetX + intWidth;
	c_vertices[5].v 		= offsetY + intHeight;
	c_vertices[5].x 		= moved_x + screenX + intWidth;
	c_vertices[5].y 		= moved_y + screenY + intHeight;
	c_vertices[5].z 		= 0;

	if (orientation == DANZEFF_ORIENT_RIGHT)
		{
		temp_u = c_vertices[1].u;
		temp_v = c_vertices[1].v;

		c_vertices[1].u = c_vertices[0].u;
		c_vertices[4].u = c_vertices[0].u;
		c_vertices[1].v = c_vertices[0].v;
		c_vertices[4].v = c_vertices[0].v;

		c_vertices[0].u = c_vertices[2].u;
		c_vertices[0].v = c_vertices[2].v;

		c_vertices[2].u = c_vertices[5].u;
		c_vertices[3].u = c_vertices[5].u;
		c_vertices[2].v = c_vertices[5].v;
		c_vertices[3].v = c_vertices[5].v;

		c_vertices[5].u = temp_u;
		c_vertices[5].v = temp_v;
		}

	if (transperant == true)
		{
		c_vertices[0].color 	= 0x80FFFFFF;
		c_vertices[1].color 	= 0x80FFFFFF;
		c_vertices[2].color 	= 0x80FFFFFF;
		c_vertices[3].color 	= 0x80FFFFFF;
		c_vertices[4].color 	= 0x80FFFFFF;
		c_vertices[5].color 	= 0x80FFFFFF;
		}
	else
		{
		c_vertices[0].color 	= 0xFFFFFFFF;
		c_vertices[1].color 	= 0xFFFFFFFF;
		c_vertices[2].color 	= 0xFFFFFFFF;
		c_vertices[3].color 	= 0xFFFFFFFF;
		c_vertices[4].color 	= 0xFFFFFFFF;
		c_vertices[5].color 	= 0xFFFFFFFF;
		}

	sceGuDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|private_gu_texture_color_format|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 6, 0, c_vertices);

	sceGuDisable( GU_BLEND );
	sceGuDisable( GU_ALPHA_TEST );
	sceGuDisable( GU_TEXTURE_2D );
}

void danzeff_block_copy(struct danzeff_gu_surface* surface, u32 *texture)
{
	u32 y, x;
	u32 stride = surface->surface_width - surface->texture_width;

	if (private_gu_Bpp == 2) //16bpp
	{
		u16 *src  = (u16*)texture;
		u16 *dest = (u16*)surface->texture;
	
		for (y = 0 ; y < surface->texture_height ; y++)
		{
			for (x = 0 ; x < surface->texture_width ; x++)
			{
				*dest++ = *src++;
			}
			// skip at the end of each line
			if (stride > 0)
			{
				dest += stride;
			}
		}
	}
	else //32bpp
	{
		u32 *dest = surface->texture;
	
		for (y = 0 ; y < surface->texture_height ; y++)
		{
			for (x = 0 ; x < surface->texture_width ; x++)
			{
			*dest++ = *texture++;
			}
			// skip at the end of each line
			if (stride > 0)
			{
				dest += stride;
			}
		}
	}
}

static void danzeff_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
	// ignore PNG warnings
}

/* Get the width and height of a png image */
int danzeff_get_png_image_size(const char* filename, u32 *png_width, u32 *png_height)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return -1;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, danzeff_user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);

	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);

	*png_width = width;
	*png_height = height;
	return 0;
}

/* Load a texture from a png image */
int danzeff_load_png_image(const char* filename, u32 *ImageBuffer)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	size_t x, y;
	u32* line;
	FILE *fp;
	u16 color16;
	u16 *ImageBuffer16 = (u16*)ImageBuffer;

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return -1;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, danzeff_user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	line = (u32*) malloc(width * 4); /* use 32bpp to load line  */
	if (!line) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}
	if (private_gu_Bpp == 2) /* 16bpp */
	{
		for (y = 0; y < height; y++)
		{
			png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
			for (x = 0; x < width; x++)
			{
				u32 color32 = line[x];
				int r = color32 & 0xff;
				int g = (color32 >> 8) & 0xff;
				int b = (color32 >> 16) & 0xff;
				color16 = (r >> 3) | ((g >> 2) << 5) | ((b >> 3) << 11);
				ImageBuffer16[y*width+x] = color16;
			}
		}
	}
	else /* 32bpp */
	{
		for (y = 0; y < height; y++)
		{
			png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
			for (x = 0; x < width; x++)
			{
				ImageBuffer[y*width+x] = line[x];
			}
		}
	}

	free(line);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);
	return 0;
}

u32 danzeff_convert_pow2(u32 size)
{
	u32 pow_counter = 0;

	for ( ; pow_counter < 32 ; pow_counter++)
	{
		// Find the first value which is higher
		if ((size >> pow_counter) == 0)
		{
			// take already good values into account
			if ((1 << pow_counter) != size)
			{
				return (1 << pow_counter);
			}
			else
			{
				return (1 << (pow_counter-1));
			}
		}
	}
	return 0;
}

/* load all the guibits that make up the OSK */
void danzeff_load()
{
	u32 *temp_texture;

	if (initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		u32 height, width;

		if (!((a-1)%3)) //skip loading the _t files
		{
			keyTextures[a].texture = NULL;
			continue;
		}
		if (danzeff_get_png_image_size(guiStrings[a], &width, &height) == 0)
		{
			temp_texture = (u32 *)malloc(width*height*private_gu_Bpp);
			if (danzeff_load_png_image(guiStrings[a], temp_texture) != 0)
			{
				// Error .. Couldn't get png info from one of the needed files
				int b;

				for (b = 0; b < a; b++)
				{
					free(keyTextures[b].texture);
					keyTextures[b].texture = NULL;
				}
				initialized = false;
				return;
			}
			else
			{
				// we need to store the texture in an image of width and heights of 2^n sizes
				keyTextures[a].texture_width 	= (float)width;
				keyTextures[a].texture_height 	= (float)height;
				keyTextures[a].surface_width 	= (float)danzeff_convert_pow2(width);
				keyTextures[a].surface_height 	= (float)danzeff_convert_pow2(height);
				keyTextures[a].texture 			= (u32 *) malloc(keyTextures[a].surface_width*keyTextures[a].surface_height*private_gu_Bpp);
				// block copy the texture into the surface
				danzeff_block_copy(&keyTextures[a], temp_texture);
				free(temp_texture);
			}
		}
		else
		{
			// Error .. Couldn't get png info from one of the needed files
			int b;

			for (b = 0; b < a; b++)
			{
				free(keyTextures[b].texture);
				keyTextures[b].texture = NULL;
			}
			initialized = false;
			return;
		}
	}
	initialized = true;
}
//These are the same in the Gu Implementation :)
void danzeff_load_lite()
{
	danzeff_load();
}

/* remove all the guibits from memory */
void danzeff_free()
{
	if (!initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		free(keyTextures[a].texture);
		keyTextures[a].texture = NULL;
	}
	initialized = false;
}

/* draw the keyboard at the current position */
void danzeff_render()
{
	int	transperant;
	int	x = selected_x*64;
	int y = selected_y*64;
	dirty = false;

	///Draw the background for the selected keyboard either transparent or opaque
	///this is the whole background image, not including the special highlighted area
	//if center is selected then draw the whole thing opaque
	if (selected_x == 1 && selected_y == 1)
		{
		transperant = false;
		}
	else
		{
		transperant = true;
		}

	surface_draw_offset(&keyTextures[6*mode + shifted*3], 0, 0, 0, 0, keyTextures[6*mode + shifted*3].texture_width,
																	 	 keyTextures[6*mode + shifted*3].texture_height,
																	 	 transperant);

	///Draw the current Highlighted Selector (orange bit) based on the orientation

	if (orientation == DANZEFF_ORIENT_RIGHT)
		{
		x = selected_y*64;
		y = (2-selected_x)*64;
		}

	surface_draw_offset(&keyTextures[6*mode + shifted*3 + 2],
	//Offset from the current draw position to render at
	selected_x*43, selected_y*43,
	//internal offset of the image
	x,y,
	//size to render (always the same)
	64, 64,
	false);
}

#endif //DANZEFF_SCEGU

///-------------------------------------------------------------------------------
///This is the direct VRAM implementation
#ifdef DANZEFF_VRAM

struct danzeff_gu_surface	keyTextures[guiStringsSize];

unsigned char *private_vram_ptr = NULL;
int				private_vram_width = 0;
int				private_vram_height = 0;
int				private_vram_bbp = 0;

void danzeff_set_screen(void* vram_ptr, int width, int height, int bbp)
{
	private_vram_ptr = vram_ptr;
	private_vram_width = width;
	private_vram_height = height;
	private_vram_bbp = bbp;
}

///Internal function to draw a surface internally offset
//Render the given surface at the current screen position offset by screenX, screenY
//the surface will be internally offset by offsetX,offsetY. And the size of it to be drawn will be intWidth,intHeight
void surface_draw_offset(struct danzeff_gu_surface* surface, int screenX, int screenY, int offsetX, int offsetY, int intWidth, int intHeight, int transperant)
{
int				x, y, bpp_counter;
unsigned char	*local_vram_ptr;
unsigned char	*local_texture_ptr;

	local_vram_ptr = private_vram_ptr + (((moved_y + screenY) * private_vram_width * private_vram_bbp) +
									  	 ((moved_x + screenX) * private_vram_bbp));

	local_texture_ptr = (unsigned char *) surface->texture;
	local_texture_ptr += (offsetY * surface->texture_width * private_vram_bbp) +
					  	 (offsetX * private_vram_bbp);

	for (y = offsetY ; y < (offsetY + intHeight) ; y++)
		{
		for (x = offsetX ; x < (offsetX + intWidth) ; x++)
			{
			for (bpp_counter = 0 ; bpp_counter < private_vram_bbp; bpp_counter++)
				{
				*local_vram_ptr++ = *local_texture_ptr++;
				}
			}
		local_vram_ptr += (private_vram_width - intWidth) * private_vram_bbp;
		local_texture_ptr += (surface->texture_width - intWidth) * private_vram_bbp;
		}
}

static void danzeff_user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
	// ignore PNG warnings
}

/* Get the width and height of a png image */
int danzeff_get_png_image_size(const char* filename, u32 *png_width, u32 *png_height)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	FILE *fp;

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return -1;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, danzeff_user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);

	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);

	*png_width = width;
	*png_height = height;
	return 0;
}

/* Load a texture from a png image */
int danzeff_load_png_image(const char* filename, u32 *ImageBuffer)
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	size_t x, y;
	u32* line;
	FILE *fp;
	u16 color16;
	u16 *ImageBuffer16 = (u16*)ImageBuffer;

	if ((fp = fopen(filename, "rb")) == NULL) return -1;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return -1;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, danzeff_user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	line = (u32*) malloc(width * 4); /* use 32bpp to load line  */
	if (!line) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return -1;
	}

	if (private_vram_bbp == 2) /* 16bpp */
	{
		for (y = 0; y < height; y++)
		{
			png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
			for (x = 0; x < width; x++)
			{
				u32 color32 = line[x];
				int r = color32 & 0xff;
				int g = (color32 >> 8) & 0xff;
				int b = (color32 >> 16) & 0xff;
				color16 = (r >> 3) | ((g >> 2) << 5) | ((b >> 3) << 11);
				ImageBuffer16[y*width+x] = color16;
			}
		}
	}
	else /* 32bpp */
	{
		for (y = 0; y < height; y++)
		{
			png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
			for (x = 0; x < width; x++)
			{
				ImageBuffer[y*width+x] = line[x];
			}
		}
	}

	free(line);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);
	return 0;
}

/* load all the guibits that make up the OSK */
void danzeff_load()
{
	u32 *temp_texture;

	if (initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		u32 height, width;

		if (!((a-1)%3)) //skip loading the _t files
		{
			keyTextures[a].texture = NULL;
			continue;
		}
		if (danzeff_get_png_image_size(guiStrings[a], &width, &height) == 0)
		{
			temp_texture = (u32 *)malloc(width*height*private_vram_bbp);
			if (danzeff_load_png_image(guiStrings[a], temp_texture) != 0)
			{
				// Error .. Couldn't get png info from one of the needed files
				int b;

				for (b = 0; b < a; b++)
				{
					free(keyTextures[b].texture);
					keyTextures[b].texture = NULL;
				}
				initialized = false;
				return;
			}
			else
			{
				// we need to store the texture in an image of width and heights of 2^n sizes
				keyTextures[a].texture_width 	= (float)width;
				keyTextures[a].texture_height 	= (float)height;
				keyTextures[a].texture 			= (u32 *)temp_texture;
			}
		}
		else
		{
			// Error .. Couldn't get png info from one of the needed files
			int b;

			for (b = 0; b < a; b++)
			{
				free(keyTextures[b].texture);
				keyTextures[b].texture = NULL;
			}
			initialized = false;
			return;
		}
	}
	initialized = true;
}

//These are the same in the VRAM Implementation :)
void danzeff_load_lite()
{
	danzeff_load();
}

/* remove all the guibits from memory */
void danzeff_free()
{
	if (!initialized) return;

	int a;
	for (a = 0; a < guiStringsSize; a++)
	{
		free(keyTextures[a].texture);
		keyTextures[a].texture = NULL;
	}
	initialized = false;
}

/* draw the keyboard at the current position */
void danzeff_render()
{
	int	transperant;
	int	x = selected_x*64;
	int y = selected_y*64;
	dirty = false;

	///Draw the background for the selected keyboard either transparent or opaque
	///this is the whole background image, not including the special highlighted area
	//if center is selected then draw the whole thing opaque
	if (selected_x == 1 && selected_y == 1)
		{
		transperant = false;
		}
	else
		{
		transperant = true;
		}

	surface_draw_offset(&keyTextures[6*mode + shifted*3], 0, 0, 0, 0, keyTextures[6*mode + shifted*3].texture_width,
																 	  keyTextures[6*mode + shifted*3].texture_height,
																	  transperant);

	///Draw the current Highlighted Selector (orange bit) based on the orientation

	if (orientation == DANZEFF_ORIENT_RIGHT)
		{
		x = selected_y*64;
		y = (2-selected_x)*64;
		}

	surface_draw_offset(&keyTextures[6*mode + shifted*3 + 2],
	//Offset from the current draw position to render at
	selected_x*43, selected_y*43,
	//internal offset of the image
	x,y,
	//size to render (always the same)
	64, 64,
	false);
}

#endif //DANZEFF_VRAM

/* move the position the keyboard is currently drawn at */
void danzeff_moveTo(const int newX, const int newY)
{
	moved_x = newX;
	moved_y = newY;
	dirty = true;
}

void danzeff_set_orientation(const int new_orientation)
{
	orientation = new_orientation;
}
