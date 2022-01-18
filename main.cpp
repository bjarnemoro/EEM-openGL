#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "general_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*
----------------------
       STYLE
----------------------
class:
    class MyClass
    method myMethod
    member my_member

global:
    function myFunction
    variable my_variable
*/

GLFWwindow* create_window();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

extern int SCREEN_WIDTH = 1200;
extern int SCREEN_HEIGHT = 800;

GeneralManager* general_manager;

int main()
{
    GLFWwindow* window = create_window();

    general_manager = new GeneralManager(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    general_manager->mainLoop();

    return 0;
}

GLFWwindow* create_window()
{
    glfwInit();
    //glfwWindowHint(GLFW_SAMPLES, 9);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FEM simulation", 0, 0);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    return window;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    general_manager->keyCallback(window, key, scancode, action, mode);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    general_manager->mouseButtonCallback(window, button, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    general_manager->cursorPosCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    general_manager->scrollCallback(window, xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}