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
#include <r3df0_color.h>

using namespace std;
using std::string;
using std::vector;

namespace r3dfrom0{

auto ppm_out(vector<vector<pixel_f>>& img_data, const string& fname){
    /**
     * PIXEL FORMAT: (float, float, float) => (red, green, blue)
     * This function scales the float in an integer between 0 and 255
     */
    auto columns = int (img_data[0].size());
    auto rows = int (img_data.size());
    ofstream out;
    out.open(fname);
    // ppm header
    out << "P3\n" << columns << " " << rows << "\n255\n" <<endl;
    // ppm body render
    for (int i = 0; i < rows; i++){
        clog << "\rScanlines remaining: " << (rows - i) << flush;
        for (int j = 0; j < columns; j++){
            auto p_i = convert_pixel_i(img_data[i][j]);
            out << p_i << " ";
        }
        out << endl;
    }
    out.close();
    clog << "\rDone                                 \n" << flush;
} // ppm_out

} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_IMAGE_H
