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

    inline interval operator+(const interval& ival, double displacement) {
        return interval(ival.min + displacement, ival.max + displacement);
    }

    inline interval operator+(double displacement, const interval& ival) {
        return ival + displacement;
    }

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
        const float& operator[](const int i) const{
            return (&x)[i];
        }

        float& operator[](const int i){
            return  (&x)[i];
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

    inline ostream& operator<< (ostream& out, const vec3f v) {
        return out << "(" << v.x << ' ' << v.y << ' ' << v.z << ")";
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
    class vec4f {
    public:
        float x, y, z, w;

        // constructors
        vec4f() : x{0}, y{0}, z{0}, w{0} {}; // default behaviour
        vec4f(float x, float y, float z, float w) : x{x}, y{y}, z{z}, w{w} {} // if args are defined
        vec4f(vec3f v, float s) : x{v.x}, y{v.y}, z{v.z}, w{s} {}

        // operators
        inline float& operator[](int i) {
            return (&x)[i];
        }

        inline const float& operator[](int i) const {
            return (&x)[i];
        }

        inline vec4f operator-() const { // negative
            return {-x, -y, -z, w};
        };

        inline vec4f& operator+=(const vec4f& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        };

        inline vec4f& operator-=(const vec4f& v){
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        inline vec4f& operator*=(const float& t) {
            x *= t;
            y *= t;
            z *= t;
            w *= t;
            return *this;
        }

        vec4f& operator/=(const float& t) {
            return *this *= (1/t);
        }

        // methods
        float sqr_length() const {
            // each of the vector component is squared
            return x*x + y*y + z*z + w*w;
        }

        float length() const {
            // length of a vector or norm or ||q||
            return sqrt(sqr_length());
        }

        bool near_zero() const{
            auto s = 1e-8;
            return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s) && (fabs(w) < s);
        }

        vec3f vector() const {
            return {x, y, z};
        }

        float scalar() const {
            return w;
        }

    }; // vec4f

    inline bool operator==(const vec4f& a, const vec4f& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!=(const vec4f& a, const vec4f& b) {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    inline ostream& operator<< (ostream& out, const vec4f v) {
        return out << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
    }

    inline vec4f operator+ (const vec4f& u, const vec4f& v) {
        // sums of two quaternions
        return {u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w};
    }

    inline vec4f operator- (const vec4f& u, const vec4f& v) {
        // subtraction of two quaternions
        return {u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w};
    }

    inline vec4f operator* (const vec4f& u, const vec4f& v) {
        return {u.scalar() * v.vector() + v.scalar() * u.vector() + cross(u.vector(),v.vector()),
                u.scalar() * v.scalar() - dot(u.vector(), v.vector())};
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

    inline const float dot(const vec4f& u, const vec4f& v) {
        // dot product or "prodotto scalare"
        return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
    }

    inline const vec4f cross(const vec4f& a, const vec4f& b) {
        // cross product or "prodotto vettoriale"
        return {a.x * b.w + a.w * b.x + a.y * b.w - a.z * b.y,
                a.y * b.w + a.w * b.y + a.z * b.x - a.x * b.z,
                a.z * b.w + a.w * b.z + a.x * b.y - a.y * b.x,
                a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
    }

    inline vec4f unit(const vec4f& v) {
        // unit vector or versor or (q / ||q||)
        return v / v.length();
    }


    class matrix44f {
    public:
        vec4f x = {0, 0, 0, 0}; // xx xy xz            | xx
        vec4f y = {0, 0, 0, 0}; // yx yy yz            |    yy
        vec4f z = {0, 0, 0, 0}; // zx zy zz  rotations |       zz
        vec4f t = {0, 0, 0, 0}; // t translations      |          tt scaling

        // constructor
        matrix44f() {};
        matrix44f(float id) { x.x = id; y.y = id; z.z = id; t.w = id; };
        matrix44f(vec4f x, vec4f y, vec4f z, vec4f t) : x{x}, y{y}, z{z}, t{t} {};

        // methods
        inline vec4f& operator[] (int i) {
            return (&x)[i];
        }

        inline const vec4f& operator[](int i) const {
            return (&x)[i];
        }

        inline void operator+=(matrix44f &f) {
            x += f.x;
            y += f.y;
            z += f.z;
            t += f.t;
        }

        inline void operator-=(matrix44f &f) {
            x -= f.x;
            y -= f.y;
            z -= f.z;
            t -= f.t;
        }

        inline void operator*=(matrix44f &f) {
            x = x * f.x;
            y = y * f.y;
            z = z * f.z;
            t = t * f.t;
        }

        inline void operator*=(float f) {
            x *= f;
            y *= f;
            z *= f;
            t *= f;
        }

        inline const float determinant() const {
            // from https://semath.info/src/determinant-four-by-four.html
            return  x.x*y.y*z.z*t.w + x.x*y.z*z.w*t.y + x.x*y.w*z.y*t.z
            - x.x*y.w*z.z*t.y - x.x*y.z*z.y*t.w - x.x*y.y*z.w*t.z
            - x.y*y.x*z.z*t.w - x.z*y.x*z.w*t.y - x.w*y.x*z.y*t.z
            + x.w*y.x*z.z*t.y + x.z*y.x*z.y*t.w + x.y*y.x*z.w*t.z
            + x.y*y.z*z.x*t.z + x.z*y.w*z.x*t.y + x.w*y.y*z.x*t.z
            - x.w*y.z*z.x*t.y - x.z*y.y*z.x*t.w - x.y*y.w*z.x*t.z
            - x.y*y.z*z.w*t.x - x.z*y.w*z.y*t.x - x.w*y.y*z.z*t.x
            + x.w*y.z*z.y*t.x + x.z*y.y*z.w*t.x + x.y*y.w*z.z*t.x;
        }

        matrix44f diagonal() const {
            return {vec4f{x.x, 0, 0, 0},
                    vec4f{0, y.y, 0, 0},
                    vec4f{0, 0, z.z, 0},
                    vec4f{0, 0, 0, t.w}};
        }

        matrix44f transpose() const {
            return {
                    {x.x, y.x, z.x, t.x},
                    {x.y, y.y, z.y, t.y},
                    {x.z, y.z, z.z, t.z},
                    {x.w, y.w, z.w,  t.w}
                    };
        }
    }; // class matrix44f

    // Matrix operations
    inline vec4f operator*(const matrix44f& m, const vec4f& q) {
        return m.x * q.x + m.y * q.y + m.z * q.z + m.t * q.w;
    }
    inline vec4f operator*(const vec4f& q, const matrix44f& m) {
        return {dot(q, m.x), dot(q, m.y), dot(q, m.z), dot(q, m.t)};
    }

    inline matrix44f operator*(const matrix44f& a, const matrix44f& b) {
        return {a * b.x, a * b.y, a * b.z, a * b.t};
    }

    inline ostream& operator<<(ostream& out, matrix44f& m) {
        return out << "{\n[ " << m.x << " ]\n[ " << m.y << " ]\n[ " << m.z << " ]\n[ " << m.t << " ]\n}";
    }

    inline bool operator==(const matrix44f& a, const matrix44f& b) {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.t == b.t;
    }

    inline bool operator!=(const matrix44f& a, const matrix44f& b) {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.t != b.t;
    }

    // https://math.stackexchange.com/questions/152462/inverse-of-transformation-matrix
    // ROTATIONS
    inline matrix44f make_rotation_matrix(const vec4f& q) {
        // https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
        return {
                {q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z, (q.x*q.y - q.w*q.z) *2, (q.w*q.y - q.x*q.z) *2, 0},
                {(q.x*q.y - q.w*q.z) *2, q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z, (q.y*q.z - q.w*q.x) *2, 0},
                {(q.x*q.z - q.w*q.y) *2, (q.w*q.x - q.y*q.z) *2, q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z, 0},
                {0, 0, 0, 1}
        };
    }


    inline matrix44f make_rotation_matrix( const vec3f& axis, const float& angle) {
        auto s = sin(angle);
        auto c = cos(angle);
        auto vv = unit(axis);
        return {{c + (1 - c) * vv.x * vv.x, (1 - c) * vv.x * vv.y + s * vv.z, (1 - c) * vv.x * vv.z - s * vv.y, 0},
                {(1 - c) * vv.x * vv.y - s * vv.z, c + (1 - c) * vv.y * vv.y, (1 - c) * vv.y * vv.z + s * vv.x, 0},
                {(1 - c) * vv.x * vv.z + s * vv.y, (1 - c) * vv.y * vv.z - s * vv.x, c + (1 - c) * vv.z * vv.z, 0},
                {0, 0, 0, 1}};
    }

    bool is_rotation_matrix(const matrix44f& m){
        auto I = matrix44f(1);
        auto det_m = m.determinant();
        cout << det_m << endl;
        auto m1 = m;
        auto m_t = m.transpose();
        cout << m1 << endl;
        cout << m_t << endl;
        if (m * m_t != I && det_m != 1.0f){
            auto a = m * m_t;
            bool b = a != I;
            cout << "m*m_t= " << b << endl;
            bool c = det_m != 1.0f;
            cout << "det_m= " << c << endl;
            return false;
        }
        return true;
    }

    inline matrix44f rotation_inverse(const matrix44f& m) {
            return m.transpose();
    }

    // TRANSLATIONS
    inline matrix44f make_translation_matrix(const vec3f& t) {
        return { {1, 0, 0, 0},
                 {0, 1, 0, 0},
                 {0, 0, 1, 0},
                 {t.x, t.y, t.z, 1}};
    }

    bool is_translation_matrix(const matrix44f& t) {
        return t.t.x + t.t.y + t.t.z + t.t.w > 1;
    }

    inline matrix44f translation_inverse(const matrix44f& t) {
        return {{1,0,0,0},
                {0,1,0,0},
                {0,0,1,0},
                {-t.t.x,-t.t.y,-t.t.z,1}};
    }

    // SCALING
    matrix44f make_scaling_matrix_xyz (const vec3f& s) {
        return {{s.x, 0, 0, 0},
                {0, s.y, 0, 0},
                {0, 0, s.z, 0},
                {0, 0, 0, 1}};
    }

    matrix44f make_scaling_matrix (const float& s) {
        // NOTE: UNIFORM SCALING ON 2D OBJECTS FUCKS WITH Z AXIS AKA IT DOESN'T WORK
        return make_scaling_matrix_xyz(vec3f{s, s, s});
    }

    bool is_scaling_matrix (const matrix44f& s) {
        auto non_diag_sum = 0.0f;
        for (int i=0; i < 4; i++){
            for (int j=0; j < 4; j++){
                if (i != j) {
                    non_diag_sum += s[i][j];
                }
            }
        }
        if (non_diag_sum > 0){
            return false;
        }
        return true;
    }

    matrix44f scaling_inverse(const matrix44f& s) {
        return {{1/s.x.x, 0, 0, 0},
                {0, 1/s.y.y, 0, 0},
                {0, 0, 1/s.z.z, 0},
                {0, 0, 0, 1}};
    }

    matrix44f invert(const matrix44f& H){
        // check what kind of matrix that is and invert it with the appropriate function
        // check if matrix is invertible
        if (H.determinant() == 0) {
            return {}; // returns default Identity matrix
        }
        // https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati
        matrix44f t = {{1,0,0,0},
                       {0,1,0,0},
                       {0,0,1,0},
                       {H.t}};
        matrix44f s = {{H.x.length(), 0, 0, 0},
                       {0, H.y.length(), 0, 0},
                       {0, 0, H.z.length(), 0},
                       {0, 0, 0, 1}};
//        matrix44f r = {{H.x.x / s.x.x, H.x.y / s.x.x, H.x.z / s.x.x, 0},
//                      {H.y.x / s.y.y, H.y.y / s.y.y, H.y.z / s.y.y, 0},
//                      {H.z.x / s.z.z, H.z.y / s.z.z, H.z.z / s.z.z, 0},
//                      {0, 0, 0, 1}};
        matrix44f r = {{H.x.x, H.x.y, H.x.z, 0},
                      {H.y.x, H.y.y , H.y.z , 0},
                      {H.z.x , H.z.y , H.z.z , 0},
                      {0, 0, 0, 1}};
        r = is_rotation_matrix(r) ? r : matrix44f();
        return r.transpose(); // scaling_inverse(s) * * translation_inverse(t);
    }

    // https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-polygon-mesh/introduction.html
    // under NORMALS AND VECTORS chapter
    inline vec3f transform_vectors(const vec3f& v, const matrix44f& m) {
        return {v.x * m.x.x + v.y * m.y.x + v.z * m.z.x + m.t.x,
                v.y * m.x.y + v.y * m.y.y + v.z * m.z.y + m.t.y,
                v.z * m.x.z + v.y * m.y.z + v.z * m.z.z + m.t.z};
                                        // v.x-^
    }

    inline vec3f transform_normals(const vec3f& normal, const matrix44f& m) {
        return transform_vectors(normal, invert(m));
    }

    inline vec3f transform_point(const vec3f& p_local, const matrix44f& m) {
        vec3f p_transform = {p_local.x * m.x.x + p_local.y * m.y.x + p_local.z * m.z.x + m.t.x,
                             p_local.y * m.x.y + p_local.y * m.y.y + p_local.z * m.z.y + m.t.y,
                             p_local.z * m.x.z + p_local.y * m.y.z + p_local.z * m.z.z + m.t.z};
                                                                   // p_local.x-^
        auto w = p_local.x * m.x.w + p_local.y * m.y.w + p_local.z * m.z.w + m.t.w;
        if (w != 1) {
            p_transform.x /= w;
            p_transform.y /= w;
            p_transform.z /= w;
            return p_transform;
        }
        return p_transform;
    }

    // INTERPOLATION
    // https://en.wikipedia.org/wiki/Perlin_noise
    inline float smoothstep_interpolation(const float& w) {
        if (w < 0.0f) return 0.0f;
        else if (w > 1.0f) return 1.0f;
        return w * w * (3 - 2 * w);
    }

    inline float smootherstep_interpolation(const float& w) {
        if (w < 0.0f) return 0.0f;
        else if (w > 1.0f) return 1.0f;
        else return w*w*w * (w * (w * 6.f - 15.f) + 10.f);
    }

    // LERP
    inline float lerp_float(const float& value, const float& start, const float& end) {
        float a = 0.5f * (value + 1.0f);
        return (1 - a) * start + a * end;
    }

    inline int lerp_int(const int& value, const int& start, const int& end) {
        float a = 0.5f * (value + 1.0f);
        return (1 - a) * start + a * end;
    }

    inline vec3f lerp_vec3f(const vec3f& value, const vec3f& start, const vec3f& end) {
        auto a = 0.5f * (value.length() + 1.0f);
        return (1 - a) * start + a * end;
    }

} // namespace r3dfrom0

#endif //_R3DFROM0_R3DF0_MATH_H
