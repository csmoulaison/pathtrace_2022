#include "sphere.h"

// Returns info about whether the sphere was hit,
// the point in space the hit occured,
// the surface normal of the hit on the sphere,
// and the "time" t along the ray cast that represents the hit point
HitInfo Sphere::hit(const Ray& ray, double t_min, double t_max) const {
	Vec3 offset_center = ray.origin - center; // Make all this relative to the camera and sphere center
	double a = ray.direction.length_squared(); // A vector dotted with itself is equal to the squared length of the vector. Hence this from dot(r.dir, r.dir)
	double half_b = dot(offset_center, ray.direction); // Halving b in the case of b = 2h (the factor of 2 in 2*dot(oc,r.dir)) simplifies quad. form to (-h +- sqrt(h^2) - ac) / a
	double c = offset_center.length_squared() - radius * radius; // Using the same logic as the new calculation for a
                                                                    //
	double discriminant = half_b * half_b - a * c; // Simplified discriminant calculation from halving b
    if(discriminant < 0) { // if we didn't hit the sphere
        return HitInfo(false);
    }

    // Simply calculating the sqrt of the discriminant in advance
    double sqrt_discriminant = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range
    // I believe root is equal to t
    double root = (-half_b - sqrt_discriminant) / a;
    if(root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
        if(root < t_min || t_max < root) { // If outside the range 
            return HitInfo(false);
        }
    }

    // Construct hit info
    double t = root;
    Vec3 point = ray.at(t);
    Vec3 outward_normal = (point - center) / radius;
    return HitInfo(true, point, outward_normal, ray, t, material);
}
