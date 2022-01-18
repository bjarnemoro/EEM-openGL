#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

class JointManager
{
public:
    int index_selected;
    double animate_progress = 0.0;
    std::vector<glm::vec2> positions;
    std::vector<bool> anchors;
    std::vector<glm::vec2> loads;
    std::vector<glm::vec2> displacements;
    void addJoint(double x, double y);
    void addLoad(double x, double y);
    bool clickJoint(double x, double y);
    void setAnchor(double x, double y);
    std::vector<glm::vec2> getDisplacedPositions();
    std::vector<glm::vec2> getJointLoadSet();
    std::vector<glm::vec3> bufferData();
    void setAnimate(int animate);
};