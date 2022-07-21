#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:

        // Member Initializer List for the variable "e". See https://stackoverflow.com/questions/1711990/what-is-this-weird-colon-member-syntax-in-the-constructor 
        // and https://www.learncpp.com/cpp-tutorial/constructor-member-initializer-lists/
        vec3() : e{0,0,0} {}                                        // Defines the "0" vector. Just call with no arguments
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}    // Defines a 3D vector. Here "e" is the variable name

        // We define a print function to show the whole vector
        void print() {std :: cout << e[0] << " " << e[1] << " " << e[2] <<"\n";}

        // Here we define member functions/class methods which output the coordinates. E.g, for
        // vec3 foo = {5,0,0};  
        // foo.e[0] outputs 5
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        /*Below we have a series of operator overloadings.
        
        We can define how operators (e.g. binary operations such as +, -, etc... interact with members of this class.
        We are basically redefining how adding, multiplying and other operations work for objects in this class, i.e. 3D vectors
        
        See: https://www.geeksforgeeks.org/operator-overloading-c/ 
        
        Moreover, const is somewhat used for safety and documentation purposes, 
        when we don't want the object to change: https://stackoverflow.com/questions/4486326/does-const-just-mean-read-only-or-something-more*/
        
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // Allows us to write -v
        double operator[](int i) const { return e[i]; }              // v[i] returns the i-th coordinate
        double& operator[](int i) { return e[i]; }                   // The & is used for reference. See e.g. https://stackoverflow.com/questions/4629317/what-does-int-mean

        // Vector addition. E.g. vectorA+=vec3(1, 5, 0) add (1, 5, 0) to vectorA;
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;   // *this points to the vector (e0, e1, e2). See: https://www.tutorialspoint.com/cplusplus/cpp_this_pointer.htm
        }

        // Scalar multiplication and division
        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        // Vector modulus
        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

    // Alpha/transparency of the pixel
    public:
        double e[3];
};

// vec3 Utility Functions

// Overloads the cout function. This way we can print the vectors using cout. E.g., cout << vectorA;
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Performs binary operations on vectors such as u + v, and so on.
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Here we define a direct product between then, i.e. u*v = (u1*v1, u2*v2, u3*v3)
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// Scalar multiplication
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// The operation above defines "t*v". Here we define the operation v*t (which should be the same)
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// Defines the dot product between two vecctors
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// Defines the cross product between two vectors
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

#endif