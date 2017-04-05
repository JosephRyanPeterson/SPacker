#ifndef _SPHERE_H
#define _SPHERE_H

#include <exception>

namespace SPacker {

    class Sphere {
    public:
        Sphere()
        :x(0.0)
        ,y(0.0)
        ,z(0.0)
        ,radius(0.0)
        {};
        Sphere(double x,
               double y,
               double z,
               double radius)
        :x(x)
        ,y(y)
        ,z(z)
        ,radius(radius)
        {
            if(radius <= 0.0f)
                throw std::exception();
        };
        
        bool intersects(const Sphere &s);
        
        double R();
        double X();
        double Y();
        double Z();
        
    private:
        double radius;
        double x;
        double y;
        double z;
        
    };
    
}


#endif