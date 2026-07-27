 
#include "glad/glad.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window,double xpos,double ypos);
void scroll_callback(GLFWwindow *window,double xoffset,double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int success;
char infolog[512];

glm::vec3 cameraPos =glm::vec3(0.0f,0.0f,3.0f);
glm::vec3 cameraFront =glm::vec3(0.0f,0.0f,-1.0f);
glm::vec3 cameraUp =glm::vec3(0.0f,1.0f,0.0f);

bool firstmouse=true;
float yaw =-90.0f;
float pitch =0.0f;
float lastX=800.0/2.0f;
float lastY=600.0/2.0f;
float fov =45.0f;


float deltaTime =0.0f;
float lastFrame =0.0f;

glm::vec3 objectColor(1.0f, 0.5f, 0.31f); 
glm::vec3 lampColor(1.0f, 1.0f, 1.0f); 


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
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
   glfwSetScrollCallback(window, scroll_callback);


   //telling openg to capture our mouse
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }//texture code///////


  glEnable(GL_DEPTH_TEST);
///////texturecode/////
Shader mycubeshader("../shader/colors.vs","../shader/colors.fs");
Shader lightingshader("../shader/light_cube.vs","../shader/light_cube.fs");

float vertices[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
 //unsigned int indices[]={0,1,3,1,2,3};

  unsigned int cubeVAO,cubeVBO,cubeEBO;
  glGenVertexArrays(1,&cubeVAO);
  glGenBuffers(1,&cubeVBO);
  glGenBuffers(1,&cubeEBO);

  glBindVertexArray(cubeVAO);

  glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

 // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
 // glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

 // glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
  //glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  //////////

unsigned int lampVAO,lampVBO;
glGenVertexArrays(1,&lampVAO);
glGenBuffers(1,&lampVBO);
glBindVertexArray(lampVAO);
glBindBuffer(GL_ARRAY_BUFFER,lampVBO);
glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
glEnableVertexAttribArray(0);


  
  unsigned int texture1,texture2;
  GLCall(glGenTextures(1,&texture1));
   GLCall(glBindTexture(GL_TEXTURE_2D,texture1));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


stbi_set_flip_vertically_on_load(true);
int width1,height1,nrChannels1;
unsigned char* data1=stbi_load("../Cute_dog.jpg",&width1,&height1,&nrChannels1,0);
if(data1)
{
     GLCall(glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width1,height1,0,GL_RGB,GL_UNSIGNED_BYTE,data1));
     GLCall(glGenerateMipmap(GL_TEXTURE_2D));
}
else {
{
  std::cout<<"failed to load texture"<<std::endl;
}
}

  stbi_image_free(data1);

   GLCall(glGenTextures(1,&texture2));
   GLCall(glBindTexture(GL_TEXTURE_2D,texture2));

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  int width ,height,nrChannels;
  unsigned char* data = stbi_load("../download.jpg",&width,&height,&nrChannels,0);

  if(data)
  {
      GLCall( glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data));
      GLCall( glGenerateMipmap(GL_TEXTURE_2D));

  }
  else{
      std::cout<<"failed to load texture"<<std::endl;
  }
   stbi_image_free(data);
   // GLCall(mycubeshader.use());
   // GLCall( glUniform1i(glGetUniformLocation(mycubeshader.ID, "texture1"), 0));
    //GLCall(mycubeshader.setInt("texture2", 2));
    //glmmms



 ///uniforms prcatise
glm::vec3 cubePositions[]={
    glm::vec3( 0.0f,  0.0f,  0.0f),
      glm::vec3( 2.0f,  5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f,  3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f,  2.0f, -2.5f),
      glm::vec3( 1.5f,  0.2f, -1.5f),
      glm::vec3(-1.3f,  1.0f, -1.5f)

};


glm::mat4 projection=glm::mat4(1.0f);

glm::mat4 model =glm::mat4(1.0f); 
projection=glm::perspective(glm::radians(100.0f),(float)SCR_WIDTH/(float)SCR_HEIGHT,0.1f,100.0f);

mycubeshader.use();

unsigned int color=glGetUniformLocation(mycubeshader.ID,"objectColor");
unsigned int lampcolor=glGetUniformLocation(mycubeshader.ID,"lampColor");
unsigned int projectionLoc=  glGetUniformLocation(mycubeshader.ID,"projection");
unsigned int modelLoc=  glGetUniformLocation(mycubeshader.ID,"model");

glUniform3fv(color,1,glm::value_ptr(objectColor));
glUniform3fv(lampcolor,1,glm::value_ptr(lampColor));
glUniformMatrix4fv(projectionLoc,1,GL_FALSE,glm::value_ptr(projection));
glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));


lightingshader.use();
   unsigned int projectionLoc1=  glGetUniformLocation(lightingshader.ID,"projection");
   unsigned int modelLoc1=  glGetUniformLocation(lightingshader.ID,"model");
   unsigned int lampcolor1=glGetUniformLocation(lightingshader.ID,"lampColor");
   
 glm::mat4  model1 =glm::translate(model,cubePositions[1]);
glUniformMatrix4fv(projectionLoc1,1,GL_FALSE,glm::value_ptr(projection));
glUniformMatrix4fv(modelLoc1,1,GL_FALSE,glm::value_ptr(model1));
glUniform3fv(lampcolor1,1,glm::value_ptr(lampColor));


  // render loop
  // -----------
  //
  while (!glfwWindowShouldClose(window)) {



      float currentFrame = static_cast<float>(glfwGetTime());
      deltaTime = currentFrame-lastFrame;
      lastFrame=currentFrame;
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


           GLCall(glActiveTexture(GL_TEXTURE0));
           GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
          GLCall( glActiveTexture(GL_TEXTURE2));
          GLCall( glBindTexture(GL_TEXTURE_2D, texture2));


    GLCall(mycubeshader.use());
     glBindVertexArray(cubeVAO);
    float time =glfwGetTime();
    float a_val=(sin(time)/2)+0.5f;
   mycubeshader.setFloat("a",a_val);

   glm::mat4 view =glm::mat4(1.0f);
  view = glm::lookAt(cameraPos,cameraFront+cameraPos,cameraUp);
glBindVertexArray(cubeVAO);
mycubeshader.use();
unsigned int viewLoc=glGetUniformLocation(mycubeshader.ID,"view");
glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
glDrawArrays(GL_TRIANGLES,0,36);
lightingshader.use();
unsigned int viewLoc1=glGetUniformLocation(lightingshader.ID,"view");
glUniformMatrix4fv(viewLoc1,1,GL_FALSE,glm::value_ptr(view));
glDrawArrays(GL_TRIANGLES,0,36);



     //glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glDeleteVertexArrays(1,&cubeVAO);
  glDeleteBuffers(1,&cubeVBO);
  glDeleteBuffers(1,&cubeEBO);

 
  glDeleteVertexArrays(1,&lampVAO);
  glDeleteBuffers(1,&lampVBO);
 


  
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
  float cameraSpeed= static_cast<float>(2.5*deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
       cameraPos += cameraSpeed * cameraFront;
   if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       cameraPos -= cameraSpeed * cameraFront;
   if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
   if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
       cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window , double xposIN,double yposIN)
{
    float xpos = static_cast<float>(xposIN);
    float ypos = static_cast<float>(yposIN);

    if(firstmouse)
    {
        lastX =xpos;
        lastY=ypos;
        firstmouse=false;
    }
    float xoffset = xpos-lastX;
    float yoffset = lastY-ypos;
    lastX=xpos;
    lastY=ypos;

    float sensitivity =0.1f;
    xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw+= xoffset;
      pitch +=yoffset;

      if(pitch>89.0f)
          pitch=89.0f;
      if(pitch< -89.0f)
          pitch =-89.0f;


      glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);

}
void scroll_callback(GLFWwindow* window,double xoffset, double yoffset)
{
    fov -=(float)yoffset;
    if(fov<1.0f)
        fov=1.0f;
    if(fov>45.0f)
        fov=45.0f;

}
