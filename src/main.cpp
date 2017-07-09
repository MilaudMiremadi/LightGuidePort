#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "common.h"

#include <stdio.h>
#define GLM_FORCE_PURE
#define GLM_FORCE_RADIANS
#define GLM_MESSAGES
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "mb_lightg.h"

using namespace std;

namespace mainspace {
    
    const float VW = 800;
    const float VH = 600;
    char key_once[GLFW_KEY_LAST + 1];
    
    GLenum glCheckError_(const char *file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            string error;
            switch (errorCode) {
                case GL_INVALID_ENUM:
                    error = "INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    error = "INVALID_VALUE";
                    break;
                case GL_INVALID_OPERATION:
                    error = "INVALID_OPERATION";
                    break;
                case GL_STACK_OVERFLOW:
                    error = "STACK_OVERFLOW";
                    break;
                case GL_STACK_UNDERFLOW:
                    error = "STACK_UNDERFLOW";
                    break;
                case GL_OUT_OF_MEMORY:
                    error = "OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    error = "INVALID_FRAMEBUFFER_OPERATION";
                    break;
            }
            cout << error << " | " << file << " (" << line << ")" << endl;
        }
        return errorCode;
    }
    
#define glCheckError() glCheckError_(__FILE__, __LINE__)
    
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
    (key_once[KEY] ? false : (key_once[KEY] = true)) :   \
    (key_once[KEY] = false))
    
    void init(void);
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoff, double yoff);
    void update(GLFWwindow * window);
    void render(void);
    void cleanup(void);
    
    rtr::camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    rtr::mb_lightg lg;
    
    float prev_x = VW / 2.0f;
    float prev_y = VH / 2.0f;
    bool mouse_init = true;
    bool mouse_free = false;
    
    rtr::shader * program;
    
    float dt = 0.0f;
    float last = 0.0f;
    
    int main() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac
                
        GLFWwindow * window = glfwCreateWindow(VW, VH, "Test", NULL, NULL);
        if (window == NULL) {
            cout << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return EXIT_FAILURE;
        }
        
        glfwMakeContextCurrent(window);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        if (glewInit() != GLEW_OK) {
            throw runtime_error("glewInit failed");
        }
        printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
        
        while (glGetError() != GL_NO_ERROR) {
            
        }
        
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        
        glClearColor(0, 0, 0, 1);
        init();
        while (!glfwWindowShouldClose(window)) {
            float cur = glfwGetTime();
            dt = cur - last;
            last = cur;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            update(window);
            render();
            
            glfwSwapBuffers(window);
            glfwPollEvents();
            
        }
        
        glfwTerminate();
        cleanup();
        return EXIT_SUCCESS;
    }
    
    void init(void) {
        glEnable(GL_DEPTH_TEST);
        rtr::ray r(glm::vec3(0, 0, 0), glm::normalize(glm::vec3(0.002f, 0.04f, 0.01f)));
        rtr::ray return_ray;
        bool success = lg.find_reflected_ray(r, return_ray);
        if (success) {
            cout << return_ray.orig.x << return_ray.orig.y << return_ray.orig.z << endl;
        }
        lg.setup_arrays();
        program = new rtr::shader("resources/vert.vs", "resources/frag.fs");
    }
    
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    
    void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        if (mouse_init) {
            prev_x = xpos;
            prev_y = ypos;
            mouse_init = false;
        }
        
        float xoff = xpos - prev_x;
        float yoff = prev_y - ypos;
        
        prev_x = xpos;
        prev_y = ypos;
        if (!mouse_free) {
            camera.mouse_motion(xoff, yoff);
        }
    }
    
    void scroll_callback(GLFWwindow* window, double xoff, double yoff) {
        camera.mouse_wheel(yoff);
    }
    
    void update(GLFWwindow * window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKeyOnce(window, GLFW_KEY_1)) {
            mouse_free = !mouse_free;
            glfwSetInputMode(window, GLFW_CURSOR, mouse_free ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.keyboard(rtr::FORWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.keyboard(rtr::BACKWARD, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.keyboard(rtr::LEFT, dt);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.keyboard(rtr::RIGHT, dt);
        }
    }
    
    void render(void) {
        {
            glm::mat4 view;
            view = camera.calc_view_matrix();
            
            program->uni_mat4("view", view);
            
            glm::mat4 proj;
            proj = glm::perspective(glm::radians(camera.fovy), VW / VH, 0.1f, 100.0f);
            program->uni_mat4("proj", proj);
        }
        
        program->use();
        
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(-(lg.xmin + lg.xmax) / 2, -(lg.ymin + lg.ymax) / 2, -(lg.zmin + lg.zmax) / 2));
        
        program->uni_mat4("model", model);
        lg.render();
        glCheckError();
    }
    
    void cleanup(void) {
        SAFE_DELETE(program);
    }
}

int main() {
    try {
        return mainspace::main();
    } catch (exception &e) {
        cerr << e.what();
    }
    return EXIT_FAILURE;
}
