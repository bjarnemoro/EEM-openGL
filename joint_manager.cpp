#include "joint_manager.h"

void JointManager::addJoint(double x, double y)
{
    positions.push_back(glm::vec2(x, y));
    loads.push_back(glm::vec2(0, 0));
    displacements.push_back(glm::vec2(0, 0));
    anchors.push_back(false);
}

void JointManager::addLoad(double x, double y)
{
    loads[index_selected] = glm::vec2(x, y);
}

std::vector<glm::vec2> JointManager::getDisplacedPositions()
{
    std::vector<glm::vec2> displaced_positions;
    for (int i = 0; i < positions.size(); i++)
    {
        displaced_positions.push_back(positions[i] + displacements[i] * (float)animate_progress);
    }

    return displaced_positions;
}

std::vector<glm::vec2> JointManager::getJointLoadSet()
{
    std::vector<glm::vec2> joint_load_set;
    for (int i = 0; i < positions.size(); i++)
    {
        if ((int)loads[i].x != 0 or (int)loads[i].y != 0)
        {
            joint_load_set.push_back(positions[i]);
            joint_load_set.push_back(positions[i] + loads[i] / (float)50000);
        } 
    }

    return joint_load_set;
}

void JointManager::setAnchor(double x, double y)
{
    if (clickJoint(x, y))
    {
        anchors[index_selected] = !anchors[index_selected];
    }
}

void JointManager::setAnimate(int animate)
{
    animate_progress = animate / 200.0;
}

std::vector<glm::vec3> JointManager::bufferData()
{
    std::vector<glm::vec3> buffer_data;

    for (int i = 0; i < positions.size(); i++)
    {
        buffer_data.push_back(glm::vec3(positions[i], anchors[i]));
    }

    return buffer_data;
}

bool JointManager::clickJoint(double x, double y)
{
    for (int i = 0; i < positions.size(); i++)
    {
        if (glm::length(positions[i] - glm::vec2(x, y)) < 8)
        {
            index_selected = i;
            return true;
        }  
    }
    return false;
}