#pragma once
#include "vec3.h"
#include "ray.h"
#include "hitinfo.h"
#include "material.h"

struct Sphere {
    Vec3 center = Vec3(0, 0, 0);
    double radius = 1;
    Material* material;

    Sphere() {}
    Sphere(Vec3 center, double radius, Material* material) : center(center), radius(radius), material(material) {}

    HitInfo hit(const Ray& ray, double t_min, double t_max) const;
};
