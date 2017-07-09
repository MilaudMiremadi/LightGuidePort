#ifndef MODEL3D_H
#define MODEL3D_H

#include <vector>
#include <glm/glm.hpp>
#include "common.h"
#include "ray.h"

namespace rtr {
    
    /**
     * Enumeration of colors
     */
    enum Color {
        WHITE,
        MAGENTA,
        GREEN,
        YELLOW
    };
    
    /**
     * Table of colors
     */
    static glm::vec3 table[4];
    
    /**
     * Number of components in a position vector
     */
    static const int POS = 3;
    
    /**
     * Number of components in a color vector
     */
    static const int COLOR = 3;
    
    /**
     * Represents a 3d wireframe model
     */
    class model3d {
        public:
            
            /**
             * Empty constructor
             */
            model3d() {
                
            }
            
            /**
             * Empty destructor
             */
            ~model3d() {
                
            }
            
            /**
             * Adds a vertex
             */
            void add_vert(float x, float y, float z);

            /**
             * Adds a vertex
             */
            void add_vert(const glm::vec3& p);

            /**
             * Adds a connection with absolute indices and gives the connection a color
             */
            void add(int p1, int p2, int color);

            /**
             * Adds a connection given indices relative to the current index and gives the connection a color
             * E.G -1 would represent the vertex that was defined before the current one
             */
            void add_relative(int p1, int p2, int color);

            /**
             * Adds a ray to the model
             */
            void add_ray(const ray& ray, int color);

            /**
             * Calculates the axis aligned bounding box
             */
            void calc_aabb();

            /**
             * Sets up the OpenGL arrays and buffers
             */
            void setup_arrays();

            /**
             * Renders the model
             */
            void render() const;

            /**
             * Returns the number of vertices in the model
             */
            inline int nvert() const {
                return vert.size() / 3;
            }
            
            /**
             * Array of connection data
             * Each integer contains 3 pieces of information:
             * High 15 bits = index into vert array of first vertex
             * Next highest 15 bits = index into vert array of second vertex
             * Last 2 bits = index into color table
             */
            std::vector<int> con;
            float xmin, xmax, ymin, ymax, zmin, zmax;
        private:
            
            /**
             * OpenGL ID of this model's element buffer object
             */
            uint EBO;

            /**
             * OpenGL ID of this model's vertex array object
             */
            uint VAO;

            /**
             * OpenGL ID of this model's vertex buffer object
             */
            uint VBO;

            /**
             * Array of floats representing the X,Y,Z coordinates of each vertex
             * Layout: { v0x, v0y, v0z, v1x, v1y, v1z, ... }
             */
            std::vector<float> vert;

            /**
             * Create element buffer object (EBO) for the model
             */
            ushort * cebo(int& len) const;

            /**
             * Create vertex buffer object (VBO) for the model
             */
            float * cvbo(int& len) const;

            /**
             * Create color table
             */
            inline void make_table() const {
                table[WHITE] = glm::vec3(1, 1, 1);
                table[MAGENTA] = glm::vec3(1, 0, 1);
                table[GREEN] = glm::vec3(0, 1, 0);
                table[YELLOW] = glm::vec3(1, 1, 0);
            }
            
    };

}

#endif /* MODEL3D_H_ */
