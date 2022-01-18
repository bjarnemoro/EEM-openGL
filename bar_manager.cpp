#include "bar_manager.h"

void BarManager::addIndices(int idx1, int idx2)
{
    joint_indices.push_back(glm::ivec2(idx1, idx2));
    stretch.push_back(1.0);
}

void BarManager::calculateStretch(JointManager* joint_manager)
{
    unsigned int idx1, idx2;
    double regular_length, stretched_length;

    std::vector<glm::vec2> displaced_positions = joint_manager->getDisplacedPositions();

    for (int i = 0; i < stretch.size(); i++)
    {
        idx1 = joint_indices[i][0];
        idx2 = joint_indices[i][1];

        regular_length = glm::length(joint_manager->positions[idx1] - joint_manager->positions[idx2]);
        stretched_length = glm::length(displaced_positions[idx1] - displaced_positions[idx2]);

        stretch[i] = stretched_length / regular_length;
    }
}