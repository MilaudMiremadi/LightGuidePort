#ifndef REFL_SURF_H
#define REFL_SURF_H

#include <glm/glm.hpp>
#include "ray.h"
#include <stdio.h>

namespace rtr {
    
    /**
     * Represents a four cornered reflective surface
     */
    class refl_surf {
        public:
            refl_surf() {
                
            }
            
            /**
             * Create a new reflective surface with the given vertices
             */
            refl_surf(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);

            /**
             * Copy constructor
             */
            refl_surf(const refl_surf& r);

            ~refl_surf() {
                
            }
            
            /**
             * Recreate the surface to represent the given vertices
             */
            void set_from_points(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);

            /**
             * Finds the intersection of a ray and the plane of this surface
             * Returns the value in 'out' and sets the 't' variable in the object
             */
            bool ray_plane_intersect(const ray& ray, glm::vec3& out);

            /**
             * Finds the intersection of a ray and the plane of this surface
             * Sets the 't' variable in the object
             */
            void calc_t(const ray& ray);

            /**
             * Check if a position is inside of this surface
             */
            bool inside_surf(const glm::vec3& vec) const;

            /**
             * Assignment operator
             */
            void operator=(const refl_surf& r);

            glm::vec3 p1; // plane corner points
            glm::vec3 p2;
            glm::vec3 p3;
            glm::vec3 p4;
            struct plane_t {
                    float A; // plane equation constants
                    float B;
                    float C;
                    float D;

                    void operator=(const plane_t& p) {
                        A = p.A;
                        B = p.B;
                        C = p.C;
                        D = p.D;
                    }
            } pl;
            float t; // ray t value
            float x_max;
            float x_min;
            float y_max;
            float y_min;
            float z_max;
            float z_min;
            
    };

}

#endif /* REFL_SURF_H_ */
