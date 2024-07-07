//
// Created by trast on 07/07/2024.
//

#ifndef R3DFROM0_R3DF0_IMAGE_H
#define R3DFROM0_R3DF0_IMAGE_H

// includes
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
using std::string;
using std::vector;

namespace r3dfrom0{

auto ppm_out(int w, int h, vector<vector<float>>& img_data, const string& fname){
    ofstream out;
    out.open(fname);
    // ppm header
    out << "P3\n" << w << " " << h << "\n255\n" <<endl;
    // ppm body render
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){

        }
    }
    out.close();
} // ppm_out

} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_IMAGE_H
