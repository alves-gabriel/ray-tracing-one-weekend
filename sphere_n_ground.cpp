#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world) {

    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1)); //Adding color(1,1,1), which is just a vector from vec3, makes the sphere brighter
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);

    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // Number of samples/rays used to color a pixel. This hels in the anti-aliasing.

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));        // Sphere
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));   // Ground. We make the gorund as a very large sphere, with radius 100 and center at (x,y,z)=(0,-100,-1)

    // Camera
    camera cam;

    // Meta data
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {

        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; ++i) {

            color pixel_color(0, 0, 0);

            // Use many samples/rays in a single pixel to color it
            for (int s = 0; s < samples_per_pixel; ++s) {

                // Adds a random number to create an antialiasing effect. Thus, if the pixel is near the edge, we will kinda
                // average out the colors between the two sides. E.g., if we are coloring an edge between a green and a blue surface, if random_double < 0
                // we will have a green contribution, since we will use the ray "on the left", otherwise we will have a blue contribution. 
                // On average, we should get some colors in-between
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world); // Colors each object in world which is hit
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}