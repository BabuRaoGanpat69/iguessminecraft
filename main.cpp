
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

  Shader lightingshader("../shader/combinedlight.vs",
                        "../shader/combinedlight.fs");
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

  glm::vec3 pointLightPositions[] = {
      glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f),
      glm::vec3(-4.0f, 2.0f, -12.0f), glm::vec3(0.0f, 0.0f, -3.0f)};
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
    std::cout << "failed to load texture" << std::endl;
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

    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingshader.use();
    lightingshader.setvec3("viewPos", camera.Position);
    lightingshader.setFloat("material.shininess", 32.0f);

    /*
       Here we set all the uniforms for the 5/6 types of lights we have. We
       have to set them manually and index the proper PointLight struct in the
       array to set each uniform variable. This can be done more code-friendly
       by defining light types as classes and set their values in there, or by
       using a more efficient uniform approach by using 'Uniform buffer
       objects', but that is something we'll discuss in the 'Advanced GLSL'
       tutorial.
    */
    // directional light
    lightingshader.setvec3("dirLight.direction",
                           glm::vec3{-0.2f, -1.0f, -0.3f});
    lightingshader.setvec3("dirLight.ambient", glm::vec3{0.05f, 0.05f, 0.05f});
    lightingshader.setvec3("dirLight.diffuse", glm::vec3{0.4f, 0.4f, 0.4f});
    lightingshader.setvec3("dirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});
    // point light 1
    lightingshader.setvec3("pointLights[0].position", pointLightPositions[0]);
    lightingshader.setvec3("pointLights[0].ambient",
                           glm::vec3{0.05f, 0.05f, 0.05f});
    lightingshader.setvec3("pointLights[0].diffuse",
                           glm::vec3{0.8f, 0.8f, 0.8f});
    lightingshader.setvec3("pointLights[0].specular",
                           glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setFloat("pointLights[0].constant", 1.0f);
    lightingshader.setFloat("pointLights[0].linear", 0.09f);
    lightingshader.setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    lightingshader.setvec3("pointLights[1].position", pointLightPositions[1]);
    lightingshader.setvec3("pointLights[1].ambient",
                           glm::vec3{0.05f, 0.05f, 0.05f});
    lightingshader.setvec3("pointLights[1].diffuse",
                           glm::vec3{0.8f, 0.8f, 0.8f});
    lightingshader.setvec3("pointLights[1].specular",
                           glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setFloat("pointLights[1].constant", 1.0f);
    lightingshader.setFloat("pointLights[1].linear", 0.09f);
    lightingshader.setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    lightingshader.setvec3("pointLights[2].position", pointLightPositions[2]);
    lightingshader.setvec3("pointLights[2].ambient",
                           glm::vec3{0.05f, 0.05f, 0.05f});
    lightingshader.setvec3("pointLights[2].diffuse",
                           glm::vec3{0.8f, 0.8f, 0.8f});
    lightingshader.setvec3("pointLights[2].specular",
                           glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setFloat("pointLights[2].constant", 1.0f);
    lightingshader.setFloat("pointLights[2].linear", 0.09f);
    lightingshader.setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    lightingshader.setvec3("pointLights[3].position", pointLightPositions[3]);
    lightingshader.setvec3("pointLights[3].ambient",
                           glm::vec3{0.05f, 0.05f, 0.05f});
    lightingshader.setvec3("pointLights[3].diffuse",
                           glm::vec3{0.8f, 0.8f, 0.8f});
    lightingshader.setvec3("pointLights[3].specular",
                           glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setFloat("pointLights[3].constant", 1.0f);
    lightingshader.setFloat("pointLights[3].linear", 0.09f);
    lightingshader.setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    lightingshader.setvec3("spotLight.position", camera.Position);
    lightingshader.setvec3("spotLight.direction", camera.Front);
    lightingshader.setvec3("spotLight.ambient", glm::vec3{0.0f, 0.0f, 0.0f});
    lightingshader.setvec3("spotLight.diffuse", glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setvec3("spotLight.specular", glm::vec3{1.0f, 1.0f, 1.0f});
    lightingshader.setFloat("spotLight.constant", 1.0f);
    lightingshader.setFloat("spotLight.linear", 0.09f);
    lightingshader.setFloat("spotLight.quadratic", 0.032f);
    lightingshader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    lightingshader.setFloat("spotLight.outerCutOff",
                            glm::cos(glm::radians(15.0f)));

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingshader.setmat4("projection", projection);
    lightingshader.setmat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingshader.setmat4("model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
      // calculate the model matrix for each object and pass it to shader
      // before drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      lightingshader.setmat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    lightCubeShader.use();
    lightCubeShader.setmat4("projection", projection);
    lightCubeShader.setmat4("view", view);

    // we now draw as many light bulbs as we have point lights.
    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, pointLightPositions[i]);
      model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
      lightCubeShader.setmat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse
    // moved etc.)
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

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
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
  // and height will be significantly larger than specified on retina
  // displays.
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
