#pragma once

#include "joint_manager.h"
#include "math.h"
#include "graphics_manager.h"
#include <Dense>
#include <Core>
//#include "bars.h"

class Anchors
{
public:
    std::vector<int> anchors;
};

class Loads
{
public:
    std::vector<glm::vec2> loads;
};
    

/*
Calculate displacement of a properly confined body consisting of bar elements.
a bar element is an element consisting of two nodes that exists on a 2d coordinate system
these nodes can displace in both the x and y direction
bar elements only take axial forces


@parameters
nodes -- list of tuples, tuple contains xand y coordinates of a point in the global coordinate system
beams -- list of tuples, tuple contains the index of two node elements
anchors -- list of indices, the index sets the degrees of freedom to 0 for the chosen node

@return
displacements -- list of tuples, tuple contains xand y displacements for the given nodes
anchor nodes have a displacement of 0
*/
std::vector<glm::vec2> FEA(JointManager* joint_manager, BarManager* bar_manager, std::vector<unsigned int> anchors, std::vector<glm::vec2> loads);

glm::mat4 local_K(double theta);

Eigen::MatrixXd get_stiffness_matrix(JointManager* joint_manager, BarManager* bar_manager);

double get_rotation(glm::vec2 joint1, glm::vec2 joint2);