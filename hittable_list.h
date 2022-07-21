#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

// From the book: "We'll use shared pointers in our code, because it allows multiple geometries to share a common instance 
// (for example, a bunch of spheres that all use the same texture map material)"
using std::shared_ptr;  
using std::make_shared;

/*This class stores a list of hittable objects.*/

class hittable_list : public hittable {

    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    // The single colon is used in a range based loop: https://www.geeksforgeeks.org/range-based-loop-c/
    // See also https://stackoverflow.com/questions/221346/what-can-i-use-instead-of-the-arrow-operator for the meaning of "->"
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;    // Returns the value of t for the intersection with the closest object so far
            rec = temp_rec;
        }
    }

    // Returns true if an object is hit
    return hit_anything;
}

#endif