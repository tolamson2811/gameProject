#ifndef UTILS_H_
#define UTILS_H_

#include <windows.h>
#include <string>
#include <SDL2\SDL.h>
#include <SDL2\SDL_image.h>
#include <SDL2\SDL_mixer.h>
#include <SDL2\SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Renderer* g_img_menu = NULL;
static SDL_Event g_event;
static Mix_Chunk* g_sound_jump = NULL;
static Mix_Chunk* g_sound_soundtrack = NULL;
static Mix_Music* g_music = NULL;


//Screen
const int FRAME_PER_SECOND = 40;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 768;
const int SCREEN_BBP = 32;

const int RENDER_DRAW_COLOR = 0xff;

#define BLANK_TILE 0 
#define TILE_SIZE 64 

#define MAX_MAP_X 10 
#define MAX_MAP_Y 100 

struct InputMovement 
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};

struct Map {
	int start_x_; 
	int start_y_; 

	int max_x_; 
	int max_y_; 

	int tile[MAX_MAP_Y][MAX_MAP_X]; 
	const char* file_name; 
};

#endif