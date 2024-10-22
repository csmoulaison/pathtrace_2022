#pragma once
#include "vec3.h"

struct Transform {
    Vec3 origin;
    Vec3 direction;

    void translate(Vec3 translation);
    void rotate(Vec3 rotation);
};
