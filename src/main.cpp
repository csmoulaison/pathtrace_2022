// Going to need to refactor all of this to make sense finally.
// This will be good to require me to understand the math better.
// Look at camera.h for more notes.
//
// This refactor will include more than just getting the camera refactored, it should also entail a major codebase refactor.
//
// So then, what should the refactor entail
// First, a Transform struct which contains the following:
//  - v3 position
//  - v3 forward // "look direction"
//  void translate(v3)
//  void rotate(v3)
//
// Second, table.h back in action to store all of our transforms. Camera has a transform and Sphere has a transform. 
//
// As a note, also make materials in a table too. Every list of things really.
//
// Third, refactor the tracer code to split it up into more easy to understand chunks. It's obvious things are a little out of hand there. The first obvious thing to do is to make the checking for an intersection one function call, that way especially when there are more shapes all those separate for loops won't clutter the main function.
//
// Fourth, create an abstraction layer around the hardware apis in preparation for Vulkan. Also, use your increasing knowledge of Vulkan to figure out what kind of an api is most sensible. For instance, we know we might be pushing the computation of ray hits off to the gpu. How might this affect the tracing code and the hardware api? 
//
// Don't get too funky about this, though. Remember, if different usages of hardware incentivize different ways of structuring the code, then that should not be made generic for cleanliness if you lose that level of control you need for optimization, at least as far as rendering goes.

#include <iostream>
#include <SDL2/SDL.h>
#include "tracer.h"
#include "world.h"
#include "random.h"
#include "platform.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

int main() {
    Platform platform = init_platform();

    // Materials
    Material diff_red(Vec3(0.8, 0.1, 0.1), 0.9, 0.3);
    Material metallic(Vec3(0.3, 0.95, 0.95), 0.95, 0.01);
    Material emissive(Vec3(0.8, 0.8, 0.2), 0.9, 0.5, 0.9);
    Material ground(Vec3(0.2, 0.9, 0.2), 0.5, 0.1);

    // Doesn't depend on order of instantiation, thank god. Referring to the depth order glitch
    World world(
        Camera(Vec3(0, 0, 1), Vec3(0.2361, 0.3265, 0.9214)),
        {
            Sphere(Vec3(0, 0, -1), 0.5, &emissive),
            Sphere(Vec3(2, 0, -1), 0.5, &metallic),
            Sphere(Vec3(0.5, 0, -2), 0.2, &diff_red),
            Sphere(Vec3(-0.5, 0, -2), 0.2, &diff_red),
            Sphere(Vec3(-1.5, 0, -2), 0.2, &diff_red),
            Sphere(Vec3(-2.5, 0, -2), 0.2, &diff_red),
            Sphere(Vec3(0, 1000.5, -1), 1000, &ground)
        }
    );

    Pathtracer tracer;
    //tracer.render(world);

    // Debug movement vars
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    const double x_speed = 0.3;
    Vec3 cam_velocity;
    SDL_SetRelativeMouseMode(SDL_TRUE);

    Vec3 sphere_direction;
    const double sphere_speed = 0.3;

	bool quit = false;
	while(!quit) {
		SDL_Event sdl_event;
		while(SDL_PollEvent(&sdl_event) != 0) {
			if(sdl_event.type == SDL_QUIT) {
				quit = true;
			}

            if(sdl_event.type == SDL_MOUSEMOTION) {
                const double rotation_speed = 0.007154;
                world.camera.rotate(
                    Vec3(
                        0,
                        sdl_event.motion.yrel * rotation_speed, 
                        sdl_event.motion.xrel * rotation_speed
                    )
                );
            }

            if(sdl_event.type == SDL_KEYDOWN) {
                switch(sdl_event.key.keysym.sym) {
                    case SDLK_ESCAPE :
                        quit = true;
                        std::cout << "SDL_ESCAPE fired!" << quit << std::endl;
                        break;
                    case SDLK_LEFT :
                        left = true;
                        break;
                    case SDLK_RIGHT :
                        right = true;
                        break;
                    case SDLK_UP :
                        up = true;
                        break;
                    case SDLK_DOWN :
                        down = true;
                        break;
                    case SDLK_a :
                        sphere_direction.x = -sphere_speed;
                        break;
                    case SDLK_d :
                        sphere_direction.x = sphere_speed;
                        break;
                    case SDLK_w :
                        sphere_direction.z = -sphere_speed;
                        break;
                    case SDLK_s :
                        sphere_direction.z = sphere_speed;
                        break;
                    default :
                        break;
                }
            }

            if(sdl_event.type == SDL_KEYUP) {
                switch(sdl_event.key.keysym.sym) {
                    case SDLK_LEFT :
                        left = false;
                        break;
                    case SDLK_RIGHT :
                        right = false;
                        break;
                    case SDLK_UP :
                        up = false;
                        break;
                    case SDLK_DOWN :
                        down = false;
                        break;
                    case SDLK_a :
                        sphere_direction.x = 0;
                        break;
                    case SDLK_d :
                        sphere_direction.x = 0;
                        break;
                    case SDLK_w :
                        sphere_direction.z = 0;
                        break;
                    case SDLK_s :
                        sphere_direction.z = 0;
                        break;
                    default :
                        break;
                }
            }
	    }

        cam_velocity.x = 0;
        cam_velocity.z = 0;
        if(left) cam_velocity.x -= x_speed;
        if(right) cam_velocity.x += x_speed;
        if(up) cam_velocity.z -= x_speed;
        if(down) cam_velocity.z += x_speed;
        world.camera.translate(cam_velocity);
        world.spheres[0].center = world.spheres[0].center + sphere_direction * sphere_speed;

        // Render (with a timer)
        Uint32 render_start_time = SDL_GetTicks();
        tracer.render(platform, world);
        Uint32 render_time = SDL_GetTicks() - render_start_time;
        //std::cout << "Render took " << render_time << "ms" << std::endl;
        render_platform(&platform);
	}

	SDL_DestroyWindow(platform.window);
	SDL_Quit();
}
