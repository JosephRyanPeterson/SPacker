#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <tuple>
#include <vector>

#include "sphere.h"
#include "region.h"
#include "packer.h"

using namespace std;
using namespace SPacker;


// Program Arguments
typedef struct {
    // Command Line Arguments
    char *infile;
    char *outfile;
    double abs_tolerance;
    double rel_tolerance;
    
    // Algorithm Arguments
    vector<tuple<string, double, uint32_t> > objects;
    string regionType;
    
    // Algorithm Objects
    Region *region;
} InputParameters;

InputParameters params;


// Functions to guage input parameters
bool readInputFile(InputParameters &params); // False on failure




int main(int argc, char **argv) {
    // Initialize arguments
    params.infile  = NULL;
    params.outfile = NULL;
    params.abs_tolerance = 1e-12;
    params.rel_tolerance = 1e-12;
    
    
    // Read command line arguments
    int c;
    opterr = 1;
    while( (c = getopt(argc, argv, "h::a::r::i:o:")) != -1) {
        switch(c) {
            case 'h':
                cout << "spacker -i [input file] -o [output file] [OPTIONAL: -h -a [absolute tolerance] -o [relative tolerance]]" << endl;
                return 0;

            // Absolute Tolerance
            case 'a':
                params.abs_tolerance = atof(optarg);
                break;
                
            // Relative Tolerance
            case 'r':
                params.rel_tolerance = atof(optarg);
                break;
                
            // Input File
            case 'i':
                params.infile = optarg;
                break;
                
            // Output File
            case 'o':
                params.outfile = optarg;
                break;
                
            // Other
            case '?':
                if(optopt == 'c')
                    cerr << "Option -" << (char)optopt << " requires and argument." << endl;
                else if(isprint(optopt))
                    cerr << "Unknown option `-" << (char)optopt << "'." << endl;
                else
                    cerr << "Unknonw option character " << (char)optopt << "." << endl;
                return 1;
            
            default:
                cerr << "Aborting..." << endl;
                abort();
        }
    }
    if(params.infile == NULL || params.outfile == NULL) {
        cerr << "Input and output files must be specified." << endl;
        return 1;
    }
    
    // Read the input file
    if(!readInputFile(params)) {
        cerr << "Reading input file failed: " << params.infile << endl;
        return 1;
    }
    
    // Run algorithm
    try {
        UniformGridPacker packer;
        params.region->packRegion(params.objects, &packer);
    } catch(exception &e) {
        
    }
    
    // Save the region
    params.region->saveRegion(params.outfile);
    
    
    // Clean up
    delete params.region;
    
    return 0;
}



bool readInputFile(InputParameters &params) {
    ifstream in;
    in.open(params.infile, ifstream::in);
    if(!in.good()) {
        return false;
    }
    
    // Read the Region Type
    in >> params.regionType;
    if(params.regionType.compare(string("spherical")) == 0) {
        cout << "Constructing spherical region..." << endl;
        double radius;
        in >> radius;
        params.region = new SphericalRegion(radius);
        cout << "  Radius: " << radius << endl;
    } else if(params.regionType.compare(string("spherocylinder")) == 0)  {
        cout << "Constructing spherocylinder region..." << endl;
        double radius;
        double length;
        in >> radius >> length;
        params.region = new SpherocylinderRegion(radius,length);
        cout << "  Radius: " << radius << " Length: " << length << endl;
    } else {
        cerr << "Unknown region type: " << params.regionType << endl;
        return false;
    }
    
    // Read the particle sizes
    string name;
    uint32_t count;
    double radius;
    cout << "Reading packing objects..." << endl;
    while(in >> name) {
        in >> count >> radius;
        params.objects.push_back(make_tuple(name,radius,count));
        cout << "  " << name << ":\t" << radius << "\t" << count << endl;
    }
    
    in.close();
    return true;
}


