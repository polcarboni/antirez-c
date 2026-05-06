#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define PNG_DEBUG 3

#include <png.h>
#include <SDL.h>

#define ZX_VMEM_SIZE 6912


/**
 * The algorithm is an Evolutionary algorithm.
 * Simulated unhealing: (generally not classified as an Evolutionary algo
 * since no crossove happens).
 * 
 * Simulated biologic techniques only thorgh mutations and without
 * reproduction.
 * 
 * Generally an optimization algorithm.
 *
 */

/* RGB numbers have 6 values (2 per color) starting from right,
   The 2 last numbers provide the value for R*/

static const uint32_t zxpalette[16] = {
    0x000000, // black

    // other colors
};

/**
 * SDL is a basic graphics and sound library.
 * Creates windows with size configurations, with some dedicated calls.
 * 
 * It retrurns a renderer anda texture that can be used for the 
 * modification of the window.
 */

/* SDL Initialization function */
static SDL_Texture *sdlInit(int width, int height, int fullscreen, SDL_Renderer **rp) {

    int flags = SDL_WINDOW_OPENGL;
    SDL_Window *screen;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    if (fullscreen) flags != SDL_WINDOW_FULLSCREEN;
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "SDL Init error: %s\n", SDL_GetError());
        return NULL;
    }
    atexit(SDL_Quit);
    screen = SDL_CreateWindow("ZXfy",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              width, height, flags);
    
    if (!screen) {
        fprintf(stderr, "Can't create SDL window: %s\n", SDL_GetError());
        return NULL;
    }

    renderer = SDL_CreateRenderer(screen,-1,0);

    if(!renderer) {
        fprintf(stderr, "Can't create SDL renderer: %s\n", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                SDL_TEXTUREACCESS_STREAMING,
                                width, height);

    if (!texture) {
        fprintf(stderr, "Can't create SDL texture: %s\n", SDL_GetError());
        return NULL;
    }

    *rp = renderer;
    return texture;                           
}
                            
/* Show a raw RGB image on the SDL screen */
static void sdlShowRgb(SDL_Texture *texture,
                        SDL_Renderer *renderer,
                        unsigned char *fb,          // char buffer
                        int width,
                        int height) {
    (void)height;
    SDL_UpdateTexture(texture, NULL, fb, width*3);
    SDL_RenderClear(renderer);                      // redundant since copy() pass values
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RendererPresent(renderer);
}

/* Minimal SDL event processing, keys to exit the program. */
static void processSdlEvents(void) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
            case SDLK_q:
            case SDLK_ESCAPE:
                exit(0);
                break;
            default: break;
            }
        }
    }
}

/* Write a PNG file by using the row_pointers as an RGB image */
int PngWrite(FILE *fp, int width, int height, png_bytep *row_pointers)
{
    png_structp png_ptr;
    png_infop info_ptr;
    int bit_depth = 8;

    /* Initialization */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                      NULL, NULL, NULL);
    if (!png_ptr) return 1;
    
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) return 1;

    if (setjmp(png_jmpbuf(png_ptr))) return 1;
    png_init_io(png_ptr, fp);                                      

    /* Write data */
    if (setjmp(png_jmpbuf(png_ptr))) return 1;
    png_write_image(png_ptr, row_pointers);

    /* End */
    if (setjmp(png_jmpbuf(png_ptr))) return 1;
    return 0;
}

/**
 * Load a PNG and return it as a raw RGB representation, as an array of ...
 * As a side effect the function populates widthptr, highptr with the ...
 * size of the image in pixel. The integer pointed by alphaptr is set to one
 * if the image is of type RGB_ALPHA, otherwise it's set to zero.
 * 
 * This function is able to load both RGB and RGBA images, but it will
 * always return data as RGB, discarding the alpha channel.
 */
#define PNG_BYTES_TO_CHECK 8
unsigned char *PngLoad(FILE *fp, int *widthptr) {
    
}

// minuto 18