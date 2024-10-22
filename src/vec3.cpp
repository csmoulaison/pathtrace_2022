#include "vec3.h"

Vec3 random_in_unit_sphere() {
    while(true) {
        Vec3 point = Vec3::random(-1, 1);
        if(point.length_squared() >= 1) continue;
        return point;
    }
}

Vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

bool Vec3::near_zero() const {
    const auto s = 1e-8;
    return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
}

Vec3 Vec3::random() {
    return Vec3(random_double(), random_double(), random_double());
}

Vec3 Vec3::random(double min, double max) {
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

Vec3 operator+(const Vec3& a, const Vec3& b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator-(const Vec3& a, const Vec3& b) {
	return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec3 operator*(const Vec3& a, const Vec3& b) {
    return Vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vec3 operator*(const Vec3& a, const double t) {
	return Vec3(a.x * t, a.y * t, a.z * t);
}

Vec3 operator*(const double t, const Vec3& a) {
	return a * t;
}

Vec3 operator/(const Vec3& a, const double t) {
	return (1 / t) * a;
}
