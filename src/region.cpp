
#include <algorithm>
#include <exception>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

#include "sphere.h"
#include "region.h"
#include "packer.h"


namespace SPacker {
    
    // Region
    bool Region::packRegion(vector<tuple<string, double, uint32_t> > &objects, Packer *packer) {
        // Error checking
        double sum = 0.0;
        for(auto iter = objects.begin(); iter != objects.end(); iter++)
            sum += Sphere(0.0,0.0,0.0,get<1>(*iter)).V() * get<2>(*iter);
        if(sum >= this->V())
            throw exception();
        if(sum >= 0.65*this->V())
            cout << "WARNING: Total volume of packed items >= 0.65*region volume." << endl;
        
        // Determine large to small ordering
        sort(objects.begin(), objects.end(),
             [](tuple<string, double, uint32_t> a, tuple<string, double, uint32_t> b)
              { return get<1>(a) < get<1>(b); } );
        reverse(objects.begin(), objects.end());
        
        return packer->pack(objects, this);
    }
    
    
    bool Region::saveRegion(const char *filename, int filetype) {
        ofstream outf;
        outf.open(filename);
        if(!outf.good()) {
            throw exception();
        }
        
        switch(filetype) {
            case DEFAULT:
            {
                // For each sphere set
                for(auto setIter = spheres.begin(); setIter != spheres.end(); setIter++) {
                    // For each sphere
                    for(auto sphereIter = (*setIter)->begin(); sphereIter != (*setIter)->end(); sphereIter++) {
                        outf << (*sphereIter).X() << "\t" << (*sphereIter).Y() << "\t" << (*sphereIter).Z() << "\t" << (*sphereIter).R() << endl;
                    }
                }
            } break;
                
            case XYZ:
            {
                // Write Header
                int count = 0;
                for(auto setIter = spheres.begin(); setIter != spheres.end(); setIter++)
                    count += (*setIter)->size();
                outf << count << endl;
                outf << "Created by SPacker." << endl;
                // Write "Atoms"
                char atomtype = 'A';
                for(auto setIter = spheres.begin(); setIter != spheres.end(); setIter++) {
                    // For each sphere
                    for(auto sphereIter = (*setIter)->begin(); sphereIter != (*setIter)->end(); sphereIter++) {
                        outf << atomtype << "\t" << (*sphereIter).X() << "\t" << (*sphereIter).Y() << "\t" << (*sphereIter).Z() << endl;
                    }
                    atomtype += 1;
                }
            } break;
                
            default:
                throw exception();
        }
        
        outf.close();
        return true;
    }
    
    
    
    // Spherical Region
    bool SphericalRegion::intersects(Sphere &s) {
        return s.intersects(Sphere(0.0,0.0,0.0, this->radius));
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

    // Spherocylinder Region
    bool SpherocylinderRegion::intersects(Sphere &s) {
        bool int1 = s.intersects(Sphere(0.0,0.0,-(this->length/2.0 - this->radius), this->radius));
        bool int2 = s.intersects(Sphere(0.0,0.0,  this->length/2.0 - this->radius,  this->radius));
        bool cyl  = true;
        if( s.Z() - s.R() > this->length/2.0 || s.X() + s.R() < -this->length/2.0)
            cyl = false;
        if( s.X()*s.X() + s.Y()+s.Y() > (this->radius-s.R())*(this->radius-s.R()) )
            cyl = false;
        return int1 || int2 || cyl;
    }
    
    bounds SpherocylinderRegion::getBoundingBox() {
        bounds bb =
            {-(this->radius), -(this->radius), -(this->length/2.0),
              (this->radius),  (this->radius),  (this->length/2.0)};
        return bb;
    }
    
    double SpherocylinderRegion::V() {
        return M_PI * this->radius * this->radius *(4.0/3.0 * this->radius + (this->length - 2.0 * this->radius));
    }
    
}
