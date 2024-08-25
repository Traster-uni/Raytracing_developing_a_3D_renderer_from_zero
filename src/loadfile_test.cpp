//
// Created by trast on 21/08/2024.
//

#include <vector>
#include "r3df0_varsutil.h"
#include "r3df0_image.h"
#include "r3df0_camera.h"
#include "r3df0_hittable.h"
#include "r3df0_shapes.h"
#include "r3df0_bvh.h"

using namespace r3dfrom0;
using namespace std;

int main(){
    int w;
    int h;

    auto img = load_img("earthmap.jpg", w, h);
    write_png("load_file_test.png", w, h, img);

    // TODO: READING THE IMAGE COOKS THE COLORS, LOOK AT load_file_test.png
}