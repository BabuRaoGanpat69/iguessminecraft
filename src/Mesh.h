#pragma once
#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "renderer.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};
struct Texture {
  unsigned int id;
  std::string type;
};
struct Shader;
class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> texture;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> texture);
  void Draw(Shader &shader);

private:
  unsigned int VAO, VBO, EBO;
  void setupMesh();
};
