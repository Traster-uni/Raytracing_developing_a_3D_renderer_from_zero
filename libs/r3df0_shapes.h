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
            // roots of the quadratic equation
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
            float phi = atan2(p.z, p.x) + 2*pi;
            float theta = acos(-p.y);
            u = phi / (2 * pi);
            v = theta / pi;
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
        quad(const vec3f& q, const vec3f& u, const vec3f& v, shared_ptr<material> material) :
            Q(q), u(u), v(v), material(material) {
            v1 = Q + u;     // lower right
            v2 = Q + u + v; // upper right
            v3 = Q + v;     // upper left

            edge1 = v1 - Q;
            edge2 = v2 - v1;
            edge3 = v3 - v2;
            edge4 = Q - v3;

            set_bounding_box();
        }
        quad (const vec3f& q, const vec3f& v1, const vec3f& v2, const vec3f& v3, shared_ptr<material> material) :
            Q(q), v1(v1), v2(v2), v3(v3), material(material) {
            u = v1 - Q;
            v = v3 - Q;

            edge1 = v1 - Q;
            edge2 = v2 - v1;
            edge3 = v3 - v2;
            edge4 = Q - v3;

            set_bounding_box();
        }

        // methods
        bool hit(const ray& r, interval i, hit_record& hit_record) const override {
            auto normal = unit(cross(u,v));
            auto denominator = dot(hit_record.normal, r.direction());
            auto D = -dot(Q, hit_record.normal);
            if (fabs(denominator) < 1e-8) { // check if ray is parallel to the surface
                return false;
            }

            auto t = D - dot(hit_record.normal, r.origin()) / denominator;
            if (hit_record.t <= 0) { // check if t intersection point is behind the surface
                return false;
            }
            auto intersect = r.at(t);

            auto c1 = intersect - Q;
            auto c2 = intersect - v1;
            auto c3 = intersect - v2;
            auto c4 = intersect - v3;
            if(dot(normal, cross(edge1, c1)) <= 0 ||
                dot(normal, cross(edge2, c2)) <= 0 ||
                dot(normal, cross(edge3, c3)) <= 0 ||
                dot(normal, cross(edge4, c4)) <= 0 ){
                return false;
            }
            // save data in hit_record
            hit_record.position = intersect;
            hit_record.t = t;
            hit_record.set_face_normals(r, normal);
            hit_record.material_ptr = material;
//            get_quad_uv(hit_record.normal, hit_record.u, hit_record.v);
            return true;
        }

        virtual void set_bounding_box() {
            auto bbox1 = axisAlignBbox(Q, v2);
            auto bbox2 = axisAlignBbox(v1, v3);
            bbox = axisAlignBbox(bbox1, bbox2);
        }

        axisAlignBbox bounding_box() const override{
            return bbox;
        }

        static void get_quad_uv(vec3f& p, float& u, float& v){
            // TODO: COMPLETE 2D UV FOR TEXTURES;
        }

    private:
        // attributes
        vec3f u, v;                         // horizontal and vertical vectors respectfully
        vec3f Q, v1, v2, v3;                // coordinates of the vertices of the quad in counter clock-wise order;
        vec3f edge1, edge2, edge3, edge4;   // vector edges of the quad in conter clock-wise order;
        shared_ptr<material> material;
        axisAlignBbox bbox;
    }; // quad class


    class triangle : public hittable {
    public:
        // constructors

        // methods
    private:
        // attributes

    }; // triangle class
}

#endif //R3DFROM0_R3DF0_SHAPES_H
