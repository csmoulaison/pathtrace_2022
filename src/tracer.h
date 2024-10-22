#pragma once
#include "vec3.h"
#include "ray.h"
#include "hitinfo.h"
#include "world.h"
#include "sphere.h"
#include "random.h"
#include "platform.h"
#include "color.h"

#include <SDL2/SDL.h>
#include <cmath>
#include <limits>

#define SAMPLES_PER_PIXEL 2
#define MAX_DEPTH 4

struct Pathtracer {
    void render(Platform& platform, const World& world);
    //void draw_frame(SDL_Renderer* renderer);
};

Vec3 render_pixel(const World& world, Platform& platform, Pathtracer& tracer, int x, int y);
Vec3 color_from_ray(const Ray& ray, const World& world, int depth);
Vec3 lambertian_reflection(HitInfo& hit_info);
Color vec3_to_color(Vec3 &color);

