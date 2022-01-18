#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

struct JointSet
{
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> loads;
    std::vector<glm::vec2> displacements;
    std::vector<bool> anchors;

    void addSet(glm::vec2 position);
};

struct BarSet
{
    std::vector<glm::ivec2> joint_indices;
    std::vector<float> stretch;

    void addSet(glm::ivec2 indices);
};
    

class MaterialManager
{
private:
    JointSet* joint_set = new JointSet();
    BarSet* bar_set = new BarSet();

public:
    void addJointPosition(glm::vec2 position);
    void addJointIndices(glm::ivec2 joint_indices);
};

