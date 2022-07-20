#include <iostream>

#include "vec3.h"

int main(){

    vec3 vector0{};               //Uses the initializer_list defined in vec3h
    vec3 vectorA = {7,-1,0};      // Copy initialization. See: https://stackoverflow.com/questions/24953658/what-are-the-differences-between-c-like-constructor-and-uniform-initialization
    vec3 vectorB(2,0,1);          // Direct initialization, in this context they are requivalent

    // We plot the first coordinate of each of the vectors
    std :: cout<<"The x coordinate of vector0 is: "<<vector0.e[0]<<"\n";
    std :: cout<<"The x coordinate of vectorA is: "<<vectorA.e[0]<<"\n"; 
    std :: cout<<"The x coordinate of vectorB is: "<<vectorB.x()<<"\n\n";

    // We show how the print() fuction in the class vec3 and the - operator can be used
    std :: cout<<"The opposite vectorA is: ";
    (-vectorA).print(); 
    std :: cout<<"The y coordinate of vectorA is: "<<vectorA[1]<<"\n"; 

    // We show the operator overload in action
    vectorA*=2.;
    std :: cout<<"The double of vectorA is: ";
    vectorA.print();

    vectorA+=vec3(-4,3,1);
    std :: cout<<"Adding (-4, 3, 1): ";
    vectorA.print(); 

    // Dot product and direct product
    std :: cout<<"Its dot product with "<<vectorB<<" is "<<dot(vectorA, vectorB)<<"\n";
    std :: cout<<"Using the operator * we get "<<vectorA*vectorB<<" as well \n";

    return 0;
}