#include <cmath>

#include "sphere.h"


namespace SPacker {
    bool Sphere::intersects(const Sphere &s) {
        double xdiff = (s.x - this->x);
        double ydiff = (s.y - this->y);
        double zdiff = (s.z - this->z);
        double rdiff = (s.radius - this->radius);
        if(xdiff*xdiff + ydiff*ydiff + zdiff*zdiff <= rdiff*rdiff)
            return true;
        else
            return false;
    }
    
}

