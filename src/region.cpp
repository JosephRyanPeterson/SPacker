
#include <algorithm>
#include <exception>
#include <vector>
#include <cmath>

using namespace std;

#include "region.h"


namespace SPacker {
    
    bool Region::packRegion(vector<string> &names, vector<double> &radii, vector<int> &counts) {
        // Number of radii needs to be the same as nuber of counts
        if(radii.size() != counts.size())
            throw exception();

        // Get the bounding box
        bounds bbox = this->getBoundingBox();
        
        // For each size type, pack
        // TODO
    }
    
    bool SphericalRegion::intersects(Sphere &s) {
        Sphere thisRegion(0.0,0.0,0.0, this->radius);
        return s.intersects(thisRegion);
    }
    
    bounds SphericalRegion::getBoundingBox() {
        bounds bb =
            {-(this->radius), -(this->radius), -(this->radius),
              (this->radius),  (this->radius),  (this->radius)};
        
        return bb;
    }
    
    double SphericalRegion::V() {
        return 4.0/3.0 * M_PI * this->radius * this->radius * this->radius;
    }
    
}
