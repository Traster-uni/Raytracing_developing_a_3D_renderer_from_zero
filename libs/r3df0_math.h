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
        float operator[](const int i) const {
            return (&x)[i];
        }

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
    inline bool operator==(const vec3f& a, const vec3f& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    inline bool operator!=(const vec3f& a, const vec3f& b) {
        return a.x != b.x || a.y != b.y || a.z != b.z;
    }

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

    // 4 dimensional vectors known as quaternions
    class vec4f : public vec3f{
    public:
        float x, y, z, w;

        // constructors
        vec4f() : x{0}, y{0}, z{0}, w{0} {}; // default behaviour
        vec4f(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {} // if args are defined

        // operators
        inline float& operator[](int i) {
            return (&x)[i];
        }

        vec4f operator-() const{ // negative
            return {-x, -y, -z, -w};
        };

        vec4f& operator+=(const vec4f v) {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        };

        vec4f& operator*=(const float t) {
            x *= t;
            y *= t;
            z *= t;
            w *= t;
            return *this;
        }

        vec4f& operator/=(const float t) {
            return *this *= (1/t);
        }

        // methods
        float sqr_length() const {
            // each of the vector component is squared
            return x*x + y*y + z*z + w*w;
        }

        float length() const {
            // length of a vector which is the sqrt of the square length
            return sqrt(sqr_length());
        }

        bool near_zero() const{
            auto s = 1e-8;
            return (fabs(this->x) < s) && (fabs(this->y) < s) && (fabs(this->z) < s) && (fabs(this->w) < s);
        }
    }; // vec3i

    inline bool operator==(const vec4f& a, const vec4f& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const vec4f& a, const vec4f& b) {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    inline ostream& operator<< (ostream& out, const vec4f& v) {
        return out << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
    }

    inline vec4f operator+ (const vec4f& u, const vec4f& v) {
        // sums of two vectors
        return {u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w};
    }

    inline vec4f operator- (const vec4f& u, const vec4f& v) {
        // subtraction of two vectors
        return {u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w};
    }

    inline vec4f operator* (const vec4f& u, const vec4f& v) {
        return {u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w};
    }

    inline vec4f operator* (const float t, const vec4f& v) {
        // vector times a real value or "vettore per uno scalare"
        return {t * v.x, t * v.y, t * v.z, t * v.w};
    }

    inline vec4f operator* (const vec4f& v, const float t) {
        // like operator* previous implementation but from the other side
        return t * v;
    }

    inline vec4f operator/ (const vec4f& v, const float t) {
        return (1/t) * v;
    }

    inline float dot(vec4f& u, vec4f& v) {
        // dot product or "prodotto scalare"
        return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
    }

    inline vec4f multiplication(vec4f& a, vec4f& b) {
        // cross product or "prodotto vettoriale"
        return {a.x * b.w + a.w * b.x + a.y * b.w - a.z * b.y,
                a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
                a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
    }

    inline vec4f unit(const vec4f& v) {
        // unit vector or versor
        return v / v.length();
    }


    class matrix44f {
    public:
        vec4f x = {1, 0, 0, 0}; // xx xy xz            | xx
        vec4f y = {0, 1, 0, 0}; // yx yy yz            |    yy
        vec4f z = {0, 0, 1, 0}; // zx zy zz  rotations |       zz
        vec4f t = {0, 0, 0, 1}; // t translations      |          tt scaling

        // constructor
        matrix44f() {};
        matrix44f(vec4f x, vec4f y, vec4f z, vec4f t) : x{x}, y{y}, z{z}, t{t} {};

        // methods
        inline vec4f &operator[](int i) {
            return (&x)[i];
        }

        void operator+=(matrix44f &f) {
            this->x += f.x;
            this->y += f.y;
            this->z += f.z;
            this->t += f.t;
        }

        void operator*=(matrix44f &f) {
            this->x = x * f.x;
            this->y = y * f.y;
            this->z = z * f.z;
            this->t = t * f.t;
        }

        void operator*=(float f) {
            this->x *= f;
            this->y *= f;
            this->z *= f;
            this->t *= f;
        }

        matrix44f diagonal() {
            return {vec4f{x.x, 0, 0, 0},
                    vec4f{0, y.y, 0, 0},
                    vec4f{0, 0, z.z, 0},
                    vec4f{0, 0, 0, t.w}};
        }

        matrix44f transpose(const matrix44f &a) {
            return {
                    {a.x.x, a.y.x, a.z.x, a.t.x},
                    {a.x.y, a.y.y, a.z.y, a.t.y},
                    {a.x.z, a.y.z, a.z.z, a.t.z},
                    {a.x.w, a.y.w, a.z.w, a.t.w}
                    };
        }
    }; // class matrix44f

    // Matrix operations
    inline bool operator==(const matrix44f &a, const matrix44f &b) {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.t == b.t;
    }

    inline bool operator!=(const matrix44f &a, const matrix44f &b) {
        return !(a == b);
    }

    inline matrix44f operator+(const matrix44f &a, const matrix44f &b) {
        return {a.x + b.x, a.y + b.y, a.z + b.z, a.t + b.t};
    }

    inline matrix44f operator*(const matrix44f &a, float b) {
        return {a.x * b, a.y * b, a.z * b, a.t * b};
    }

    inline vec4f operator*(const matrix44f &a, const vec4f &b) {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.t * b.w;
    }

    inline vec4f operator*(const vec4f &a, const matrix44f &b) {
        return {dot(a, b.x), dot(a, b.y), dot(a, b.z), dot(a, b.t)};
    }

    inline matrix44f operator*(const matrix44f &a, const matrix44f &b) {
        return {a * b.x, a * b.y, a * b.z, a * b.t};
    }

    matrix44f inverse(const matrix44f& m) {
        return {{},
                {},
                {},
                {0,0,0,1}};
    }

    matrix44f rotation_matrix(const vec4f& q) {
        return {{1 - (q.y * q.y + q.z * q.z) * 2, (q.x * q.y - q.z * q.w) * 2, (q.x * q.z + q.w * q.y) * 2, 0},
                {(q.x * q.y + q.w * q.z) * 2, 1 - (q.x * q.x + q.z * q.z) * 2, (q.y * q.z - q.w * q.x) * 2, 0},
                {(q.x * q.z - q.w * q.y) * 2, (q.y * q.z + q.w * q.x) * 2, 1 - (q.x * q.x + q.y * q.y) * 2, 0},
                {0, 0, 0, 1}
               }; // colum major
//        return {{1 - (q.y * q.y + q.z * q.z) * 2, (q.x * q.y + q.z * q.w) * 2, (q.x * q.z - q.w * q.y) * 2, 0},
//                {(q.x * q.y - q.w * q.z) * 2, 1 - (q.x * q.x + q.z * q.z) * 2, (q.y * q.z + q.w * q.x) * 2, 0},
//                {(q.x * q.z + q.w * q.y) * 2, (q.y * q.z - q.w * q.x) * 2, 1 - (q.x * q.x + q.y * q.y) * 2, 0},
//                {0, 0, 0, 1}
//               }; // row major
        // TODO: TEST THIS FUNCTION, MAY NEED TO BE INVERTED TO ROW MAJOR
    }

    vec3f local_to_world(vec3f &p_local, matrix44f &m) {
        return {p_local.x * m.x.x + p_local.y * m.y.x + p_local.z * m.z.x + m.t.x,
                p_local.y * m.x.y + p_local.y * m.y.y + p_local.z * m.z.y + m.t.y,
                p_local.z * m.x.z + p_local.y * m.y.z + p_local.z * m.z.z + m.t.z};
    }

//    vec3f world_to_local(vec3f &p_world, matrix44f &m) {
//    }
} // namespace r3dfrom0

#endif //_R3DFROM0_R3DF0_MATH_H
