#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "graphics_manager.h"
#include "material_manager.h"
#include "simulation_manager.h"

#include "joint_manager.h"
#include "bar_manager.h"

#include "sprite_renderer.h"

#include "FEM.h"

class Cursor
{
public:
    double xpos, ypos;
    double lastX = 0, lastY = 0;
    double xoffset, yoffset;
};

enum drawMode
{
    BUILD = 0,
    SHOW = 1
};

enum buildMode
{
    BARS = 0,
    LOADS = 1
};

/*
------------------------
          tasks
------------------------
 - manage all other managers
 - handle all incoming inputs
 - house the main loop
 - house global variables

------------------------
   important variables
------------------------

*/
class GeneralManager
{
private:
    GraphicsManager* graphics_manager;
    SimulationManager simulation_manager;
    MaterialManager* material_manager;

    GLFWwindow* window;

    Cursor cursor;

    JointManager* joint_manager;
    BarManager* bar_manager;

    drawMode draw_mode = BUILD;
    buildMode build_mode = BARS;
    bool switched = false;
    int selected;
    int animate = 0;

    bool mouse_hidden;
    bool first_mouse;
    double LastX, LastY = 0;

    bool move_cam = false;

public:
    GeneralManager(GLFWwindow* window);

    void mainLoop();
    void simulate();

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    void updateMouse(bool _mouse_hidden);
    void cameraMouseCallback(GLFWwindow* window, double _xpos, double _ypos);
};

