#include "mb_lightg.h"

namespace rtr {
    
    const float MAX_ITER = 256;
    const float DEPTH = 3;
    const float WIDTH_A = 0.4f;
    const float WIDTH_B = 0.0093f;
    
    mb_lightg::mb_lightg() {
        glm::vec3 p1(WIDTH_A, 0, WIDTH_A);
        glm::vec3 p2(-WIDTH_A, 0, WIDTH_A);
        glm::vec3 p4(WIDTH_A, 0, -WIDTH_A);
        glm::vec3 p3(-WIDTH_A, 0, -WIDTH_A);
        glm::vec3 p5(WIDTH_B, DEPTH, WIDTH_B); // trapezoidal light pipe
        glm::vec3 p6(-WIDTH_B, DEPTH, WIDTH_B);
        glm::vec3 p7(WIDTH_B, DEPTH, -WIDTH_B);
        glm::vec3 p8(-WIDTH_B, DEPTH, -WIDTH_B);
        /*  glm::vec3 p5(0.01f, 0.1f, 0.01f); //square light pipe
         glm::vec3 p6(-0.01f, 0.1f, 0.01f);
         glm::vec3 p7(0.01f, 0.1f, -0.01f);
         glm::vec3 p8(-0.01f, 0.1f, -0.01f);*/

        add_vert(p1); //-8
        add_vert(p2); //-7
        add_vert(p3); //-6
        add_vert(p4); //-5
        add_vert(p5); //-4
        add_vert(p6); //-3
        add_vert(p7); //-2
        add_vert(p8); //-1
        add_relative(-8, -7, MAGENTA); //surface 0
        add_relative(-7, -6, MAGENTA);
        add_relative(-6, -5, MAGENTA);
        add_relative(-5, -8, MAGENTA);
        
        add_relative(-4, -3, MAGENTA); //surface 5
        add_relative(-3, -1, MAGENTA);
        add_relative(-1, -2, MAGENTA);
        add_relative(-2, -4, MAGENTA);
        
        add_relative(-8, -4, MAGENTA); //lines going from surface 0 to 5
        add_relative(-7, -3, MAGENTA);
        add_relative(-5, -2, MAGENTA);
        add_relative(-6, -1, MAGENTA);
        
        surf_list.reserve(6);
        surf_list.push_back(refl_surf(p1, p2, p3, p4));
        surf_list.push_back(refl_surf(p1, p5, p7, p4));
        surf_list.push_back(refl_surf(p7, p8, p3, p4));
        surf_list.push_back(refl_surf(p2, p3, p8, p6));
        surf_list.push_back(refl_surf(p1, p2, p6, p5));
        surf_list.push_back(refl_surf(p5, p6, p8, p7));
        
        n_sides = 6;
    }
    
    bool mb_lightg::find_reflected_ray(const ray& r, ray& out) {
        float t_min = FLT_MAX;
        int nearest_surf = 0;
        glm::vec3 nearest_intersection(0, 0, 0);
        int old_surf = 0;
        refl_count = 0;
        ray reflected = r;
        add_vert(reflected.orig);
        while ((nearest_surf != (n_sides - 1)) && (refl_count < MAX_ITER)) { //while hasn't reached exit face or too many iterations
            ray& incident = reflected;
            add_vert(incident.orig);
            add_relative(-1, -2, GREEN);
            refl_surf* rs;
            
            for (int n = 0; n < n_sides; n++) { //find minimum t at surface n
                if (n == old_surf) {
                    continue;
                }
                rs = &surf_list[n];
                glm::vec3 intersection;
                
                bool success = rs->ray_plane_intersect(incident, intersection);
                if (!success) {
                    continue;
                }
                if (rs->t <= 0) {
                    continue;
                }
                if (!rs->inside_surf(intersection)) {
                    continue;
                }
                if (rs->t < t_min) {
                    t_min = rs->t;
                    nearest_surf = n;
                    nearest_intersection = glm::vec3(intersection);
                }
            }
            if (t_min == FLT_MAX) {
                return false;
            } //no surface intersection
            old_surf = nearest_surf; // save so that on next pass we do not check this plane for intersection
                    
            t_min = FLT_MAX;
            if (nearest_surf != (n_sides - 1)) { // if incident is on rear surface then let ray go through                
                rs = &surf_list[nearest_surf];
                ray incidentNorm(glm::vec3(incident.orig.x, incident.orig.y, incident.orig.z), glm::vec3(rs->pl.A, rs->pl.B, rs->pl.C));
                rs->calc_t(incidentNorm);
                glm::vec3 mirror = incidentNorm.vec_at_t(2 * rs->t);
                glm::vec3 reflectedDir(nearest_intersection);
                reflectedDir -= mirror;
                
                reflected = ray(nearest_intersection, glm::normalize(reflectedDir));
            } else {
                incident.orig = nearest_intersection;
                add_vert(nearest_intersection); //draw exiting ray
                add_relative(-1, -2, GREEN);
                add_ray(incident, 1);
                out = incident;
                return true;
            }
            refl_count++;
        }
        
        if (refl_count == MAX_ITER) {
            return false;
        } // internal ray bounce 
        add_vert(reflected.orig);
        add_relative(-1, -2, YELLOW);
        out = reflected;
        return true;
    }

}
