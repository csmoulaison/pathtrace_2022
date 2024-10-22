#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include "color.h"

struct Input {
	bool w;
	bool a;
	bool s;
	bool d;
	bool quit;
};

struct Platform {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int win_w;
	int win_h;
	struct Input input;
	uint32_t *pixels;
};

struct Platform init_platform();
struct Input input_from_platform();
void draw_pixel(Platform *platform, Color *color, int x, int y);
void fill_texture(Platform *platform, Color *color);
void buffer_pixel(Platform *platform, Color *color, int index);
void render_platform(Platform *platform);
