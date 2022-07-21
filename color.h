#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// Displays the colors in RGB format
void write_color_old(std::ostream &out, color pixel_color) {
    
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    // I.e., we should have color^(1/gamma). This should be the square root in our case.
    // See: https://en.wikipedia.org/wiki/Gamma_correction
    // and https://photo.stackexchange.com/questions/53725/what-is-the-purpose-of-gamma-correction-in-todays-screens-and-how-does-it-relat
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif