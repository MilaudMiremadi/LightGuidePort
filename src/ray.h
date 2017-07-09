#ifndef RAY_H
#define RAY_H
#include <glm/glm.hpp>

namespace rtr {
    
    class ray {
        public:
            glm::vec3 orig;
            glm::vec3 dir;

            ray() {
                
            }
            
            /**
             * Create a ray with the given origin and direction
             */
            ray(const glm::vec3 ori, const glm::vec3 dr);

            /**
             * Copy constructor
             */
            ray(const ray& r);

            ~ray() {
                
            }
            
            /**
             * Find the vector at the given distance 't' along this ray
             */
            inline glm::vec3 vec_at_t(float t) const {
                return glm::vec3(orig + (dir * t));
            }
            
            /**
             * Assignment operator
             */
            void operator=(const ray& r);
    };

}

#endif /* RAY_H_ */
