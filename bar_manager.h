#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "joint_manager.h"

class BarManager
{
private:
public:
    std::vector<glm::ivec2> joint_indices;
    std::vector<double> stretch;
    void addIndices(int idx1, int idx2);
    void calculateStretch(JointManager* joint_manager);
};