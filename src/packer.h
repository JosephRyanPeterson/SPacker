#ifndef _PACKER_H
#define _PACKER_H

#include <string>
#include <tuple>
#include <vector>


namespace SPacker {

    class Region;

    // Base class for packing spheres into a region
    class Packer {
    public:
        Packer() {}
        ~Packer() {}

        virtual bool pack(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Region *region) = 0;
        
    private:
    };

    // Simple packer
    class SimplePacker : public Packer {
    public:
        SimplePacker() {}
        ~SimplePacker() {}

        virtual bool pack(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Region *region);
        
    private:
    };

    // Uniform Grid packer
    class UniformGridPacker : public Packer {
    public:
        UniformGridPacker() {}
        ~UniformGridPacker() {}

        virtual bool pack(std::vector<std::tuple<std::string, double, uint32_t> > &objects, Region *region);
        
    private:
    };


}

#endif
