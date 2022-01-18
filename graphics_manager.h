#pragma once
#include <iostream>

#include <glm/glm.hpp>

#include "model.h"
#include "shader.h"
#include "joint_manager.h"
#include "camera.h"
#include "bar_manager.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

class GraphicsManager
{
private:
    Model steel_beam;
    Model load_body;
    Model load_head;
    Model steel_joint;

    Shader model_shader;
    Shader circle_shader;
    Shader beam_shader;

    unsigned int VAO1, VAO2, VAO3, VBO1, VBO2, VBO3, EBO2, EBO3;

    BarManager* bar_manager;
    JointManager* joint_manager;

    glm::mat4 steel_beam_transform;
    std::vector<glm::mat4> transforms;
    std::vector<glm::mat4> load_transforms;
    std::vector<glm::mat4> joint_transforms;
    std::vector<glm::vec4> colors;
public:
    Camera camera;

    GraphicsManager(JointManager* joint_manager_, BarManager* bar_manager_);
    void drawBeam(double currentTime);
    void draw2d(double currentFrame);
    void updateData(JointManager* _joint_manager, BarManager* bar_manager_);
    void updateTransforms();

    void getVAO1();
    void getVAO2();
    void getVAO3();
    void updateVAO1();
    void updateVAO2();
    void updateVAO3();
};

