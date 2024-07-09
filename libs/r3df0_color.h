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
    public:
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
        //TODO: COMPLETE PIXEL CLASS 3.1 COLOR UTILITY
    };

    // Easily print a pixel
    inline ostream& operator<<(ostream& out, const pixel_f& p) {
        return out << "("<< p.r << ', ' << p.g << ', ' << p.b << ")";
    }

    // integers
    class pixel_i : public pixel_f{
        int r, g, b;

        // constructors
        pixel_i() : r{0}, g{0}, b{0} {} // default behaviour
        pixel_i(int r, int g, int b) : r{r}, g{g}, b{b} {} // if args are defined

    };
} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_COLOR_H
