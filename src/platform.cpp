#include "platform.h"
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <stdio.h>

struct Platform init_platform() {
	struct Platform platform;
	SDL_Init(SDL_INIT_EVERYTHING);
	platform.win_w = 128;
	platform.win_h = 128; 
	platform.window = SDL_CreateWindow("cnake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 2560, 1440, SDL_WINDOW_FULLSCREEN);
	platform.renderer = SDL_CreateRenderer(platform.window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_SetWindowMinimumSize(platform.window, platform.win_w, platform.win_h);
	SDL_RenderSetLogicalSize(platform.renderer, platform.win_w, platform.win_h);
	//SDL_RenderSetIntegerScale(platform.renderer, SDL_TRUE);
	platform.texture = SDL_CreateTexture(platform.renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, platform.win_w, platform.win_h);
	platform.pixels = (uint32_t*)malloc(platform.win_w * platform.win_h * 4);
	return platform;
}

struct Input input_from_platform() {
	struct Input input;
	input.w = false;
	input.a = false;
	input.s = false;
	input.d = false;
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) {
		if(event.type == SDL_QUIT) {
			input.quit = true;
		}
		
		if(event.type == SDL_KEYDOWN) {
			switch(event.key.keysym.sym) {
				case SDLK_w:
					input.w = true;
					break;
				case SDLK_a:
					input.a = true;
					break;
				case SDLK_s:
					input.s = true;
					break;
				case SDLK_d:
					input.d = true;
					break;
				case SDLK_ESCAPE:
					input.quit = true;
					break;
				default:
					break;
			}
		}
	}

	return input;
}

void draw_pixel(Platform *platform, Color *color, int x, int y) {
	int index = x + y * platform->win_w;
	buffer_pixel(platform, color, index);
}

void fill_texture(Platform *platform, Color *color) {
	int pixel_count = platform->win_w * platform->win_h;
	for(int i = 0; i < pixel_count; ++i) {
		buffer_pixel(platform, color, i);
	}
}

void buffer_pixel(Platform *platform, Color *color, int index) {
	platform->pixels[index] = color->value;
}

void render_platform(Platform *platform) {
	int pitch = platform->win_w * 4;
	SDL_LockTexture(platform->texture, NULL, (void**) &platform->pixels, &pitch);
	SDL_SetRenderDrawColor(platform->renderer, 0, 10, 0, 255);
	SDL_RenderClear(platform->renderer);
	SDL_UnlockTexture(platform->texture);
	SDL_RenderCopy(platform->renderer, platform->texture, NULL, NULL);
	SDL_RenderPresent(platform->renderer);
}
