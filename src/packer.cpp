#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#include "sphere.h"
#include "packer.h"
#include "region.h"

using namespace std;

namespace SPacker {


    bool SimplePacker::pack(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Region *region) {

        // Get the bounding box
        bounds bbox = region->getBoundingBox();

        // Get region spheres
        auto spheres = region->Spheres();
        
        // For each size type, pack
        double x;
        double y;
        double z;
        vector<uint32_t> countsPacked;
        int currCount = 1;
        int countTotal = objects.size();
        for(auto iter = objects.begin(); iter != objects.end(); iter++) {
            cout << "Packing (" << currCount <<"/" << countTotal << "): " << get<0>(*iter) << " Radius: " << get<1>(*iter)<< endl;
            uint32_t attempts = 0;
            uint32_t countToAdd = get<2>(*iter);
            double radius = get<1>(*iter);
            double x, y, z;
            countsPacked.push_back(0);
            spheres->push_back(new vector<Sphere>());
            while(countsPacked.back() < countToAdd) {
                // Error checking and printing
                if(attempts > 10*countToAdd) {
                    throw exception();
                }
                if(countsPacked.back() % std::max((countToAdd/10),(uint)1) == 0)
                    cout << "  " << countsPacked.back() << endl;
                
                // Generate random sphere and attempt to place
                Sphere trial = Sphere(x,y,z,radius);
                do {
                    x = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.xmax-bbox.xmin);
                    y = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.ymax-bbox.ymin);
                    z = ((double)rand()/(double)RAND_MAX - 0.5f)*(bbox.zmax-bbox.zmin);
                    trial = Sphere(x,y,z,radius);
                } while(!region->intersects(trial));
                
                // Find if it intersects
                bool successful = true;
                for(auto sphereSizeIter = spheres->begin(); sphereSizeIter != spheres->end(); sphereSizeIter++) {
                    for(auto sphereIter = (*sphereSizeIter)->begin(); sphereIter != (*sphereSizeIter)->end(); sphereIter++) {
                            successful = false;
                            break;
                    }
                    if(successful == false)
                        break;
                }
                if(successful) {
                    spheres->back()->push_back(trial);
                    countsPacked.back()++;
                }
                
                
                // Update placement attempts
                attempts++;
            }
            // Update counter
            currCount++;
        }

        return true;
    }

}
