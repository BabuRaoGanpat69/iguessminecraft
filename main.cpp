
#include "Shader.hpp"
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
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int success;
char infolog[512];

bool firstmouse = true;

float lastX = 800.0 / 2.0f;
float lastY = 600.0 / 2.0f;
float fov = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main() {
  // glfw: initialize and configure
  // ---------------------------------

  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  std::cout << "hl11o" << std::endl;
  glfwMakeContextCurrent(window);
  std::cout << "hlo" << std::endl;
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  std::cout << "hlo" << std::endl;
  // telling openg to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  } // texture code///////

  glEnable(GL_DEPTH_TEST);
  ///////texturecode/////

  Shader lightingshader("../shader/colors.vs", "../shader/colors.fs");
  Shader lightCubeShader("../shader/light_cube.vs", "../shader/light_cube.fs");
  std::cout << "shaders wrod " << std::endl;
  float vertices[] = {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,
      0.0f,  -1.0f, 1.0f,  1.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.5f,  -0.5f,
      0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      0.0f,  1.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      1.0f,  1.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,
      -0.5f, -1.0f, 0.0f,  0.0f,  1.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
      0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,
      -0.5f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,
      0.0f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
      0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,  0.5f,  -0.5f,
      -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,
      -1.0f, 0.0f,  1.0f,  0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
      1.0f,  0.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f,  1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,
      -0.5f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,
      1.0f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f};
  // unsigned int indices[]={0,1,3,1,2,3};

  unsigned int cubeVAO, cubeVBO;
  GLCall(glGenVertexArrays(1, &cubeVAO));
  GLCall(glGenBuffers(1, &cubeVBO));
  GLCall(glBindVertexArray(cubeVAO));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVBO));
  GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                      GL_STATIC_DRAW));

  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                               (void *)0));
  GLCall(glEnableVertexAttribArray(0));

  GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                               (void *)(3 * sizeof(float))));
  GLCall(glEnableVertexAttribArray(1));

  GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                               (void *)(6 * sizeof(float))));
  GLCall(glEnableVertexAttribArray(2));
  // glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
  // glEnableVertexAttribArray(1);

  GLCall(glBindVertexArray(0));
  //////////

  unsigned int lightCubeVAO;
  GLCall(glGenVertexArrays(1, &lightCubeVAO));
  GLCall(glBindVertexArray(lightCubeVAO));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVBO));
  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                               (void *)0));
  GLCall(glEnableVertexAttribArray(0));

  unsigned int diffuseMap, specularMap;
  GLCall(glGenTextures(1, &diffuseMap));
  GLCall(glBindTexture(GL_TEXTURE_2D, diffuseMap));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_set_flip_vertically_on_load(true);
  int width1, height1, nrChannels1;
  unsigned char *data1 =
      stbi_load("../container2.png", &width1, &height1, &nrChannels1, 0);
  if (data1) {

    GLenum format = (nrChannels1 == 4) ? GL_RGBA : GL_RGB;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width1, height1, 0, format,
                        GL_UNSIGNED_BYTE, data1));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
  } else {
    {
      std::cout << "failed to load texture" << std::endl;
    }
  }

  stbi_image_free(data1);

  GLCall(glGenTextures(1, &specularMap));
  GLCall(glBindTexture(GL_TEXTURE_2D, specularMap));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../container2_specular.png", &width, &height, &nrChannels, 0);

  if (data) {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                        GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

  } else {
    std::cout << "failed to load texture" << std::endl;
  }
  GLCall(stbi_image_free(data));

  lightingshader.use();
  lightingshader.setInt("material.diffuse", 0);
  lightingshader.setInt("material.specular", 1);
  std::cout << "loop starting" << std::endl;

  float aval = 0;
  //////looopppp///////

  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  while (!glfwWindowShouldClose(window)) {

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, diffuseMap));
    GLCall(glActiveTexture(GL_TEXTURE1));
    GLCall(glBindTexture(GL_TEXTURE_2D, specularMap));

    lightingshader.use();
    lightingshader.setvec3("light.position", lightPos);
    lightingshader.setvec3("viewPos", camera.Position);
    // light properties
    lightingshader.setvec3("light.ambient", glm::vec3{0.2f, 0.2f, 0.2f});
    lightingshader.setvec3("light.diffuse", glm::vec3{0.7f, 0.7f, 0.7f});
    lightingshader.setvec3("light.specular", glm::vec3{1.0f, 1.0f, 1.0f});

    lightingshader.setFloat("light.constant", 1.0f);
    lightingshader.setFloat("light.linear", 0.09f);
    lightingshader.setFloat("light.quadratic", 0.032f);

    lightingshader.setFloat("material.shininess", 64.0f);

    GLCall(glm::mat4 projection = glm::perspective(
               glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT,
               0.1f, 100.0f));
    GLCall(glm::mat4 view = camera.GetViewMatrix());
    lightingshader.setmat4("projection", projection);
    lightingshader.setmat4("view", view);

    // world transformation

    aval = static_cast<float>(glfwGetTime());
    std::cout << aval << std::endl;
    aval = sin(aval);
    GLCall(glBindVertexArray(cubeVAO));
    for (unsigned int i{}; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
     // model = glm::rotate(model, sin(360.0f * glm::radians(aval)),
       ///                   glm::vec3{1.0f, 0.0f, 0.0f});
      lightingshader.setmat4("model", model);
      GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    }
    // render the cube

    lightCubeShader.use();
    lightCubeShader.setmat4("projection", projection);
    lightCubeShader.setmat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setmat4("model", model);

    GLCall(glBindVertexArray(lightCubeVAO));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

    // glBindVertexArray(0);
    //  glfw: swap buffers and poll IO events (keys pressed/released, mouse
    //  moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  GLCall(glDeleteVertexArrays(1, &cubeVAO));
  GLCall(glDeleteBuffers(1, &cubeVBO));

  GLCall(glDeleteVertexArrays(1, &lightCubeVAO));

  //  glDeleteProgram(ShaderProgram);
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstmouse) {
    lastX = xpos;
    lastY = ypos;
    firstmouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
