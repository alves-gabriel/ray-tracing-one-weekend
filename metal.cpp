#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {

        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))       // Checks whether the ray is scattered, for the given object and material
            return attenuation * ray_color(scattered, world, depth-1);

        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);

    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // Number of samples/rays used to color a pixel. This hels in the anti-aliasing.
    const int max_depth = 50;
    
    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0)); // Diffusive
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);                  // Refraction index is 1.5, same as glass
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), .5);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));  // Ground
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));  // Middle Sphere
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));    // Left Sphere
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));   // Right sphere
    world.add(make_shared<sphere>(point3(-.4 ,    -.3,  0.0), -0.25, material_left));    // A hollow glass sphere. Negative radius leaves geometry unchanged but substitutes the outer material
                                                                                         // by the inner one.

    // Camera
    point3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 2.0;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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
                pixel_color += ray_color(r, world, max_depth); // Colors each object in world which is hit
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}