//
// Created by trast on 01/07/2024.
//

#ifndef _R3DFROM0_MATH_H
#define _R3DFROM0_MATH_H

// INCLUDES
#include <cmath>
#include <ostream>
#include "r3df0_varsutil.h"

using namespace std;
namespace r3dfrom0 { // vectors and vector related functions
    // float
    class vec3f{
    public: // attributes
        float x, y, z;

        // constructors
        vec3f() : x{0}, y{0}, z{0} {} // default behaviour
        vec3f(float x, float y, float z) : x{x}, y{y}, z{z} {} // if args are defined

        // custom operators
        vec3f operator-() const{ // negative
            return {-x, -y, -z};
        };

        vec3f& operator+=(const vec3f v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        };

        vec3f& operator*=(const float t) {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        vec3f& operator/=(const float t) {
            return *this *= (1/t);
        }

        // methods
        float sqr_length() const {
            return x*x + y*y + z*z;
        }

        float length() const { // length of a vector
            return sqr_length();
        }

        float normalize() const {
            return sqrt(sqr_length());
        }
    }; // vec3f

    // Integration of Algebraic operators as util function and other
    // Easily print a vector
    inline ostream& operator<< (ostream& out, const vec3f& v) {
        return out << v.x << ' ' << v.y << ' ' << v.z;
    }

    // sums of two vectors
    inline vec3f operator+ (const vec3f& u, const vec3f& v) {
        return {u.x + v.x, u.y + v.y, u.z + v.z};
    }

    // subtraction of two vectors
    inline vec3f operator- (const vec3f& u, const vec3f& v) {
        return {u.x - v.x, u.y - v.y, u.z - v.z};
    }

    inline vec3f operator* (const vec3f& u, const vec3f& v) {
        return {u.x * v.x, u.y * v.y, u.z * v.z};
    }

    // vector times a real value or "vettore per uno scalare"
    inline vec3f operator* (const float t, const vec3f& v) {
        return {t * v.x, t * v.y, t * v.z};
    }

    // as above but from the other side
    inline vec3f operator* (const vec3f& v, const float t) {
        return t * v;
    }

    inline vec3f operator/ (const vec3f& v, const float t) {
        return (1/t) * v;
    }

    // dot product or "prodotto scalare"
    inline float dot(vec3f u, vec3f v) {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    // cross product or "prodotto vettoriale"
    inline vec3f cross(vec3f u, vec3f v) {
        return {u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
    }

    // unit vector or versor
    inline vec3f unit(const vec3f& v) {
        return v / v.length();
    }

    // integer
    class vec3i : public vec3f{
    public:
        int x, y, z;

        // constructors
        vec3i() : x{0}, y{0}, z{0} {}; // default behaviour
        vec3i(int x, int y, int z) : x{x}, y{y}, z{z} {} // if args are defined

    }; // vec3i

} // namespace r3dfrom0

namespace r3dfrom0 {
    class interval{
    public:
        /**
         * An interval class to define a continuos interval of real values
         */
        // attributes
        double min, max;

        // constructors
        interval() : min(-infinity), max(+infinity) {}
        interval(double m, double M) : min{m}, max{M} {}

        // methods
        bool size() const{
            return max - min;
        }

        bool includes(double x) const{
            return min <= x && x <= max;
        }

        bool excludes(double x) const{ // maybe redundant
            return x < min || x > max;
        }

        // constants
        static const interval empty;
        static const interval universe;
    };

    // constants definitions
    const interval interval::empty    = interval(+infinity, -infinity);
    const interval interval::universe = interval(-infinity, +infinity);

} // namespace r3dfrom0

#endif //_R3DFROM0_MATH_H
