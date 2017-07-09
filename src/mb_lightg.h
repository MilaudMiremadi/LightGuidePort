#ifndef MB_LIGHTG_H
#define MB_LIGHTG_H

#include <vector>
#include "ray.h"
#include "refl_surf.h"
#include "model3d.h"

namespace rtr {
    
    // max number of ray bounces
    extern const float MAX_ITER;
    
    // dimensions
    extern const float DEPTH;
    extern const float WIDTH_A;
    extern const float WIDTH_B;
    
    /**
     * Represents a light guide model
     * Extends model3d
     */
    class mb_lightg: public model3d {
            
        public:
            
            mb_lightg();

            ~mb_lightg() {
                
            }
            
            /**
             * Traces a ray through the light guide
             * param 'r' - start ray
             * param 'out' - output ray
             * return true if ray exited
             */
            bool find_reflected_ray(const ray& r, ray& out);

        private:
            
            /**
             * Number of sides in the 3d model
             */
            int n_sides;

            /**
             * Array of reflective surfaces in the light guide
             * first element is front surface last element is exit surface
             */
            std::vector<refl_surf> surf_list;

            /**
             * Number of reflections
             */
            int refl_count;
            
    };

}

#endif /* MB_LIGHTG_H_ */
