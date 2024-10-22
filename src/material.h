#pragma once
#include "vec3.h"
#include <math.h>

// Can be used to describe any general material
// Right now its being used by pointer in these classes, but should be table optimally.
struct Material {
    Vec3 albedo;
    double metallicity = 0.0;
    double fuzziness = 0.0;
    double lightness = 0.0;

    Material() {}
    Material(Vec3 albedo) : albedo(albedo) {}
    Material(Vec3 albedo, double metallicity, double fuzziness) : albedo(albedo), metallicity(metallicity), fuzziness(fuzziness) {}
    Material(Vec3 albedo, double metallicity, double fuzziness, double lightness) : albedo(albedo), metallicity(metallicity), fuzziness(fuzziness) , lightness(lightness) {}
};
