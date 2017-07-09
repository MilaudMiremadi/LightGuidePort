#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

namespace rtr {
    
    /**
     * Movement directions
     */
    enum Direction {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };
    
    // Defaults
    extern const float XROT;
    extern const float YROT;
    extern const float MOVE_SPEED;
    extern const float LOOK_SPEED;
    extern const float FOVY;
    
    /**
     * Represents a camera in 3D space
     */
    class camera {
        public:
            
            // Position and orientation
            glm::vec3 pos;
            glm::vec3 forward;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 world_up;

            // View rotation
            float x_rot;
            float y_rot;

            // Settings
            float move_speed;
            float look_speed;
            float fovy;

            /**
             * Creates a camera at the given position with the given up vector rotated by the given x and y rotation
             */
            camera(glm::vec3 position, glm::vec3 up = glm::vec3(0, 1, 0), float theta_x = XROT, float theta_y = YROT);

            /**
             * Calculates view matrix
             */
            glm::mat4 calc_view_matrix() const;

            /**
             * Process keyboard
             */
            void keyboard(Direction direction, float dt);

            /**
             * Process mouse motion
             */
            void mouse_motion(float xoff, float yoff);

            /**
             * Process mouse wheel
             */
            void mouse_wheel(float yoff);

        private:
            /**
             * Find direction/forward vector
             */
            void update_direction_vec();
    };
}
#endif
