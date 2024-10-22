#include <thread>
#include <vector>
#include <iostream>

#include "cm_math.h"
#include "tracer.h"
#include "random.h"

void Pathtracer::render(Platform& platform, const World& world) { 	
	// Render image
	for(int y = platform.win_h - 1; y >= 0; --y) {
		for(int x = 0; x < platform.win_w; ++x) {
            Vec3 color = render_pixel(world, platform, *this, x, y);
            Color pixel_color = vec3_to_color(color);
            draw_pixel(&platform, &pixel_color, x, y);
        }
    }
}

Color vec3_to_color(Vec3 &color) {
    Color pixel_color;
    pixel_color.channels.r = color.x;
    pixel_color.channels.g = color.y;
    pixel_color.channels.b = color.z;
    pixel_color.channels.a = 255;
    return pixel_color;
}

Vec3 render_pixel(const World& world, Platform& platform, Pathtracer& tracer, int x, int y) {
    Vec3 pixel_color(0, 0, 0);
    for(int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
        double u = (x + random_double()) / (platform.win_w - 1);
        double v = (y + random_double()) / (platform.win_h - 1);
        Ray ray = world.camera.get_ray(u, v);
        pixel_color = pixel_color + color_from_ray(ray, world, MAX_DEPTH) * 254.0;
    }
    double luminance_scalar = 1.0 / SAMPLES_PER_PIXEL;
    pixel_color.x = (luminance_scalar * pixel_color.x);
    pixel_color.y = (luminance_scalar * pixel_color.y);
    pixel_color.z = (luminance_scalar * pixel_color.z);
    return pixel_color;
}

// Even more hacky approximation of a diffuse material. Probably not even be as performant.
// Here for learning purposes
Vec3 easy_reflection(HitInfo& hit_info) {
    Vec3 random_in_hemisphere = random_in_unit_sphere();
    if(dot(random_in_hemisphere, hit_info.normal) <= 0.0) {
        random_in_hemisphere = -random_in_hemisphere;
    }
    return (hit_info.point + random_in_hemisphere) - hit_info.point;
}

// Simpler approximation of lambertian reflection. Should be ever so slightly cheaper.
Vec3 lambertian_approximation(HitInfo& hit_info) {
    return (hit_info.point + hit_info.normal + random_in_unit_sphere()) - hit_info.point;
}

// Most accurate diffuse material reflection
Vec3 lambertian_reflection(HitInfo& hit_info) {
    return (hit_info.point + hit_info.normal + random_unit_vector()) - hit_info.point;
}

Vec3 color_from_ray(const Ray& ray, const World& world, int depth) {
    if(depth <= 0) {
        return Vec3(0, 0, 0);
    }

    double t_max = std::numeric_limits<double>::infinity();
    HitInfo hit_info(false); // Stores the closest hit

    // For different shapes just need more for loops, continuing to populate hit info based on proximity
    for(const Sphere& sphere : world.spheres) {
        if(hit_info.hit) {
            t_max = hit_info.t;
        }
        HitInfo temp_info = sphere.hit(ray, 0.001, t_max);
        if(temp_info.hit) {
            hit_info = temp_info;
        }
    }

    // If we hit a surface, shoot another ray from a random direction on a tangent unit sphere
    if(hit_info.hit) {
        // If we are a light, return our albedo
        if(hit_info.material->lightness > random_double()) {
            return hit_info.material->albedo;
        }

        bool is_bouncing = true;
        Vec3 bounce_direction;
        Ray bounce_ray;
        // If we are randomly scattering (diffuse case)
        if(random_double() >= hit_info.material->metallicity) {
            bounce_direction = hit_info.normal + random_unit_vector();
            if(bounce_direction.near_zero()) {
                bounce_direction = hit_info.normal;
            }
            bounce_ray = Ray(hit_info.point, bounce_direction);
        }
        // If we are reflecting (metallic case)
        else {
            Vec3 v = unit_vector(ray.direction);
            Vec3 n = hit_info.normal;
            bounce_direction = v - 2.0 * dot(v, n) * n;
            bounce_ray = Ray(hit_info.point, bounce_direction + hit_info.material->fuzziness * random_in_unit_sphere());
            if(dot(bounce_ray.direction, hit_info.normal) <= 0) {
                is_bouncing = false;
            }
        }
        
        Vec3 attenuation = hit_info.material->albedo;

        // If scattering light. 
        if(is_bouncing) {
            return attenuation * color_from_ray(bounce_ray, world, depth - 1);
        }

        // If absorbed light
        return Vec3(0, 0, 0);
        // return 0.5 * color_from_ray(Ray(hit_info.point, lambertian_reflection(hit_info)), world, current_depth - 1);
    }

    // If we didn't hit a surface, return the sky gradient
    Vec3 unit_direction = unit_vector(ray.direction);
    double t = 0.5 * (unit_direction.y + 1.0);
    // WHITE */ return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(1.0, 1.0, 1.0);
    /* RED/BLUE */ return (1.0 - t) * Vec3(0.8, 0.2, 0.2) + t * Vec3(0.0, 0.0, 0.9);
    // BLACK/WHITE */ return (1.0 - t) * Vec3(0.9, 0.9, 0.9) + t * Vec3(0.1, 0.1, 0.1);
    // SLIGHT RED */ return (1.0 - t) * Vec3(0.9, 0.7, 0.7) + t * Vec3(0.2, 0.1, 0.1);
    // DARK SLIGHT RED */ return (1.0 - t) * Vec3(0.2, 0.1, 0.1) + t * Vec3(0.1, 0.1, 0.1);
    // SKY BLUE */ return (1.0 - t) * Vec3(0.5, 0.3, 0.9) + t * Vec3(0.9, 0.9, 0.9);
}

/*
void Pathtracer::draw_frame(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    for(int y = 0; y < IMAGE_HEIGHT; ++y) {
        for(int x = 0; x < IMAGE_WIDTH; ++x) {
            Vec3 pixel_color = pixel_colors[x + y * IMAGE_WIDTH];
            SDL_SetRenderDrawColor(renderer, (int)pixel_color.x, (int)pixel_color.y, (int)pixel_color.z, 255);
			SDL_Rect rect;
			rect.x = x * PIXEL_SCALAR;
			rect.y = y * PIXEL_SCALAR;
			rect.w = PIXEL_SCALAR;
			rect.h = PIXEL_SCALAR;
			SDL_RenderFillRect(renderer, &rect);
		}
	}
	SDL_RenderPresent(renderer);
}
*/
