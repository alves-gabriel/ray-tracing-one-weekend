#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

// The material class defines the behaviour of the rays, such as in which way and how much they scatter
class material {
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

// Lambertinan reflection. 
class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector(); // If this vector is the exact opposite of the normal, we have get the zero vector. This is dealt in the vec3 class

            // Catch degenerate scatter direction
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;

            return true;
        }

    public:
        color albedo;
};

// Uses the equal-angle reflection for metals
class metal : public material {
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}             // Takes fuzzy reflection into account

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());           // Reflection + Fuzzy contribution 
            attenuation = albedo;

            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        double fuzz;

};

class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {

            attenuation = color(1.0, 1.0, 1.0);                         // Absorbs nothing
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;   // Refraction index is n from the front and 1/n from the back

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

            // True if sin(theta)>1/n
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            // Takes total reflection into account, when sin(theta')>1 in Snell's law
            // We also use Schlick's approximation for variable reflectivity
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    public:
        double ir; // Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) {

            // Use Schlick's approximation for reflectance.
            // This will, among other things, make borders much smoother
            // See: https://en.wikipedia.org/wiki/Schlick's_approximation
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};

#endif