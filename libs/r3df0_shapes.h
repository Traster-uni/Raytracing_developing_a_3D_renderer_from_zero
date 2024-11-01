//
// Created by tommasomarialopedote on 18/07/24.
//

#ifndef R3DFROM0_R3DF0_SHAPES_H
#define R3DFROM0_R3DF0_SHAPES_H

#include "r3df0_varsutil.h"
#include "r3df0_material.h"
#include "r3df0_axisAlignBBoxes.h"

using namespace std;

namespace r3dfrom0{
    class sphere : public hittable{
    public:
        // constructors
        sphere() : center{0,0,0}, radius{1.0} {}; // default behavior
        sphere(const vec3f& c, const float& r, shared_ptr<material> mat) :
                center(c), radius(float (fmax(0,r))), material_ptr(mat) {
            // build bvh
            auto temp_v = vec3f{radius, radius, radius};
            bbox = axisAlignBbox{center - temp_v, center + temp_v};
        };


        // methods
        bool hit(const ray& r, interval i, hit_record& hit_record) const override{
            auto c_q = center - r.origin();
            // optimized the dot function of a vector by noticing that the dot function of a
            // vector with itself is just the sqr_length of said vector.
            auto a = r.direction().sqr_length(); // d^2
            // b in old func is now d * (C - Q)
            auto h = dot(r.direction(), c_q); // d * (C-Q)
            // optimized the dot function of a vector to itself is, sqr_length of said vector
            auto c = c_q.sqr_length() - (radius * radius); // (C - Q)^2 - r^2
            // delta optimized by algebraic simplification
            auto delta = h * h - a * c;
            if (delta < 0) {
                return false;
            }
            auto sqrt_delta = sqrt(delta);
            // roots of the quadratic formula
            auto roots = (h - sqrt_delta) / a;
            if (i.excludes(roots)){
                roots = (h + sqrt_delta) / a;
                if (i.excludes(roots)) {
                    return false;
                }
            }

            // adding tuple to save the hit to the sphere object
            hit_record.t = roots;
            hit_record.position = r.at(hit_record.t); // at what point the ray intersected the sphere
            auto normal = (hit_record.position - center) / radius; // calc normal on ray hit
            hit_record.set_face_normals(r, normal);
            hit_record.material_ptr = material_ptr;
            get_sphere_uv(hit_record.normal, hit_record.u, hit_record.v);
            return true;
        } // hit method

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

        static void get_sphere_uv(vec3f& p, float& u, float& v){
            float phi = atan2(-p.z, p.x) + pi;
            float theta = acos(-p.y);
            u = phi / (2 * pi);
            v = theta / pi;
        }

        void apply_w2o() const override {
            transform_point(center, *w2o);
        }

        void apply_o2w() const override {
            transform_point(center, *o2w);
        }

    private:
        // attributes
        vec3f center;
        float radius = 1.0;
        shared_ptr<material> material_ptr;
        axisAlignBbox bbox;
    }; // sphere class


    class quad : public hittable {
    public:
        // constructors
        quad(const vec3f& v1, const vec3f& u, const vec3f& v, shared_ptr<material> material, bool ss = false) :
            v1(v1), u(u), v(v), material(material), single_side(ss) {
            v2 = v1 + u;     // lower right     4_______3
            v3 = v1 + u + v; // upper right    /       /
            v4 = v1 + v;     // upper left   1/_______/2

            set_bounding_box();
        }

        quad (const vec3f& v1, const vec3f& v2, const vec3f& v3, const vec3f& v4, shared_ptr<material> material, bool ss = false) :
            v1(v1), v2(v2), v3(v3), v4(v4), material(material), single_side(ss) {
            u = v2 - v1;
            v = v4 - v1;

            set_bounding_box();
        }

        // methods
        bool hit(const ray& r, interval i, hit_record& hit_record) const override {
            auto normal = unit(cross(u,v));
            auto denominator = dot(normal, r.direction());
            auto D = dot(v1, normal);
            if (fabs(denominator) < 1e-8) { // check if ray is parallel to the surface
                return false;
            }

            // check facing angle
            if (fabs(denominator) > 0 && single_side) {
                return false;
            }

            auto t = (D - dot(normal, r.origin())) / denominator;
            if (!i.includes(t)) { // check if t intersection point is behind the surface
                return false;
            }
            auto intersect = r.at(t);

            auto c1 = intersect - v1;
            auto c2 = intersect - v2;
            auto c3 = intersect - v3;
            auto c4 = intersect - v4;
            if( dot(normal, cross(v2 - v1, c1)) < 0 ||
                dot(normal, cross(v3 - v2, c2)) < 0 ||
                dot(normal, cross(v4 - v3, c3)) < 0 ||
                dot(normal, cross(v1 - v4, c4)) < 0   ){
                return false;
            }

            // save data in hit_record
            hit_record.position = intersect;
            hit_record.t = t;
            hit_record.set_face_normals(r, normal);
            hit_record.material_ptr = material;
//            get_quad_uv(hit_record.normal, hit_record.u, hit_record.v); // TODO: IMPLEMENT get_quad_uv()
            return true;
        }


        virtual void set_bounding_box() {
            auto bbox1 = axisAlignBbox(v1, v3);
            auto bbox2 = axisAlignBbox(v2, v4);
            bbox = axisAlignBbox(bbox1, bbox2);
        }

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

        static void get_quad_uv(vec3f& p, float& u, float& v){
            // TODO: COMPLETE 2D UV FOR TEXTURES THE BOOK HAS NOT PROVIDED A GOOD INSIGHT;
        }

        void apply_w2o() const override {
            transform_point(v1, *w2o);
            transform_point(v2, *w2o);
            transform_point(v3, *w2o);
            transform_point(v4, *w2o);
        }

        void apply_o2w() const override {
            transform_point(v1, *o2w);
            transform_point(v2, *o2w);
            transform_point(v3, *o2w);
            transform_point(v4, *o2w);
        }

    private:
        // attributes
        vec3f u, v;                         // horizontal and vertical vectors respectfully
        vec3f v1, v2, v3, v4;                // coordinates of the vertices of the quad in counter clock-wise order;
        shared_ptr<material> material;
        axisAlignBbox bbox;
        bool single_side;
    }; // quad class


    class triangle : public hittable {
    public:
        // constructors
//        triangle(const vec3f& v1, const vec3f& u, const vec3f& v, shared_ptr<material> material) :
//                v1(v1), u(u), v(v), material(material) {
//            // lower left + 2 vectors to find the other 2 vertices definition
//            v2 = v1 + u;     // lower right
//            v3 = v1 + v;     // upper left
//
//            set_bounding_box();
//        }
        triangle(const vec3f& v1, const vec3f& v2, const vec3f& v3, shared_ptr<material> material, bool ss = false) :
                v1(v1), v2(v2), v3(v3), material(material), single_side(ss) {
            u = v2 - v1;
            v = v3 - v1;

            set_bounding_box();
        }

        // methods
        friend inline ostream& operator<<(ostream& out, const triangle& t){
            return out <<"triangle_instance: (" << t.v1 << "," << t.v2 << "," << t.v3 << ")" << endl;
        }


        bool hit(const ray& r, interval i, hit_record& hit_record) const override {
//            auto origin_transform = transform_point(r.origin(), object_to_world);
//            auto direction_transform = transform_point(r.direction(), object_to_world);
//            ray r_t = ray(origin_transform, direction_transform);
            auto r_t = r;

            auto normal = cross(u,v);
            auto denominator = dot(normal, r_t.direction());
            auto D = -dot(v1, normal);
            if (fabs(denominator) < 1e-8) { // check if ray is parallel to the surface
                return false;
            }

            // check facing angle
            if (fabs(denominator) > 0 && single_side) {
                return false;
            }

            auto t = -(D + dot(normal, r_t.origin())) / denominator;
            if (!i.includes(t)) { // check if t intersection point is behind the surface
                return false;
            }

            if (dot(r_t.direction(), normal) > 0 && single_side)
                return false;

            auto intersect = r_t.at(t);

            auto c1 = intersect - v1;
            auto c2 = intersect - v2;
            auto c3 = intersect - v3;
            if( dot(normal, cross(v2 - v1, c1)) < 0 ||
                dot(normal, cross(v3 - v2, c2)) < 0 ||
                dot(normal, cross(v1 - v3, c3)) < 0 ){
                return false;
            }

            // save data in hit_record
            hit_record.position = intersect;
            hit_record.t = t;
            hit_record.set_face_normals(r_t, normal);
            hit_record.material_ptr = material;
//            get_triangle_uv(hit_record.normal, hit_record.u, hit_record.v); // TODO: IMPLEMENT get_triangle_uv()
            return true;
        }

        virtual void set_bounding_box() {
            auto bbox1 = axisAlignBbox(v1, v3);
            auto bbox2 = axisAlignBbox(v1, v2);
            bbox = axisAlignBbox(bbox1, bbox2);
        }

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

        void apply_w2o() const override {
            transform_point(v1, *w2o);
            transform_point(v2, *w2o);
            transform_point(v3, *w2o);
        }

        void apply_o2w() const override {
            transform_point(v1, *o2w);
            transform_point(v2, *o2w);
            transform_point(v3, *o2w);
        }


    private:
        // attributes
        vec3f u, v;                         // horizontal and vertical vectors respectfully
        vec3f v1, v2, v3;                   // coordinates of the vertices of the quad in counter clock-wise order;
        shared_ptr<material> material;
        axisAlignBbox bbox;
        const bool single_side;
    }; // triangle class

    inline shared_ptr<hittable_list> box_quad(const vec3f& a, const vec3f& b, shared_ptr<material> mat){
        auto quad_list = make_shared<hittable_list>();
        // lowest and highest vertices
        auto min = vec3f(fmin(a.x,b.x), fmin(a.y,b.y), fmin(a.z,b.z));
        auto max = vec3f(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
        // find vectors
        auto dx = vec3f(max.x - min.x, 0, 0);
        auto dy = vec3f(0, max.y - min.y, 0);
        auto dz = vec3f(0, 0, max.z - min.z);
        // define quads
        quad_list->append(make_shared<quad>(vec3f(min.x, min.y, max.z), dx, dy, mat)); // front
        quad_list->append(make_shared<quad>(vec3f(max.x, min.y, max.z), -dz, dy, mat)); // right
        quad_list->append(make_shared<quad>(vec3f(max.x, min.y, min.z), -dx, dy, mat)); // back
        quad_list->append(make_shared<quad>(vec3f(min.x, min.y, min.z), dz, dy, mat)); // left
        quad_list->append(make_shared<quad>(vec3f(min.x, min.y, min.z), dz, dx, mat)); // bottom
        quad_list->append(make_shared<quad>(vec3f(max.x, max.y, max.z), -dx, -dz, mat)); // top

        return quad_list;
    } // box_quad function

    inline shared_ptr<hittable_list> pyramid_sqr_base(const vec3f& v1, const vec3f& u, const vec3f& v,
                                                      const vec3f& h, shared_ptr<material> mat) {
        auto pyramid_list = make_shared<hittable_list>();
        pyramid_list->append(make_shared<quad>(v1, u, v, mat)); // base
        // define vertex of base
        auto v2 = v1 + u;     // lower right     4_______3
        auto v3 = v1 + u + v; // upper right    /       /
        auto v4 = v1 + v;     // upper left   1/_______/2

        // define x,y position vertex of the apex
        auto center = vec3f((v1.x + v2.x + v3.x + v4.x)/4, 0,(v1.z + v2.z + v3.z + v4.z)/4);
        auto apex = center + h;
        // (43, 124, 255)
        pyramid_list->append(make_shared<triangle>(v1, v2, apex, mat)); // front
        pyramid_list->append(make_shared<triangle>(v2, v3, apex, mat)); // right
        pyramid_list->append(make_shared<triangle>(v3, v4, apex, mat)); // back
        pyramid_list->append(make_shared<triangle>(v4, v1, apex, mat)); // left

        return pyramid_list;
    }
}

#endif //R3DFROM0_R3DF0_SHAPES_H
