#ifndef INCLUDED_KEYBOARDS_DANZEFF_H
#define INCLUDED_KEYBOARDS_DANZEFF_H

//danzeff is BSD licensed, if you do make a new renderer then please share it back and I can add it
//to the original distribution.

//Danzeff always uses pspctrl.h, if you want to build it on pc then add it to your includes
// then use the pspctrlemu lib to convert a SDL_Joystick into a SceCtrlData
#include <pspctrl.h>

#ifdef __cplusplus
extern "C" {
#endif

//Initialization and de-init of the keyboard, provided as the keyboard uses alot of images, so if you aren't going to use it for a while, I'd recommend unloading it.
void danzeff_load();
//Loads the danzeff keyboard without the transparent images (_t images) (NO DIFFERENCE IN GU IMPLEMENTATION)
void danzeff_load_lite();
void danzeff_free();

//returns true if the keyboard is initialized
/*bool*/ int danzeff_isinitialized();

/** Attempts to read a character from the controller
* If no character is pressed then we return 0
* Other special values: 1 = move left, 2 = move right, 3 = select, 4 = start
* Every other value should be a standard ascii value.
* An unsigned int is returned so in the future we can support unicode input
*/
unsigned int danzeff_readInput(SceCtrlData pspctrl);
#define DANZEFF_LEFT   1
#define DANZEFF_RIGHT  2
#define DANZEFF_SELECT 3
#define DANZEFF_START  4

//Move the area the keyboard is rendered at to here
void danzeff_moveTo(const int newX, const int newY);

#define DANZEFF_ORIENT_UP     1  /* Normal rendering       */
#define DANZEFF_ORIENT_RIGHT  2  /* Rotated 90 degrees CW  */

void danzeff_set_orientation(const int new_orientation);

//Returns true if the keyboard that would be rendered now is different to the last time
//that the keyboard was drawn, this is altered by readInput/render.
/*bool*/ int danzeff_dirty();

//draw the keyboard to the screen
void danzeff_render();


///Functions only for particular renderers:

#ifdef DANZEFF_SDL ///Functions only for SDL Renderer
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//set the screen surface for rendering on.
void danzeff_set_screen(SDL_Surface* screen);
#endif //DANZEFF_SDL

#ifdef DANZEFF_VRAM ///Functions only for direct VRAM Renderer
//set the VRAM pointer for rendering.
void danzeff_set_screen(void* vram_ptr, int width, int height, int bbp);
#endif //DANZEFF_VRAM

#ifdef DANZEFF_SCEGU ///Functions only for GU Renderer
//Set pixel size (Bytes per pixel) and properties
//Example for 32bpp(4Bpp): danzeff_set_screen(4, GU_PSM_8888, GU_COLOR_8888);
void danzeff_set_screen(int Bpp, int PixelColorFormat, int TextureColorFormat);
#endif

#ifdef __cplusplus
}
#endif

#endif //INCLUDED_KEYBOARDS_DANZEFF_H
