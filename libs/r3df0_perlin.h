#ifndef R3DFROM0_R3DF0_PERLIN_H
#define R3DFROM0_R3DF0_PERLIN_H

#include "r3df0_varsutil.h"

using  namespace std;

namespace r3dfrom0{

    class perlin_noise {
        // FOR MORE INFORMATION: https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1/creating-simple-2D-noise.html

    public:
        perlin_noise() {
            for (int i = 0; i < p_count; i++) {
                rand_vec_array[i] = unit(vec3f().random(-1, 1)); // gradient

                perlin_permutations(perm_x);
                perlin_permutations(perm_y);
                perlin_permutations(perm_z);
            }
        }

        float noise(const vec3f& p) const {
            auto u = p.x - floor(p.x);
            auto v = p.y - floor(p.y);
            auto w = p.z - floor(p.z);

            auto i = int(floor(p.x));
            auto j = int(floor(p.y));
            auto k = int(floor(p.z));
            vec3f c[2][2][2];
            for(int ii = 0; ii < 2; ii++){
                for(int jj = 0; jj < 2; jj++){
                    for(int kk = 0; kk < 2; kk++){
                        c[ii][kk][jj] = rand_vec_array[
                                perm_x[i+ii & 255] ^
                                perm_y[j+jj & 255] ^
                                perm_z[k+kk & 255]
                        ];
                    }
                }
            }
            return trilinear_interpolation(c, u, v, w);
        }

        double turbolance(const vec3f& p, const int depth){
            float accumulation = .0f;
            auto temp_p = p;
            auto weight = 1.0f;

            for (int i = 0; i < depth; i++){
                accumulation += weight * noise(temp_p);
                weight *= .5f;
                temp_p *= 2;
            }
            return fabs(accumulation);
        }

    private:
        static const int p_count = 256;
        vec3f rand_vec_array[p_count]; // gradiant
        // Permutation components arrays
        int perm_x[p_count];        // permutations on x axis
        int perm_y[p_count];        // permutations on y axis
        int perm_z[p_count];        // permutations on z axis

        // private methods
        static void perlin_permutations(int* perm_array){
            for (int i = 0; i < p_count; i++){
                perm_array[i] = i;
            }
            scramble(perm_array);
        }

        static void scramble(int* perm_array){
            for (int i = p_count-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = perm_array[i];
                perm_array[i] = perm_array[target];
                perm_array[target] = tmp;
            }
        }

        static float trilinear_interpolation(const vec3f c[2][2][2], float u, float v, float w) {
            auto uu = smootherstep_interpolation(u);
            auto vv = smootherstep_interpolation(v);
            auto ww = smootherstep_interpolation(w);

            float accumulation = .0f;
            for (int i=0; i < 2; i++){
                for (int j=0; j < 2; j++) {
                    for (int k=0; k < 2; k++) {
                        auto weight_v = vec3f(u - i, v - j, w - k);
                        accumulation += (i * uu + (1-i) * (1-uu))
                                      * (j * vv + (1-j) * (1-vv))
                                      * (k * ww + (1-k) * (1-ww))
                                      * dot(unit(c[i][j][k]), weight_v);
                    }
                }
            }
            return accumulation;
        };
        // TODO: FIX PERLIN_INTERPOLATION FUNCTION, VISIBLE STEPS
    }; // perlin_noise class

} // namespace r3dfrom0
#endif //R3DFROM0_R3DF0_PERLIN_H
