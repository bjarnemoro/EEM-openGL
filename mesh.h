#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shader.h"

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Material {
    glm::vec4 Ka;
    glm::vec4 Kd;
    glm::vec4 Ks;
};

class Mesh {
public:
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Material material;

    unsigned int VAO;
    unsigned int uniformBlockIndex;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);
    void Draw(Shader& shader);
    void UpdateColor();

private:
    unsigned int VBO, EBO;
    void setupMesh();
};