#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// Solution to the quadratic equation t^2 b.b + 2t b.(A-C) + (A-C).(A-C) - r^2 = 0
// for the intersection between a sphere and the ray. Read "." as the dot product.
// In this function we do not return the interaction point itself. We just decide wheter the sphere is hit or not
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;              // A - C
    auto a = dot(r.direction(), r.direction()); // b.b
    auto b = 2.0 * dot(oc, r.direction());      // 2(A-C).b
    auto c = dot(oc, oc) - radius*radius;       // (A - C).(A- C)- r^2
    auto discriminant = b*b - 4*a*c;            
    return (discriminant > 0);                  // If Discriminant > 0, there is an intersection (non-tangent, here discriminant = 0). 
}

color ray_color(const ray& r) {

    // Checks wheter the sphere is hit by the ray. We place the sphere at C = (0, 0, -1), with radius = 0.5
    if (hit_sphere(point3(0,0,-1), 0.5, r))
        return color(1, 0, 0); // No shading, returns plain red color

    vec3 unit_direction = unit_vector(r.direction()); // Unit vector of the ray direction
    auto t = 0.5*(unit_direction.y() + 1.0);          // Defines a vertical gradient. The parameter t depends on the y component of the direction of the ray. 
                                                      // Note that -1 < y < 1. Thus, 0 < t < 1

    // Convex combination of start and end values, parametrized by 0 < t < 1. This is a linear interpolation
    // (1 - t)*White + t*Royal Blue
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.25, 0.41, 0.88);
}

int main() {

    // Image settings
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;                           // Projection plane/camera size - arbitrary
    auto viewport_width = aspect_ratio * viewport_height; // The camera should follow the same aspect ratio as the image
    auto focal_length = 1.0;                              // Distance between the projection plane and the origin (projection point in our case)

    auto origin = point3(0, 0, 0);            
    auto horizontal = vec3(viewport_width, 0, 0); 
    auto vertical = vec3(0, viewport_height, 0);  
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length); // Lower left corner of the camera. Displaces by focal length and half its dimensions

    // Meta data
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Calculates the intersection betweent the ray and the projection plane for each point the plane.
    // The vector u transverses the plane horizontally, and v vertically. We start on the lower-left corner.
    // At each step the point is given by u*(Plane width) + v*(Plane height). We subtract this from the origin to find the intersection. 
    // The insersection point between the ray and the plane define the color of the pixel at that point.
    for (int j = image_height-1; j >= 0; --j) {

        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {

            auto u = double(i) / (image_width-1);                                   // Sweeps the projection plane horizontally
            auto v = double(j) / (image_height-1);                                  // Sweeps the projection plane vertically
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);  // The ray, centered at the origin, intersects the projection plane. The points u and v define the intersection point.
            color pixel_color = ray_color(r);                                       // Colors the ray depending on the intersection point r (given by a vector), using the linear interpolation defined above
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}