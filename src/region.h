#ifndef _REGION_H
#define _REGION_H

#include <string>
#include <tuple>
#include <vector>

#include "sphere.h"

namespace SPacker {

    enum {
        DEFAULT = 0,
        XYZ = 1
    };
    
    
    typedef struct {
        double xmin;
        double ymin;
        double zmin;
        
        double xmax;
        double ymax;
        double zmax;
    } bounds;
    
    class Region {
    public:
        Region() {};
        
        virtual ~Region() {};
        
        // Must be overwritten
        virtual bool intersects(Sphere &s) = 0;
        virtual bounds getBoundingBox() = 0;
        virtual double V() = 0;
        
        // Can be overwritten
        bool packRegion(std::vector<std::tuple<std::string, double, uint32_t> > &objects);
        
        // Save Region
        virtual bool saveRegion(const char *filename, int filetype = XYZ);
        
        
    protected:
        
        double volume;
        
        std::vector<std::vector<Sphere> *> spheres;
        
    };
    
    
    class SphericalRegion : public Region {
    public:
        SphericalRegion(double radius)
        :radius(radius)
        {}
        
        virtual bool intersects(Sphere &s);
        virtual double V();
        
        virtual bounds getBoundingBox();
        
    
    private:
        double radius;
        
    };

}

#endif
