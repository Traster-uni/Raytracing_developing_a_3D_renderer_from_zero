#include <iostream>
#include <vector>
#include "r3df0_math.h"
#include "r3df0_image.h"
#include "r3df0_color.h"
using namespace std;
using namespace r3dfrom0;

int main(){
    vector<vector<pixel_f>> img = {};
    for (int i = 0; i < 256; i++){
        vector<pixel_f> row = {};
        for (int j = 0; j<256; j++){
            pixel_f p(double(i) / (255), double(j) / (255), 0.0);
            row.push_back(p);
//          cout << "(" << p.r << " " << p.g << " " << p.b << ")" << " ";
        }
//        cout << "\n" << endl;
        img.push_back(row);
    }
    ppm_out(img, "hello_world.ppm");
}