//
// Created by tommasomarialopedote on 09/07/24.
//

#ifndef R3DFROM0_R3DF0_COLOR_H
#define R3DFROM0_R3DF0_COLOR_H

// INCLUDES
#include <cmath>

using namespace std;

namespace r3dfrom0{
// pixel class
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

    inline pixel_f operator+ (const pixel_f& u, const pixel_f& v) {
        return {u.r + v.r, u.g + v.g, u.b + v.b};
    };

    inline pixel_f operator* (const pixel_f& u, const pixel_f& v) {
        return {u.r * v.r, u.g * v.g, u.b * v.b};
    };

    // vector times a real value or "vettore per uno scalare"
    inline pixel_f operator* (const float t, const pixel_f& v) {
        return {t * v.r, t * v.g, t * v.b};
    };

    // as above but from the other side
    inline pixel_f operator* (const pixel_f& v, const float t) {
        return t * v;
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
        static const interval intensity(0, 0.999);
        return {int(256 * intensity.clamp(p.r)),
                int(256 * intensity.clamp(p.g)),
                int(256 * intensity.clamp(p.b))};
    } // convert_pixel_i

    pixel_f lerp_color(const float& _a, const pixel_f& start_color, const pixel_f& end_color){
        /**
         * Blends the float value _a from the start color, to the end color;
         */
        float a = 0.5 * (_a + 1.0);
        return (1 - a) * start_color + a * end_color;
    }

    pixel_f color_map(const vec3f& unit_vec_length){
        return 0.5 * pixel_f{unit_vec_length.x + 1,
                             unit_vec_length.y + 1,
                             unit_vec_length.z + 1};
    }

} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_COLOR_H
