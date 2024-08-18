//
// Created by trast on 01/07/2024.
//

#ifndef _R3DFROM0_R3DF0_MATH_H
#define _R3DFROM0_R3DF0_MATH_H

// INCLUDES
#include <cmath>
#include <ostream>
#include "r3df0_varsutil.h"

using namespace std;

namespace r3dfrom0 {
    class interval{
    public:
        /**
         * An interval class to define a continuos interval of real values
         */
        // attributes
        float min, max;

        // constructors
        interval() : min(-infinity), max(+infinity) {}
        interval(float m, float M) : min{m}, max{M} {}
        interval(interval _x, interval _y){
            min = _x.min <= _y.min ? _x.min : _y.min;
            max = _x.max >= _y.max ? _x.max : _y.max;
        }

        // methods
        bool size() const{
            return max - min;
        }

        bool includes_or_equals(float x) const{
            return min <= x && x <= max;
        }

        bool includes(float x) const{ // maybe redundant
            return min < x && x < max;
        }

        bool excludes(float x){
            return x < min || x > max;
        }

        float clamp(float x) const{
            if (x < min) return min;
            else if (x > max) return max;
            return x;
        }

        interval expand(float delta){
            float half_delta = delta / 2.0f;
            return interval(min-half_delta, max+half_delta);
        }

        // constants declaration
        static const interval empty;
        static const interval universe;
    };

    // constants definitions
    const interval interval::empty    = interval(+infinity, -infinity);
    const interval interval::universe = interval(-infinity, +infinity);

} // namespace r3dfrom0

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

        float operator[](const int n) const {
            if (n==0) return this->x;
            else if (n == 1) return this->y;
            else return this->z;
        }

        // methods
        float sqr_length() const {
            // each of the vector component is squared
            return x*x + y*y + z*z;
        }

        float length() const {
            // length of a vector which is the sqrt of the square length
            return sqrt(sqr_length());
        }

        bool near_zero() const{
            auto s = 1e-8;
            return (fabs(this->x) < s) && (fabs(this->y) < s) && (fabs(this->z) < s);
        }

        vec3f random(){
            return {this->x = random_float(),
                    this->y = random_float(),
                    this->z = random_float()};
        }

        vec3f random(float min, float max){
            return {this->x = random_float(min, max),
                    this->y = random_float(min, max),
                    this->z = random_float(min, max)};
        }
    }; // vec3f class

    // Integration of Algebraic operators as util function and other
    // Easily print a vector
    inline ostream& operator<< (ostream& out, const vec3f& v) {
        return out << v.x << ' ' << v.y << ' ' << v.z;
    }

    inline vec3f operator+ (const vec3f& u, const vec3f& v) {
        // sums of two vectors
        return {u.x + v.x, u.y + v.y, u.z + v.z};
    }

    inline vec3f operator- (const vec3f& u, const vec3f& v) {
        // subtraction of two vectors
        return {u.x - v.x, u.y - v.y, u.z - v.z};
    }

    inline vec3f operator* (const vec3f& u, const vec3f& v) {
        return {u.x * v.x, u.y * v.y, u.z * v.z};
    }

    inline vec3f operator* (const float t, const vec3f& v) {
        // vector times a real value or "vettore per uno scalare"
        return {t * v.x, t * v.y, t * v.z};
    }

    inline vec3f operator* (const vec3f& v, const float t) {
        // like operator* previous implementation but from the other side
        return t * v;
    }

    inline vec3f operator/ (const vec3f& v, const float t) {
        return (1/t) * v;
    }

    inline float dot(vec3f u, vec3f v) {
        // dot product or "prodotto scalare"
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    inline vec3f cross(vec3f u, vec3f v) {
        // cross product or "prodotto vettoriale"
        return {u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
    }

    inline vec3f unit(const vec3f& v) {
        // unit vector or versor
        return v / v.length();
    }


    inline vec3f shitty_random_in_unit_sphere() {
        while (true) {
            vec3f p;
            p.random(-1,1);
            if (p.sqr_length() < 1)
                return p;
        }
    }


    inline vec3f random_in_unit_sphere(){
//        Using Gaussian distribution for all three coordinates of your point
//        will ensure an uniform distribution on the surface of the sphere.
//        You should proceed as follows:

//        Generate three random numbers x,y,z
//        using Gaussian distribution
//        Multiply each number by 1/sqrt(x2+y2+z2)
//        (a.k.a. Normalise) you should handle what happens if x=y=z=0
//                Multiply each number by the radius of your sphere.
        static mt19937 mt_gen;
        static normal_distribution<float> distribution(0.0, 0.5);
        vec3f rdm_v = {distribution(mt_gen),
                       distribution(mt_gen),
                       distribution(mt_gen)};
        if (rdm_v.near_zero()){
            return random_in_unit_sphere();
        }
        return unit(rdm_v);
    }


    inline vec3f random_in_unit_disk(){
        static mt19937 mt_gen;
        static uniform_real_distribution<float> distribution(0, 1);
        vec3f rdm_v {distribution(mt_gen), distribution(mt_gen), 0};
        if (rdm_v.near_zero()){
            return random_in_unit_disk();
        }
        return unit(rdm_v);
    }

    inline vec3f random_unit_vector(){
        // normalizes vector in sphere, only direction no magnitude
        return unit(random_in_unit_sphere());
    }

    inline vec3f random_on_hemisphere(const vec3f normal){
        auto rdm_unit_v = random_unit_vector();
        if (dot(rdm_unit_v, normal) > 0.0f){
            return rdm_unit_v;
        } else {
            return -rdm_unit_v;
        }
    }

    inline vec3f reflect(const vec3f& v, const vec3f& n){
        // vect_reflect = v + 2b where b is one of the cathetus of the triangle between surface and v
        return v - 2 * dot(v,n) * n;
    }

    inline vec3f refract(const vec3f& unit_vec_R, const vec3f& normal, float etai_over_etat){
        // refract function to simulate water refraction
        // etai_over_etat being the fraction between the two coefficients
        auto cos_theta = fmin(dot(-unit_vec_R, normal), 1.0f);
        auto r_y = etai_over_etat * (cos_theta * normal + unit_vec_R); // r parallel component
        auto r_x = -sqrt(fabs(1.0f - r_y.sqr_length())) * normal; // r perpendicular component
        return r_y + r_x;
    }

    inline float schlick_approx(const float& cos_theta, const float& refraction_index){
        // the Schlick approximation to simulate glass like reflectance
        auto r0 = (1.0f - refraction_index) / (1.0f + refraction_index);
        auto sqr_r0 = r0 * r0;
        return sqr_r0 + (1.0f - sqr_r0) * pow(1 - cos_theta, 5);
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

#endif //_R3DFROM0_R3DF0_MATH_H
