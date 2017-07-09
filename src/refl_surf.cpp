#include "refl_surf.h"

namespace rtr {
    
    refl_surf::refl_surf(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
        set_from_points(a, b, c, d);
    }
    
    refl_surf::refl_surf(const refl_surf& r) {
        p1 = r.p1;
        p2 = r.p2;
        p3 = r.p3;
        p4 = r.p4;
        pl = r.pl;
        t = r.t;
        x_max = r.x_max;
        x_min = r.x_min;
        y_max = r.y_max;
        y_min = r.y_min;
        z_max = r.z_max;
        z_min = r.z_min;
    }
    
    void refl_surf::set_from_points(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
        p1 = a;
        p2 = b;
        p3 = c;
        p4 = d;
        pl.A = p2.y * p3.z - p2.y * p1.z - p1.y * p3.z - p2.z * p3.y + p2.z * p1.y + p1.z * p3.y;
        pl.B = p2.x * p1.z + p2.z * p3.x - p2.z * p1.x - p1.z * p3.x - p2.x * p3.z + p3.z * p1.x;
        pl.C = p2.y * p1.x + p2.x * p3.y - p2.x * p1.y - p2.y * p3.x + p1.y * p3.x - p3.y * p1.x;
        pl.D = p2.x * p3.z * p1.y - p2.y * p3.z * p1.x - p2.x * p3.y * p1.z + p2.y * p3.x * p1.z + p2.z * p3.y * p1.x - p2.z * p3.x * p1.y;
        t = 0;
        x_max = p1.x;
        x_min = p1.x;
        y_max = p1.y;
        y_min = p1.y;
        z_max = p1.z;
        z_min = p1.z;
        
        if (p2.x > x_max)
            x_max = p2.x;
        if (p2.x < x_min)
            x_min = p2.x;
        if (p3.x > x_max)
            x_max = p3.x;
        if (p3.x < x_min)
            x_min = p3.x;
        if (p4.x > x_max)
            x_max = p4.x;
        if (p4.x < x_min)
            x_min = p4.x;
        
        if (p2.y > y_max)
            y_max = p2.y;
        if (p2.y < y_min)
            y_min = p2.y;
        if (p3.y > y_max)
            y_max = p3.y;
        if (p3.y < y_min)
            y_min = p3.y;
        if (p4.y > y_max)
            y_max = p4.y;
        if (p4.y < y_min)
            y_min = p4.y;
        
        if (p2.z > z_max)
            z_max = p2.z;
        if (p2.z < z_min)
            z_min = p2.z;
        if (p3.z > z_max)
            z_max = p3.z;
        if (p3.z < z_min)
            z_min = p3.z;
        if (p4.z > z_max)
            z_max = p4.z;
        if (p4.z < z_min)
            z_min = p4.z;
    }
    
    bool refl_surf::ray_plane_intersect(const ray& ray, glm::vec3& out) {
        float denom = (pl.A * ray.dir.x + pl.B * ray.dir.y + pl.C * ray.dir.z);
        if (denom == 0) {
            // ray does not intersect plane
            return false;
        }
        t = -(pl.A * ray.orig.x + pl.B * ray.orig.y + pl.C * ray.orig.z + pl.D) / denom;
        out = glm::vec3(ray.vec_at_t(t));
        return true;
    }
    
    void refl_surf::calc_t(const ray& ray) {
        float denom = (pl.A * ray.dir.x + pl.B * ray.dir.y + pl.C * ray.dir.z);
        if (denom == 0) {
            // ray does not intersect plane
            return;
        }
        t = -(pl.A * ray.orig.x + pl.B * ray.orig.y + pl.C * ray.orig.z + pl.D) / denom;
    }
    
    bool refl_surf::inside_surf(const glm::vec3& vec) const {
        float eps = 1e-4f;
        
        if (vec.x < x_min) {
            if (std::abs(vec.x - x_min) > eps) {
                return false;
            }
        }
        if (vec.x > x_max) {
            if (std::abs(vec.x - x_max) > eps) {
                return false;
            }
        }
        if (vec.y < y_min) {
            if (std::abs(vec.y - y_min) > eps) {
                return false;
            }
        }
        if (vec.y > y_max) {
            if (std::abs(vec.y - y_max) > eps) {
                return false;
            }
        }
        if (vec.z < z_min) {
            if (std::abs(vec.z - z_min) > eps) {
                return false;
            }
        }
        if (vec.z > z_max) {
            if (std::abs(vec.z - z_max) > eps) {
                return false;
            }
        }
        
        return true;
    }
    
    void refl_surf::operator=(const refl_surf& r) {
        p1 = r.p1;
        p2 = r.p2;
        p3 = r.p3;
        p4 = r.p4;
        pl = r.pl;
        t = r.t;
        x_max = r.x_max;
        x_min = r.x_min;
        y_max = r.y_max;
        y_min = r.y_min;
        z_max = r.z_max;
        z_min = r.z_min;
    }

}
