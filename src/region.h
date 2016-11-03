#ifndef _REGION_H
#define _REGION_H

#include <vector>

#include "sphere.h"

namespace SPacker {

class Region {
public:
    Region() {};
    
    virtual ~Region() {};
    
    virtual bool intersects(Sphere &s) = 0;
    
    bool packRegion(std::vector<double> radii, std::vector<int> counts);
    
    virtual double V() = 0;
    
private:
    
    double volume;
    
    std::vector<std::vector<Sphere *> *> spheres;
    
};
    
}

#endif
