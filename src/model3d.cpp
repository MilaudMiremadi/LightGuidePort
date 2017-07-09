#include "model3d.h"
#include <GL/glew.h>

namespace rtr {
    static bool init = false;
    
    void model3d::add_vert(float x, float y, float z) {
        vert.push_back(x);
        vert.push_back(y);
        vert.push_back(z);
    }
    
    void model3d::add_vert(const glm::vec3& p) {
        add_vert(p.x, p.y, p.z);
    }
    
    void model3d::add(int p1, int p2, int color) {
        if (p1 >= nvert() || p2 >= nvert()) {
            return;
        }
        
        con.push_back((p1 << 17) | (p2 << 2) | (color & 0x3));
    }
    
    void model3d::add_relative(int p1, int p2, int color) {
        p1 += nvert();
        p2 += nvert();
        
        if (p1 >= nvert() || p2 >= nvert() || p1 < 0 || p2 < 0) {
            return;
        }
        
        con.push_back((p1 << 17) | (p2 << 2) | (color & 0x3));
    }
    
    void model3d::add_ray(const ray& ray, int color) {
        add_vert(ray.orig);
        glm::vec3 v(ray.dir);
        v += ray.orig;
        add_vert(v);
        add_relative(-2, -1, color);
    }
    
    void model3d::calc_aabb() {
        if (nvert() <= 0) {
            return;
        }
        std::vector<float> v = vert;
        float xMn = v[0], xMx = xMn;
        float yMn = v[1], yMx = yMn;
        float zMn = v[2], zMx = zMn;
        for (int i = nvert() * 3; (i -= 3) > 0;) {
            float x = v[i];
            if (x < xMn)
                xMn = x;
            if (x > xMx)
                xMx = x;
            float y = v[i + 1];
            if (y < yMn)
                yMn = y;
            if (y > yMx)
                yMx = y;
            float z = v[i + 2];
            if (z < zMn)
                zMn = z;
            if (z > zMx)
                zMx = z;
        }
        xmax = xMx;
        xmin = xMn;
        ymax = yMx;
        ymin = yMn;
        zmax = zMx;
        zmin = zMn;
    }
    
    float* model3d::cvbo(int& len) const {
        if (!init) {
            init = true;
            make_table();
        }
        len = nvert() * 6;
        float * gl = new float[len];
        int j = 0;
        int k = 0;
        for (int i = 0; i < len; i += 6) {
            gl[i + 0] = vert.at(j + 0);
            gl[i + 1] = vert.at(j + 1);
            gl[i + 2] = vert.at(j + 2);
            gl[i + 3] = table[con[k] & 0x3].r;
            gl[i + 4] = table[con[k] & 0x3].g;
            gl[i + 5] = table[con[k] & 0x3].b;
            
            j += 3;
            k++;
        }
        return gl;
    }
    
    ushort* model3d::cebo(int& len) const {
        if (!init) {
            init = true;
            make_table();
        }
        len = con.size() * 2;
        ushort * gl = new ushort[len];
        int j = 0;
        for (int i = 0; i < len; i += 2) {
            int k = con.at(j);
            gl[i + 0] = (k >> 17) & 0x7fff;
            gl[i + 1] = (k >> 2) & 0x7fff;
            j++;
        }
        return gl;
    }
    
    void model3d::setup_arrays() {
        int vlen = 0;
        int elen = 0;
        calc_aabb();
        
        float * vbo = cvbo(vlen);
        ushort * ebo = cebo(elen);
        printf("AABB: %f, %f, %f, <%f, %f, %f>\n", xmin, ymin, zmin, xmax, ymax, zmax);
        
        printf("%d, %d\n", vlen, elen);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vlen * sizeof(float), vbo, GL_STATIC_DRAW);
        
        int num = POS + COLOR;
        
        // position
        glVertexAttribPointer(0, POS, GL_FLOAT, GL_FALSE, num * sizeof(float), VOID_POINTER(0));
        glEnableVertexAttribArray(0);
        
        // color
        glVertexAttribPointer(1, COLOR, GL_FLOAT, GL_FALSE, num * sizeof(float), VOID_POINTER(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, elen * sizeof(ushort), ebo, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        SAFE_DELETEV(vbo);
        SAFE_DELETEV(ebo);

    }
    
    void model3d::render() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_LINES, con.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }

}
