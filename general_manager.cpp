#include "general_manager.h"

bool click_button(glm::vec2 cursor_pos, glm::vec2 button_pos, glm::vec2 button_dims);

GeneralManager::GeneralManager(GLFWwindow* window)
    : window(window), cursor(Cursor()),
    joint_manager(new JointManager()),
    bar_manager(new BarManager()),
    //graphics_manager(new GraphicsManager(joint_manager, bar_manager)),
    material_manager(new MaterialManager())
    //simulation_manager(new SimulationManager())
{}

void GeneralManager::mainLoop()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    joint_manager->addJoint(200, 500);
    joint_manager->addJoint(300, 300);
    bar_manager->addIndices(0, 1);

    graphics_manager = new GraphicsManager(joint_manager, bar_manager);
    SpriteRenderer* sprite_renderer = new SpriteRenderer();
    Texture2D texture = Texture2D("../images/GUI.png", true);
    Texture2D simulate_button = Texture2D("../images/Simulate_Button.png", true);

    updateMouse(true);

    double currentFrame, deltaTime, lastFrame = 0;

    while (!glfwWindowShouldClose(window))
    {
        //-----------------------------------
        //          handle time
        //-----------------------------------
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //-----------------------------------
        //             drawing
        //-----------------------------------
        glClearColor(0.45f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        switch (draw_mode)
        {
        case BUILD:
            graphics_manager->draw2d(currentFrame);
            break;
        case SHOW:
            if (switched)
            {
                graphics_manager->updateData(joint_manager, bar_manager);
                switched = false;
            }
            if (animate > 0)
            {
                joint_manager->setAnimate(200 - animate);
                bar_manager->calculateStretch(joint_manager);
                graphics_manager->updateTransforms();
                animate -= 1;
            }
            //if (animate == 1)
            //    animate = 200;
                
            graphics_manager->drawBeam(currentFrame);
            //sprite_renderer->DrawSprite(simulate_button, glm::vec2(10.0f, 170.0f), glm::vec2(145.0, 64.0), 0.0f);
            break;
        default:
            break;
        }
        
        //sprite_renderer->DrawSprite(texture, glm::vec2(0.0f, 0.0f), glm::vec2(1200.0, 800.0), 0.0f);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GeneralManager::simulate()
{
    animate = 200;

    std::cout << "doing a simulation" << std::endl;
    std::cout << "number of JointManager in simulation: " << joint_manager->positions.size() << std::endl;
    std::cout << "number of anchors in simulation: " << joint_manager->anchors.size() << std::endl;

    std::vector<unsigned int> anchors_indices;
    int iterator = 0;
    for (bool anchor : joint_manager->anchors)
    {
        if (anchor)
        {
            anchors_indices.push_back(iterator);
        }
        iterator++;
    }

    std::vector<glm::vec2> displacements = FEA(joint_manager, bar_manager, anchors_indices, joint_manager->loads);

    std::cout << "resulting displacements" << std::endl;
    for (int i = 0; i < displacements.size(); i++)
    {
        std::cout << "x: " << displacements[i].x << "    y: " << displacements[i].y << std::endl;
        joint_manager->displacements[i] = displacements[i];
    }

    bar_manager->calculateStretch(joint_manager);

    graphics_manager->updateData(joint_manager, bar_manager);
}

void GeneralManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action)
    {
        if (key == 67)
        {
            //joint_manager->reset()
        }

        if (key == 82)
        {
            for (int i = 0; i < joint_manager->displacements.size(); i++)
            {
                joint_manager->displacements[i] = glm::vec2(0.0, 0.0);
            }
            bar_manager->calculateStretch(joint_manager);
            graphics_manager->updateData(joint_manager, bar_manager);
        }

        if (key == 84)
        {
            draw_mode = (drawMode)!draw_mode;
            switched = true;
        }

        if (key == 81)
        {
            build_mode = (buildMode)!build_mode;
        }

        if (key == 83)
        {
            simulate();
        }
    }
}

void GeneralManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (draw_mode)
    {
    case BUILD:
        switch (build_mode)
        {
        case BARS:
            if (action && button == 0)
            {
                if (selected != -1)
                {
                    if (joint_manager->clickJoint(cursor.xpos, cursor.ypos))
                    {
                        bar_manager->addIndices(joint_manager->index_selected, selected);
                        selected = joint_manager->index_selected;
                    }
                    else
                    {
                        joint_manager->addJoint(cursor.xpos, cursor.ypos);
                        bar_manager->addIndices(joint_manager->positions.size() - 1, selected);
                        selected = joint_manager->positions.size() - 1;
                    }
                }
                if (joint_manager->clickJoint(cursor.xpos, cursor.ypos))
                    selected = joint_manager->index_selected;
                else
                    selected = joint_manager->positions.size() - 1;
            }

            if (action && button == 1)
                selected = -1;

            if (action && button == 2)
            {
                joint_manager->setAnchor(cursor.xpos, cursor.ypos);
            }
            break;
        case LOADS:
            if (action && button == 0)
            {
                if (joint_manager->clickJoint(cursor.xpos, cursor.ypos))
                    selected = joint_manager->index_selected;
                else
                    joint_manager->addLoad((cursor.xpos - joint_manager->positions[joint_manager->index_selected].x) * 5e4, (cursor.ypos - joint_manager->positions[joint_manager->index_selected].y) * 5e4);
            }
            if (action && button == 0)
            {
                if (joint_manager->clickJoint(cursor.xpos, cursor.ypos))
                    joint_manager->addLoad(0.0, 0.0);
            }
            break;

        default:
            break;
        }
        break;
        
    case SHOW:
        if (button == 1)
        {
            if (action == 1)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                updateMouse(false);
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                updateMouse(true);
            }
        }
        if (button == 0 && action == 1 && click_button(glm::vec2(cursor.xpos, cursor.ypos), glm::vec2(10.0f, 170.0f), glm::vec2(145.0, 64.0)))
        {
            simulate();
        }
        if (button == 0)
        {
            move_cam = (action == 1);
        }

        cameraMouseCallback(window, cursor.xpos, cursor.ypos);
        break;
    default:
        break;
    }
}

void GeneralManager::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    graphics_manager->camera.ProcessMouseScroll(yoffset);
}

void GeneralManager::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    cursor.xpos = xpos;
    cursor.ypos = ypos;

    cursor.xoffset = xpos - cursor.lastX;
    cursor.yoffset = ypos - cursor.lastY;

    cursor.lastX = xpos;
    cursor.lastY = ypos;

    switch (draw_mode)
    {
    case BUILD:
        break;
    case SHOW:
        if (move_cam)
        {
            graphics_manager->camera.MidPosition.y += cursor.yoffset * 0.03;
            graphics_manager->camera.MidPosition.z -= cursor.xoffset * cos(graphics_manager->camera.yaw_rad) * 0.03;
            graphics_manager->camera.updateCameraVectors();
        }
    default:
        break;
    }

    cameraMouseCallback(window, xpos, ypos);
}

void GeneralManager::updateMouse(bool _mouse_hidden)
{
    mouse_hidden = _mouse_hidden;
    first_mouse = !_mouse_hidden;
}

void GeneralManager::cameraMouseCallback(GLFWwindow* window, double _xpos, double _ypos)
{
    double xpos = _xpos;
    double ypos = _ypos;
    if (!mouse_hidden)
    {
        if (first_mouse)
        {
            LastX = _xpos;
            LastY = _ypos;
            first_mouse = false;
        }

        float xoffset = _xpos - LastX;
        float yoffset = LastY - _ypos; // reversed since y-coordinates go from bottom to top

        LastX = _xpos;
        LastY = _ypos;

        graphics_manager->camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

bool click_button(glm::vec2 cursor_pos, glm::vec2 button_pos, glm::vec2 button_dims)
{
    if ((cursor_pos.x > button_pos.x && cursor_pos.x < (button_pos.x + button_dims.x)) &&
        (cursor_pos.y > button_pos.y && cursor_pos.y < (button_pos.y + button_dims.y)))
        return true;
    else
        return false;
}