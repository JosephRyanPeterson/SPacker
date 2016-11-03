#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "sphere.h"
#include "region.h"

using namespace std;

int main(int argc, char **argv) {
    char *infile  = NULL;
    char *outfile  = NULL;
    double abs_tolerance = 1e-12;
    double rel_tolerance = 1e-12;
    
    // Read command line arguments
    int c;
    opterr = 0;
    while( (c = getopt(argc, argv, "ario:")) != -1) {
        switch(c) {
            // Absolute Tolerance
            case 'a':
                abs_tolerance = atof(optarg);
                break;
            // Relative Tolerance
            case 'r':
                rel_tolerance = atof(optarg);
                break;
            // Input File
            case 'i':
                infile = optarg;
                break;
            // Output File
            case 'o':
                outfile = optarg;
                break;
            // Other
            case '?':
                if(optopt == 'c')
                    cerr << "Option -" << optopt << " requires and argument." << endl;
                else if(isprint(optopt))
                    cerr << "Unknown option `-" << optopt << "'." << endl;
                else
                    cerr << "Unknonw option character " << optopt << "." << endl;
                return 1;
            
            default:
                abort();
        }
    }
    
    return 0;
}



