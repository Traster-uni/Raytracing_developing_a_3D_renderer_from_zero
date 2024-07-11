//
// Created by tommasomarialopedote on 09/07/24.
//

#ifndef R3DFROM0_R3DF0_COLOR_H
#define R3DFROM0_R3DF0_COLOR_H

// INCLUDES
#include <cmath>

using namespace std;
namespace r3dfrom0{
    // float
    class pixel_f{
    public: // attributes
        float r, g, b;

        // constructors
        pixel_f() : r{0}, g{0}, b{0} {} // default behaviour
        pixel_f(float r, float g, float b) : r{r}, g{g}, b{b} {} // if args are defined

        // custom operators
        pixel_f operator-() const{ // negative
            return {-r, -g, -b};
        };

        pixel_f& operator+=(const pixel_f v) {
            r += v.r;
            g += v.g;
            b += v.b;
            return *this;
        };

        pixel_f& operator*=(const float t) {
            r *= t;
            g *= t;
            b *= t;
            return *this;
        }

        pixel_f& operator/=(const float t) {
            return *this *= (1/t);
        }
    }; // pixel_f

    // Easily print a pixel
    inline ostream& operator<<(ostream& out, const pixel_f& p) {
        return out << p.r << ' ' << p.g << ' ' << p.b;
    }

    // integers
    class pixel_i : public pixel_f{
    public:
        int r, g, b;

        // constructors
        pixel_i() : r{0}, g{0}, b{0} {} // default behaviour
        pixel_i(int r, int g, int b) : r{r}, g{g}, b{b} {} // if args are defined

    }; // pixel_i

    inline ostream& operator<<(ostream& out, const pixel_i& p) {
        return out << p.r << ' ' << p.g << ' ' << p.b;
    }

    pixel_i convert_pixel_i(pixel_f p){
        /**
         * Converts a pixel in floating point precision into a pixel with
         * integer precision between 0 and 256.
         * Note: By converting from float to integer, the operation is not reversible.
         */
        return {int(255.999 * p.r), int(255.999 * p.g), int(255.999 * p.b)};
    } // convert_pixel_i

} // namespace r3dfrom0

namespace r3dfrom0 {
    auto
}

namespace r3dfrom0 {
    pixel_f linear_interpolation() {
    }
}
#endif //R3DFROM0_R3DF0_COLOR_H
