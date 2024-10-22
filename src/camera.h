// Okay, hi again. It's the GameOverGreggy show back from the dead. Let's eat some oreos.
// This lower left corner, horizontal, and vertical business is the actual world space position of the camera plane, I think.
// Therefore, we need to make sure our new system works with get ray. It will just be a matter of recreating this idea of the camera plane ourselves.
// Try to offload as much calculation into not being per pixel or sample as possible.
//
// We need a camera with a position and a look direction. Eventually quaternions might be good but ya.
// The idea is to create a Transform struct that gets applied to all objects.
//
// Hello, some notes:
// This way of dealing with the camera and the tracing and shit is obviously fucked.
// We are going to need a way to set up the camera in a sane manner, the way one might expect.
//
// To this end, we will finally get a little serious here and make a Transform struct to be applied to both cameras and objects like spheres.
//
// If it ends up being more performant to precompute some sort of values in camera so that the tracer doesn't have to every ray or pixel or whatever, just do that in the tracer. The camera shouldn't know about any of this.
//
// This get_ray function is a bit mysterious at the moment. It takes in a 2d screen space coordinate (pixels or scaled value?) and returns a ray fired forward from that screen position, at least presumably. Therefore, it should really be renamed, forward_ray_from_screen_point or something like that. Try to obviously figure out a good name for it based on whatever it actually does.
//
// Get table.h up in here and figure that out. Back to main.h for more notes on all that

#pragma once
#include <math.h>

#include "vec3.h"
#include "ray.h"
#include "transform.h"

struct Camera {
    // Member variables
    Vec3 origin;
    Vec3 orientation;
    Vec3 view_up;
    double vertical_fov; // vertical
    double aspect_ratio;

    Vec3 view_target;
    Vec3 viewport_origin;
    Vec3 viewport_horizontal;
    Vec3 viewport_vertical;

    // Constructor
    Camera(Vec3 origin, Vec3 orientation);

    // Methods
    void transform(Vec3 translation, Vec3 rotation);
    void translate(Vec3 translation);
    void rotate(Vec3 rotation);
    Ray get_ray(double u, double v) const;
};

// Converts degrees into radians
double degrees_to_radians(double degrees);
