#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

// The single collon here is used to denote inheriterance, see https://www.learncpp.com/cpp-tutorial/basic-inheritance-in-c/
class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override; // Check https://stackoverflow.com/questions/18198314/what-is-the-override-keyword-in-c-used-for

    public:
        point3 center;
        double radius;
};

// Accepts a range [t_min, t_max] for the range
// The double colon :: is the scope resolution operator, and makes clear to which namespace something belongs. See: https://stackoverflow.com/questions/5345527/what-does-the-mean-in-c
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());         // Here we make a redefinition with a new variable h = b/2. This somewhat simplifies the equations
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;                                       // rec is the hit record, a struct in hittable.h 
    rec.p = r.at(rec.t);                                // Remember that ray.at(t) is P(t)
    vec3 outward_normal = (rec.p - center) / radius;    // Normal vector constructed from the solution
    rec.set_face_normal(r, outward_normal);             // Decides whether it is outwards or innerwards

    //rec.normal = (rec.p - center) / radius; // Normal vector constructed from the solution

    return true;
}

#endif