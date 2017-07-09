#include "ray.h"

namespace rtr {
    
    ray::ray(const glm::vec3 ori, const glm::vec3 dr) {
        orig = ori;
        dir = dr;
    }
    
    ray::ray(const ray& r) {
        orig = r.orig;
        dir = r.dir;
    }
    
    void ray::operator=(const ray& r) {
        orig = r.orig;
        dir = r.dir;
    }

}
