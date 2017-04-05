#ifndef _REGION_H
#define _REGION_H

#include <string>
#include <tuple>
#include <vector>

#include "sphere.h"


namespace SPacker {

    class Packer;

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
        
        // Must be implemented
        virtual bool intersects(Sphere &s) = 0;
        virtual bounds getBoundingBox() = 0;
        virtual double V() = 0;
        inline std::vector<std::vector<Sphere> *> *Spheres() { return &(this->spheres); }
        
        // Can be implemented
        bool packRegion(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Packer *packer);
        
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

    class SpherocylinderRegion : public Region {
    public:
        SpherocylinderRegion(double radius, double length)
        :radius(radius)
        ,length(length)
        {}
        
        virtual bool intersects(Sphere &s);
        virtual double V();
        
        virtual bounds getBoundingBox();
        
    
    private:
        double radius;
        double length;
        
    };

}

#endif
