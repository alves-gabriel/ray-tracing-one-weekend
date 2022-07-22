#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;   // Material property. E.g., diffuse, metal etc. This is a pointer to the class material
    double t;
    bool front_face;                // True if hit in the front, False if hit form the back

    // We use this to decide wheter the object is hit from the inside or the outside
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {

        front_face = dot(r.direction(), outward_normal) < 0;   // Computes the inner product between the the front face and the ray
        normal = front_face ? outward_normal :-outward_normal; // If true, we are hitting the front face. Otherwise we are hitting the back, returns the opposite normal
    }
};

class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif