#include "material_manager.h"

void JointSet::addSet(glm::vec2 position)
{
    positions.push_back(position);
    loads.push_back(glm::vec2(0.0f, 0.0f));
    displacements.push_back(glm::vec2(0.0f, 0.0f));
    anchors.push_back(false);
}

void BarSet::addSet(glm::ivec2 indices)
{
    joint_indices.push_back(indices);
    stretch.push_back(0.0f);
}

void MaterialManager::addJointPosition(glm::vec2 position) { joint_set->addSet(position); }
void MaterialManager::addJointIndices(glm::ivec2 joint_indices) { bar_set->addSet(joint_indices); }