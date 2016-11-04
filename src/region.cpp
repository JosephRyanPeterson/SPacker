
#include <algorithm>
#include <exception>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

#include "region.h"


namespace SPacker {
    
    // Region
    bool Region::packRegion(vector<tuple<string, double, uint32_t> > &objects) {
        // Error checking
        double sum = 0.0;
        for(auto iter = objects.begin(); iter != objects.end(); iter++)
            sum += Sphere(0.0,0.0,0.0,get<1>(*iter)).V() * get<2>(*iter);
        if(sum > this->V())
            throw exception();
        
        // Determine large to small ordering
        // TODO
        
        // Get the bounding box
        bounds bbox = this->getBoundingBox();
        
        // For each size type, pack
        double x;
        double y;
        double z;
        vector<uint32_t> countsPacked;
        for(auto iter = objects.begin(); iter != objects.end(); iter++) {
            cout << "Packing: " << get<0>(*iter) << endl;
            uint32_t attempts = 0;
            uint32_t countToAdd = get<2>(*iter);
            double radius = get<1>(*iter);
            double x, y, z;
            countsPacked.push_back(0);
            spheres.push_back(new vector<Sphere>());
            while(countsPacked.back() < countToAdd) {
                // Error checking and printing
                if(attempts > 10*countToAdd) {
                    throw exception();
                }
                if(countsPacked.back() % (countToAdd/10) == 0)
                    cout << "  " << countsPacked.back() << endl;
                
                // Generate random sphere and attempt to place
                Sphere trial = Sphere(x,y,z,radius);
                do {
                    x = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.xmax-bbox.xmin);
                    y = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.ymax-bbox.ymin);
                    z = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.zmax-bbox.zmin);
                    trial = Sphere(x,y,z,radius);
                } while(!this->intersects(trial));
                
                // Find if it intersects
                bool successful = true;
                for(auto sphereSizeIter = spheres.begin(); sphereSizeIter != spheres.end(); sphereSizeIter++) {
                    for(auto sphereIter = (*sphereSizeIter)->begin(); sphereIter != (*sphereSizeIter)->end(); sphereIter++) {
                        if(trial.intersects(*sphereIter)) {
                            successful = false;
                            sphereSizeIter = spheres.end();
                            break;
                        }
                    }
                }
                if(successful) {
                    spheres.back()->push_back(trial);
                    countsPacked.back()++;
                }
                
                
                // Update placement attempts
                attempts++;
            }
        }
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
    
}
