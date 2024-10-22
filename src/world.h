#pragma once
#include "sphere.h"
#include <vector>
#include "camera.h"

struct World {
    Camera camera;
    std::vector<Sphere> spheres;

    World(Camera camera, std::vector<Sphere> spheres);
};
