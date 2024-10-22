#include "camera.h"
#include <iostream>

Camera::Camera(Vec3 origin, Vec3 orientation) : origin(origin), orientation(orientation) {
    vertical_fov = 90.0;
    view_up = Vec3(0, 1, 0);
    aspect_ratio = 1;
    transform(Vec3(), Vec3());
}

void Camera::transform(Vec3 translation, Vec3 rotation) {
    // Translate origin
    origin = origin + translation;

    // Rotate orientation
    //
    // https://danceswithcode.net/engineeringnotes/rotations_in_3d/rotations_in_3d_part1.html
    // roll = u = x
    // pitch = v = y
    // yaw = w = z
    //
    /* Rotate orientation around x axis by roll angle (rotation.x) 
    if(rotation.x > 0.001) {
        orientation.x = orientation.x;
        orientation.y = orientation.y * cos(rotation.x) - orientation.z * sin(rotation.x);
        orientation.z = orientation.y * sin(rotation.x) + orientation.z * cos(rotation.x);
    }
    // Rotate orientation around y axis by pitch angle (rotation.y)
    if(rotation.y > 0.001) {
        orientation.x = orientation.x * cos(rotation.y) + orientation.z * sin(rotation.y);
        orientation.y = orientation.y;
        orientation.z = -orientation.x * sin(rotation.y) + orientation.z * cos(rotation.y);
    }
    // Rotate orientation around z axis by yaw angle (rotation.z)
    if(rotation.z > 0.001) {
        orientation.x = rotation.x * cos(rotation.z) - orientation.y * sin(rotation.z);
        orientation.y = orientation.x * sin(rotation.z) + orientation.y * cos(rotation.z);
        orientation.z = orientation.z;
    }
    */
    
    view_target = origin + orientation;

    /*
    std::cout << "Rotation: " << rotation.x << "|" << rotation.y << "|" << rotation.z << "|" << std::endl;
    std::cout << "Orientation: " << orientation.x << "|" << orientation.y << "|" << orientation.z << "|" << std::endl;
    std::cout << "View target: " << view_target.x << "|" << view_target.y << "|" << view_target.z << "|" << std::endl;
    */

    // Calculate viewport
    double theta = degrees_to_radians(vertical_fov);
    double h = tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    Vec3 w = unit_vector(origin - view_target);
    Vec3 u = unit_vector(cross(view_up, w));
    Vec3 v = cross(w, u);

    viewport_horizontal = viewport_width * u;
    viewport_vertical = viewport_height * v;
    viewport_origin = origin - viewport_horizontal / 2 - viewport_vertical / 2 - w;
}

void Camera::translate(Vec3 translation) {
    transform(translation, Vec3());
}

void Camera::rotate(Vec3 rotation) {
    transform(Vec3(), rotation);
}

Ray Camera::get_ray(double s, double t) const {
    return Ray(origin, viewport_origin + s * viewport_horizontal + t * viewport_vertical - origin);
}

double degrees_to_radians(double degrees) {
    return degrees * 0.0174533;
}
