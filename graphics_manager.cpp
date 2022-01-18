#include "graphics_manager.h"

GraphicsManager::GraphicsManager(JointManager* joint_manager_, BarManager* bar_manager_)
    : model_shader(Shader("../EEM_opengl/model_shader.vs", "../EEM_opengl/model_shader.fs")),
    circle_shader(Shader("../EEM_opengl/circle.vs", "../EEM_opengl/circle.fs")),
    beam_shader(Shader("../EEM_opengl/beam.vs", "../EEM_opengl/beam.fs")),
    steel_beam(Model("../models/steel_beam.obj")),
    load_body(Model("../models/load_body.obj")),
    load_head(Model("../models/load_head.obj")),
    steel_joint(Model("../models/steel_joint.obj")),
    camera(Camera()),
    bar_manager(bar_manager_),
    joint_manager(joint_manager_)
{
    model_shader.use();
    model_shader.SetMat4("projection", glm::perspective(45.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.3f, 300.0f));
    model_shader.SetMat4("view", glm::lookAt(glm::vec3(-8.0f, 2.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    model_shader.SetMat4("model", glm::mat4(1.0f));
    model_shader.SetVec3("viewPos", glm::vec3(-8.0f, 0.0f, 0.0f));

    getVAO1();
    getVAO2();
    getVAO3();
}

void GraphicsManager::updateData(JointManager* _joint_manager, BarManager* bar_manager_)
{
    updateTransforms();
}

void GraphicsManager::updateTransforms()
{
    transforms.clear();
    load_transforms.clear();
    joint_transforms.clear();
    colors.clear();

    glm::vec2 joint1, joint2;
    std::vector<glm::vec2> displaced_position = joint_manager->getDisplacedPositions();

    for (unsigned int i = 0; i < bar_manager->joint_indices.size(); i++)
    {
        joint1 = displaced_position[bar_manager->joint_indices[i][0]];
        joint2 = displaced_position[bar_manager->joint_indices[i][1]];

        joint1.y = SCREEN_HEIGHT - joint1.y;
        joint2.y = SCREEN_HEIGHT - joint2.y;

        //make it so the JointManager adhere to the local worldspace
        joint1 = joint1 / 20.0f;
        joint2 = joint2 / 20.0f;

        glm::vec2 mid_joint = (joint1 + joint2) / 2.0f;
        glm::vec2 vector = joint1 - joint2;

        float rad = glm::atan(vector[1] / vector[0]);
        float size = glm::length(joint1 - joint2);
        float stretch = abs(1 - bar_manager->stretch[i]) * 30;

        steel_beam_transform = glm::mat4(1.0f);
        steel_beam_transform = glm::translate(steel_beam_transform, glm::vec3(0.0f, mid_joint[1] - 20.0f, mid_joint[0] - 30.0f));
        //steel_beam_transform = glm::rotate(steel_beam_transform, (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));

        steel_beam_transform = glm::rotate(steel_beam_transform, rad, glm::vec3(-1.0f, 0.0f, 0.0f));
        steel_beam_transform = glm::scale(steel_beam_transform, glm::vec3(1.0f, 1.0f, size / 2));
        transforms.push_back(steel_beam_transform);
        colors.push_back(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f) * (1 - stretch) + glm::vec4(0.8f, 0.2f, 0.2f, 1.0f) * stretch);

        steel_beam_transform = glm::translate(steel_beam_transform, glm::vec3(5.0f, 0.0f, 0.0f));
        transforms.push_back(steel_beam_transform);
        colors.push_back(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f) * (1 - stretch) + glm::vec4(0.8f, 0.2f, 0.2f, 1.0f) * stretch);
    }
    
    //in this loop the transforms for the steel beam are calculated
    //we also add transforms for the steel joints
    for (unsigned int i = 0; i < joint_manager->positions.size(); i++)
    {
        joint1 = displaced_position[i];

        //invert the joint so it adheres to screen space
        joint1.y = SCREEN_HEIGHT - joint1.y;

        joint1 = joint1 / 20.0f;

        //calculate the transforms for the joints
        glm::mat4 joint_transform = glm::mat4(1.0f);
        joint_transform = glm::translate(joint_transform, glm::vec3(0.0f, joint1[1] - 20.0f, joint1[0] - 30.0f));
        joint_transforms.push_back(joint_transform);
        joint_transform = glm::translate(joint_transform, glm::vec3(5.0f, 0.0f, 0.0f));
        joint_transform = glm::rotate(joint_transform, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        joint_transforms.push_back(joint_transform);

        //calculate the transforms for the connection beams
        steel_beam_transform = glm::mat4(1.0f);
        steel_beam_transform = glm::translate(steel_beam_transform, glm::vec3(2.5f, joint1[1] - 20.0f, joint1[0] - 30.0f));
        steel_beam_transform = glm::rotate(steel_beam_transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        steel_beam_transform = glm::scale(steel_beam_transform, glm::vec3(1.0f, 1.0f, 5.0f / 2));
        transforms.push_back(steel_beam_transform);

        //decide on the colors for anchors and the beams, color depends on the joint being an anchor or not
        if (!joint_manager->anchors[i])
            colors.push_back(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));  //glm::vec4(0.6f, 0.6f, 0.4f, 1.0f)
        else
            colors.push_back(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));  //glm::vec4(0.6f, 0.6f, 0.4f, 1.0f)
    }

    //calculate the transforms for the loads
    glm::mat4 load_transform;
    glm::mat4 head_transform;
    for (int i = 0; i < joint_manager->loads.size(); i++)
    {
        if ((int)joint_manager->loads[i].x != 0 or (int)joint_manager->loads[i].y != 0)
        {
            joint1 = displaced_position[i];
            joint2 = joint1 + glm::vec2(joint_manager->loads[i].x / 1e4, joint_manager->loads[i].y / 1e4);

            joint1.y = SCREEN_HEIGHT - joint1.y;
            joint2.y = SCREEN_HEIGHT - joint2.y;

            joint1 = joint1 / 20.0f;
            joint2 = joint2 / 20.0f;

            glm::vec2 mid_joint = (joint1 + joint2) / 2.0f;
            glm::vec2 vector = joint1 - joint2;

            float rad = glm::atan(vector[1] / vector[0]) + glm::radians(180.0f) * (vector[0] >= 0);
            float size = glm::length(joint1 - joint2);

            load_transform = glm::mat4(1.0f);
            head_transform = glm::mat4(1.0f);
            load_transform = glm::translate(load_transform, glm::vec3(0.0f, mid_joint[1] - 20.0f, mid_joint[0] - 30.0f));
            head_transform = glm::translate(head_transform, glm::vec3(0.0f, joint2[1] - 20.0f, joint2[0] - 30.0f));

            load_transform = glm::rotate(load_transform, rad, glm::vec3(-1.0f, 0.0f, 0.0f));
            head_transform = glm::rotate(head_transform, rad, glm::vec3(-1.0f, 0.0f, 0.0f));
            
            load_transform = glm::translate(load_transform, glm::vec3(2.5f, 0.0f, 0.0f));
            head_transform = glm::translate(head_transform, glm::vec3(2.5f, 0.0f, 0.0f));
            load_transform = glm::scale(load_transform, glm::vec3(1.0f, 1.0f, size / 2));
            load_transforms.push_back(load_transform);
            load_transforms.push_back(head_transform);
        }   
    }
}

void GraphicsManager::drawBeam(double currentTime)
{
    model_shader.use();
    model_shader.SetMat4("view", camera.GetViewMatrix());

    for (unsigned int i = 0; i < transforms.size(); i++)
    {
        model_shader.SetMat4("model", transforms[i]);
        steel_beam.ChangeColor(colors[i]);
        steel_beam.Draw(model_shader);
        steel_beam.ChangeColor(glm::vec4(0.044627f, 0.058888f, 0.063708f, 1.0f));
    }

    for (unsigned int i = 0; i < joint_transforms.size(); i++)
    {
        model_shader.SetMat4("model", joint_transforms[i]);
        //steel_joint.ChangeColor(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)); bar_manager->joint_indices.size() * 2
        steel_joint.ChangeColor(colors[i/2 + (bar_manager->joint_indices.size()) * 2]);
        steel_joint.Draw(model_shader);
    }

    for (unsigned int i = 0; i < load_transforms.size() / 2; i++)
    {
        model_shader.SetMat4("model", load_transforms[i * 2]);
        load_body.Draw(model_shader);
        model_shader.SetMat4("model", load_transforms[i * 2 + 1]);
        load_head.Draw(model_shader);
    }
}

void GraphicsManager::draw2d(double currentFrame)
{
    updateVAO1();
    updateVAO2();
    updateVAO3();

    beam_shader.use();

    glBindVertexArray(VAO2);
    beam_shader.SetVec3("color", glm::vec3(0.7f, 0.8f, 0.14f));
    glDrawElements(GL_LINES, bar_manager->joint_indices.size() * 2, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO3);
    beam_shader.SetVec3("color", glm::vec3(0.047274f, 0.296477f, 0.800000f));
    glDrawArrays(GL_LINES, 0, joint_manager->getJointLoadSet().size());

    circle_shader.use();
    circle_shader.SetFloat("time", currentFrame / 30);
    circle_shader.SetInt("selected", joint_manager->index_selected);
    glBindVertexArray(VAO1);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, joint_manager->positions.size());
    glBindVertexArray(0);
}

void GraphicsManager::getVAO1()
{
    float vertices[] =
    {
        0.05f, 0.05f, 0.0f, //top-right
        -0.05f, 0.05f, 0.0f, //top-left
        0.05f, -0.05f, 0.0f, //bottom-right

        -0.05f, 0.05f, 0.0f, //top-left
        -0.05f, -0.05f, 0.0f, //bottom-left
        0.05f, -0.05f, 0.0f, //bottom-right
    };

    glGenBuffers(1, &VBO1);
    updateVAO1();

    // create buffers/arrays
    unsigned int VBO;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}

void GraphicsManager::getVAO2()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    // set the vertex attribute pointers
    // vertex Positions
    updateVAO2();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void GraphicsManager::getVAO3()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    // set the vertex attribute pointers
    // vertex Positions
    updateVAO3();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void GraphicsManager::updateVAO1()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    std::vector<glm::vec3> buffer_data = joint_manager->bufferData();
    if (buffer_data.size() > 0)
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * buffer_data.size(), &buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void GraphicsManager::updateVAO2()
{
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, joint_manager->positions.size() * sizeof(glm::vec2), &joint_manager->positions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    if (bar_manager->joint_indices.size() > 0)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bar_manager->joint_indices.size() * sizeof(glm::ivec2), &bar_manager->joint_indices[0], GL_STATIC_DRAW);
}

void GraphicsManager::updateVAO3()
{
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    std::vector<glm::vec2> joint_load_set = joint_manager->getJointLoadSet();
    if (joint_load_set.size() > 0)
        glBufferData(GL_ARRAY_BUFFER, joint_load_set.size() * sizeof(glm::vec2), &joint_load_set[0], GL_STATIC_DRAW);
}