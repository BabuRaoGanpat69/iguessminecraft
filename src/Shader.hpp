#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<istream>
#include<sstream>
#include <stdexcept>
#include "glad/glad.h"
#include"GLFW/glfw3.h"
#include"renderer.hpp"
#include "glm/gtc/type_ptr.hpp"
class Shader
{
  public:
    unsigned int ID;
    struct vec4f{float x;float y;float z; float w;};
  Shader(const char* vertexPath,const char* fragmentPath);
 void use();
 void setBool(const std::string &name,bool value ) const;
 void setInt(const std::string &name, int value ) const;
 void setFloat(const std::string &name, float value) const;
  void set4f(const std::string &name,vec4f value) const;
  void setvec3(const std::string &name,glm::vec3 value) const;
  void setmat4(const std::string &name,glm::mat4 value) const;
 ~Shader();
};
