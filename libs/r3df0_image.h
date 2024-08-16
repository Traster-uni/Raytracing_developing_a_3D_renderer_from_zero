//
// Created by trast on 07/07/2024.
//

#ifndef R3DFROM0_R3DF0_IMAGE_H
#define R3DFROM0_R3DF0_IMAGE_H

#include "r3df0_varsutil.h"
#include "r3df0_color.h"

using namespace std;

namespace r3dfrom0{
    // utility functions
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


inline float linear_to_gamma(float linear_color){
    // linear to gamma color space conversion
    if (linear_color > 0.0f){
        return sqrt(linear_color);
    }
    return 0;
} // linear_to_gamma


void write_color(ostream& out_stream, const pixel_f& color_data){
    auto color_gamma_corrected = pixel_f{linear_to_gamma(color_data.r),
                                       linear_to_gamma(color_data.g),
                                       linear_to_gamma(color_data.b)};
    auto p_i = convert_pixel_i_clamp(color_gamma_corrected, 0.001, 0.999); // clamps colors and converts into integers
    out_stream << p_i << " ";
} // write color

} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_IMAGE_H
