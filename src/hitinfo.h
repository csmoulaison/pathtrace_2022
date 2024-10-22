#pragma once
#include "vec3.h"
#include "material.h"

struct HitInfo {
    bool hit = false;
    Vec3 point;
    Vec3 normal; // Always faces against ray as opposed to always facing out from surface
    double t;
    bool outside_surface;
    Material* material;

    HitInfo() {}
    HitInfo(bool hit) : hit(hit) {}

    // Calculates outside_surface and the normal from outward_normal
    HitInfo(bool hit, Vec3 point, const Vec3& outward_normal, const Ray& ray, double t, Material* material) : hit(hit), point(point), t(t), material(material) {
        outside_surface = dot(ray.direction, outward_normal) < 0;
        normal = outside_surface ? outward_normal : -outward_normal; // Ternary sets normal based on condition
    }
};
