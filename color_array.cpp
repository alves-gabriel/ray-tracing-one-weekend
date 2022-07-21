#include <iostream>

/*
Run with

g++ color_array.cpp -o color_array && ./color_array>image.ppm
display image.ppm #In case you have ImageMagick installed
*/

int main(){

    // Image parameters
    const int image_width = 256;
    const int image_height = 256;

    //File format information: see https://en.wikipedia.org/wiki/Netpbm#PPM_example
    std :: cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; j--) {
        for (int i = 0; i < image_width; ++i) {

            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

            // auto is used for type inference here
            // We introduce a gradient were red and green depend on the coordinates
            auto b = double(i) / (image_width-1);
            auto g = double(j) / (image_width-1);
            auto r = 1.; 

            // static_cast is a unary operator used for data conversion
            // see: https://stackoverflow.com/questions/103512/why-use-static-castintx-instead-of-intx
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            // Outputs the RBB values
            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    // This is standard error stream. It is unbuffered and we use it to immediately display a
    // message. It can be used to display e.g. errors or temporary messages.
    std::cerr << "\nDone\n";

    // Since ir = 0 at the top left (and 256 at the top right), we should have the corners 
    // should be colored in the following order:
    // YELLOW   WHITE
    // RED      PINK

    return 0;
}