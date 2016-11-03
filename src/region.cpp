
#include <algorithm>
#include <exception>
#include <vector>

using namespace std;

#include "region.h"


namespace SPacker {
    
    bool Region::packRegion(vector<double> radii, vector<int> counts) {
        // Number of radii needs to be the same as nuber of counts
        if(radii.size() != counts.size())
            throw exception();

    }
    
}
