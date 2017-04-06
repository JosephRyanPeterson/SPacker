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
                if(attempts > 10*countToAdd)
                    throw exception();
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
                        if((*sphereIter).intersects(trial)) {
                            successful = false;
                            break;
                        }
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


    bool UniformGridPacker::pack(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Region *region) {
        // Get the bounding box
        bounds bbox = region->getBoundingBox();
        double bbdx = bbox.xmax-bbox.xmin;
        double bbdy = bbox.ymax-bbox.ymin;
        double bbdz = bbox.zmax-bbox.zmin;
        // Get region spheres
        auto spheres = region->Spheres();
       
        // Create grid
        double dx = 0.0;
        for(auto iter = objects.begin(); iter != objects.end(); iter++) {
            if(get<1>(*iter) > dx) {
                dx = get<1>(*iter);
            }
        }
        dx *= 3.0;
        uint32_t nx = std::ceil(bbdx/dx);
        uint32_t ny = std::ceil(bbdy/dx);
        uint32_t nz = std::ceil(bbdz/dx);
        uint32_t gridPoints = nx*ny*nz;
        auto index = [nx,ny](uint32_t ix, uint32_t iy, uint32_t iz) { return ix + nx*(iy + ny*iz); };

        std::vector<std::vector<Sphere> *> grid;
        std::vector<std::vector<std::string> *> objIdxs;
        for(uint32_t i = 0; i < gridPoints; i++) {
            grid.push_back(new vector<Sphere>());
            objIdxs.push_back(new vector<std::string>());
        }
 
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
            double x, y, z; // Trial sphere center 
            uint32_t ix, iy, iz; // Uniform grid indices
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
                    x = ((double)rand()/(double)RAND_MAX - 0.5f)*bbdx;
                    y = ((double)rand()/(double)RAND_MAX - 0.5f)*bbdy;
                    z = ((double)rand()/(double)RAND_MAX - 0.5f)*bbdz;
                    trial = Sphere(x,y,z,radius);
                } while(!region->intersects(trial));
               
                // Get local grid coordinate
                ix = (uint32_t)floor((x+bbdx)/(2.0*dx));
                iy = (uint32_t)floor((y+bbdx)/(2.0*dx));
                iz = (uint32_t)floor((z+bbdx)/(2.0*dx));
 
                // Find if it intersects
                bool successful = true;
                for(uint32_t i = std::max(uint32_t(0),ix-1); i < std::min(nx,ix+1); i++) {
                    for(uint32_t j = std::max(uint32_t(0),iy-1); j < std::min(ny,iy+1); j++) {
                        for(uint32_t k = std::max(uint32_t(0),iz-1); k < std::min(nz,iz+1); k++) {
                            for(auto sphereIter = grid[index(i,j,k)]->begin(); sphereIter != grid[index(i,j,k)]->end(); sphereIter++) {
                                if((*sphereIter).intersects(trial)) {
                                    successful = false;
                                    i = nx;
                                    j = ny;
                                    k = nz;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(successful) {
                    grid[index(ix,iy,iz)]->push_back(trial);
                    objIdxs[index(ix,iy,iz)]->push_back(get<0>(*iter));
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
