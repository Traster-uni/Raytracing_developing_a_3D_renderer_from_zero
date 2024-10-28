//
// Created by trast on 07/07/2024.
//

#ifndef R3DFROM0_R3DF0_IMAGE_H
#define R3DFROM0_R3DF0_IMAGE_H

#include "r3df0_varsutil.h"
#include "r3df0_color.h"

#include "stb_image/stb_image/stb_image_runner.h"
#include <cstdlib>

using namespace std;

namespace r3dfrom0{
    // utility functions
    vector<vector<pixel_i>> texture_not_found(const int& w = 100, const int& h = 100){
        const pixel_i MAGENTA = {255, 0, 255}; // SOMETHING WENT WRONG COLOR
        vector<vector<pixel_i>> empty_img = {};
        for (int i = 0; i < w; i++){
            vector<pixel_i> row = {};
            empty_img.push_back(row);
            for (int j = 0; j < h; j++){
                empty_img[i].push_back(MAGENTA);
            }
        }
        return empty_img;
    } // texture_not_found

    int float_to_int(float n){
        if (n <= 0){
            return 0;
        } else if (n >= 1){
            return 255;
        } else {
//            interval i = {0, 255};
//            return i.clamp(255.9999f * n);
            return 255 * n;
        }
    }

    float int_to_float(int n){
        const interval interval(0.0f, 1.0f);
        return interval.clamp(n/255.0f);
    }

    bool  load_img(vector<vector<pixel_f>>& tuple_img, const string& filename, int& width, int& height){
        //    width;          -- width of the loaded image
        //    height;         -- height of the loaded image
        int bytes_pixel;      // number of bytes per pixel [1,8]
        int bytes_scanline;   // number of bytes per row in image
        int bytes_total;      // number of bytes in the entire image

        auto dir_fname = R"(C:\Raytracing_developing_a_3D_renderer_from_zero\resources\textures\)" + filename;
        auto env = getenv("RTW_IMAGES");

        // FILL INFO
        int query_result = stbi_info(dir_fname.c_str(), &width, &height, &bytes_pixel);

        // LOAD FILE
        auto file_data = stbi_load(dir_fname.c_str(), &width, &height, &bytes_pixel, 3);
        if (file_data == nullptr){
            cout << "\nCall to load_img(" << dir_fname << "): Failed to open file" << endl;
            return false;
        };

        // CONVERSION TO VEC3F
        bytes_scanline = width * bytes_pixel;
        bytes_total = bytes_scanline * height;

        int r = 0;       // row counter
        for (int i = 0; i < bytes_total; i+=bytes_scanline, r++){
            vector<pixel_f> scanline = {};
            tuple_img.push_back(scanline);
            int c = 0;   // column counter
            for (int j = 0; j < bytes_scanline; j+=3, c++){
                // stbi_load already loads data as bytes!
                auto color_comp_r = int (*file_data++);
                auto color_comp_g = int (*file_data++);
                auto color_comp_b = int (*file_data++);
                pixel_f pixel = {int_to_float(color_comp_r),
                                 int_to_float(color_comp_g),
                                 int_to_float(color_comp_b)};
                tuple_img[r].push_back(pixel); // write pixel tuple
            }
        }
        return true;
    } // load_png


    void write_png(const string& filename, int& width, int& height, vector<vector<pixel_i>> tuple_image){
        uint8_t bytes_pixel = 3;
        int bytes_scanline = width * bytes_pixel;
        int bytes_total = width * height * bytes_pixel;

        auto *pixels_ptr = new unsigned char[bytes_total];
        auto l = 0;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                auto p = tuple_image[i][j];
                // convert pixel_i to float component and push them into array
                pixels_ptr[++l] = (unsigned char) p.g;
                pixels_ptr[++l] = (unsigned char) p.b;
                pixels_ptr[++l] = (unsigned char) p.r;
            }
        }

        stbi_write_png(filename.c_str(), width, height, 3, pixels_ptr, bytes_scanline);
    } // write_img


    void ppm_out(vector<vector<pixel_f>>& img_data, const string& fname){
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

    pixel_i correct_color_gamma(const pixel_f& color_data){
        auto color_gamma_corrected = pixel_f{linear_to_gamma(color_data.r),
                                             linear_to_gamma(color_data.g),
                                             linear_to_gamma(color_data.b)};
        auto p_i = convert_pixel_i_clamp(color_gamma_corrected); // clamps colors and converts into integers
        return p_i;
    } // correct_color_gamma


    void write_color_ppm(ostream& out_stream, const pixel_f& color_data){
        out_stream << correct_color_gamma(color_data) << " ";
    } // write color

} // namespace r3dfrom0

#endif //R3DFROM0_R3DF0_IMAGE_H
