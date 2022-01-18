#include "FEM.h"

std::vector<glm::vec2> FEA(JointManager* joint_manager, BarManager* bar_manager, std::vector<unsigned int> _anchors, std::vector<glm::vec2> _loads)
{
    std::vector<glm::vec2> displacements_vector;
    Eigen::MatrixXd displacements;
    Eigen::MatrixXd stifness_matrix;
    Eigen::MatrixXd confined_stifness_matrix;

    Eigen::MatrixXd loads(1, _loads.size() * 2);

    for (int i = 0; i < _loads.size(); i++)
    {
        loads(i * 2) = _loads[i].x;
        loads(i * 2 + 1) = _loads[i].y;
    }

    std::vector<unsigned int> non_confined_rows;
    for (int i = 0; i < _loads.size(); i++)
    {
        if (std::find(_anchors.begin(), _anchors.end(), i) != _anchors.end()) {}
        else
        {
            non_confined_rows.push_back(i * 2);
            non_confined_rows.push_back(i * 2 + 1);
        }
    }

    //Get the stifness matrix, delete the anchor rows and columns from the K matrix afterwards
    stifness_matrix = get_stiffness_matrix(joint_manager, bar_manager);

    confined_stifness_matrix = stifness_matrix(non_confined_rows, non_confined_rows);

    //invert the K matrix
    Eigen::MatrixXd inverted_confined_K = confined_stifness_matrix.inverse();


    Eigen::MatrixXd confined_loads = loads(Eigen::all, non_confined_rows);

    displacements = confined_loads * inverted_confined_K;

    int iterator = 0;
    for (unsigned int i = 0; i < _loads.size(); i++)
    {
        if (std::find(_anchors.begin(), _anchors.end(), i) != _anchors.end()) 
        {
            displacements_vector.push_back(glm::vec2(0, 0));
        }
        else
        {
            displacements_vector.push_back(glm::vec2(displacements(iterator), displacements(iterator+1)));
            iterator += 2;
        }
        
    }

    return displacements_vector;
}

Eigen::MatrixXd get_stiffness_matrix(JointManager* joint_manager, BarManager* bar_manager)
{
    const int size = joint_manager->positions.size() * 2;
    int E = 210e3;
    int A = 1000;

    Eigen::MatrixXd stiffness_matrix = Eigen::MatrixXd::Zero(size, size);

    glm::vec2 joint1, joint2;
    unsigned int i1, i2;
    double rotation;

    for (const glm::ivec2& indices : bar_manager->joint_indices)
    {
        joint1 = joint_manager->positions[indices[0]];
        joint2 = joint_manager->positions[indices[1]];

        rotation = get_rotation(joint1, joint2);

        double L = glm::length(joint2 - joint1);
        double stifness = ((double)E * (double)A) / L;
        glm::mat4 local_stifness = local_K(rotation);

        std::vector<glm::ivec2> combos = 
        { 
            glm::ivec2(indices[0], indices[0]), 
            glm::ivec2(indices[0], indices[1]), 
            glm::ivec2(indices[1], indices[0]), 
            glm::ivec2(indices[1], indices[1]) 
        };

        for (unsigned int i = 0; i < 4; i++)
        {
            i1 = i % 2;
            i2 = i / 2;
            stiffness_matrix(combos[i][0] * 2, combos[i][1] * 2) += stifness * local_stifness[i1 * 2][i2 * 2];
            stiffness_matrix(combos[i][0] * 2 + 1, combos[i][1] * 2) += stifness * local_stifness[i1 * 2 + 1][i2 * 2];
            stiffness_matrix(combos[i][0] * 2, combos[i][1] * 2 + 1) += stifness * local_stifness[i1 * 2][i2 * 2 + 1];
            stiffness_matrix(combos[i][0] * 2 + 1, combos[i][1] * 2 + 1) += stifness * local_stifness[i1 * 2 + 1][i2 * 2 + 1];
        }
    }

    return stiffness_matrix;
}

glm::mat4 local_K(double theta)
{
    double c = glm::cos(theta);
    double s = glm::sin(theta);
    return glm::mat4(
        pow(c, 2), c * s, -pow(c, 2), -c * s,
        c * s, pow(s, 2), -c * s, -pow(s, 2),
        -pow(c, 2), -c * s, pow(c, 2), c * s,
        -c * s, -pow(s, 2), c * s, pow(s, 2));
}

double get_rotation(glm::vec2 joint1, glm::vec2 joint2)
{
    if ((joint1[1] - joint2[1]) != 0)
        return glm::atan((joint1[1] - joint2[1]) / (joint1[0] - joint2[0]));
    else
        return 0.0;
}