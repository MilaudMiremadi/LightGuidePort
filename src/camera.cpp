#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace rtr {
    
    const float XROT = 0.0f;
    const float YROT = -90.0f;
    const float MOVE_SPEED = 2.5f;
    const float LOOK_SPEED = 0.1f;
    const float FOVY = 45.0f;
    
    camera::camera(glm::vec3 position, glm::vec3 up, float theta_x, float theta_y) {
        forward = glm::vec3(0.0f, 0.0f, -1.0f);
        move_speed = MOVE_SPEED;
        look_speed = LOOK_SPEED;
        fovy = FOVY;
        pos = position;
        world_up = up;
        x_rot = theta_x;
        y_rot = theta_y;
        update_direction_vec();
    }
    
    glm::mat4 camera::calc_view_matrix() const {
        return glm::lookAt(pos, pos + forward, up);
    }
    
    void camera::keyboard(Direction direction, float dt) {
        float vel = move_speed * dt;
        switch (direction) {
            case FORWARD:
                pos += forward * vel;
                break;
            case BACKWARD:
                pos -= forward * vel;
                break;
            case LEFT:
                pos -= right * vel;
                break;
            case RIGHT:
                pos += right * vel;
                break;
            default:
                break;
        }
    }
    
    void camera::mouse_motion(float xoff, float yoff) {
        xoff *= look_speed;
        yoff *= look_speed;
        
        x_rot += yoff;
        y_rot += xoff;
        
        if (x_rot > 89.0f) {
            x_rot = 89.0f;
        }
        if (x_rot < -89.0f) {
            x_rot = -89.0f;
        }
        update_direction_vec();
    }
    
    void camera::mouse_wheel(float yoff) {
        // Nothing
    }
    
    void camera::update_direction_vec() {
        glm::vec3 dir;
        dir.x = cos(glm::radians(y_rot)) * cos(glm::radians(x_rot));
        dir.y = sin(glm::radians(x_rot));
        dir.z = sin(glm::radians(y_rot)) * cos(glm::radians(x_rot));
        forward = glm::normalize(dir);
        right = glm::normalize(glm::cross(forward, world_up));
        up = glm::normalize(glm::cross(right, forward));
    }
}
