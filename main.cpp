
#include "glad/glad.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "Shader.hpp"
#include "glm/trigonometric.hpp"
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int success;
char infolog[512];
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

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }//texture code///////


  
///////texturecode/////
Shader myshader("../shader/vertex.shader","../shader/fragment.shader");

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
 unsigned int indices[]={0,1,3,1,2,3};

  unsigned int VAO,VBO,EBO;
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&VBO);
  glGenBuffers(1,&EBO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
  
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);
  
  glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
  //////////




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
    GLCall(myshader.use());
    GLCall( glUniform1i(glGetUniformLocation(myshader.ID, "texture1"), 0));
    GLCall(myshader.setInt("texture2", 2));
    //glmmms

    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
   
   
    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);


    
 ///uniforms prcatise 

  // render loop
  // -----------
  //
  while (!glfwWindowShouldClose(window)) {
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

          
    GLCall(myshader.use());
     glBindVertexArray(VAO);
    float time =glfwGetTime();
    float a_val=(sin(time)/2)+0.5f;
   myshader.setFloat("a",a_val);
    glm::mat4 trans =glm::mat4(1.0f);
   trans=glm::rotate(trans,glm::radians(360.0f*a_val),glm::vec3(1.0f,1.0f,1.0f));
   trans=glm::scale(trans,glm::vec3(a_val,a_val,1.0f));

   GLCall(unsigned int transformloc=glGetUniformLocation(myshader.ID,"transform"));
   GLCall(glUniformMatrix4fv(transformloc,1,GL_FALSE,glm::value_ptr(trans)));
       myshader.setFloat("offset",0);
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
 
    myshader.setFloat("offset",a_val);
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

    myshader.setFloat("offset",-a_val);
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
   // glBindVertexArray(0);
    //glUseProgram(ShaderProgram1);
   // glBindBuffer(GL_ARRAY_BUFFER,VBO1);
   // glBindVertexArray(VAO1);
  //  glDrawArrays(GL_TRIANGLES, 0,3);
    //glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
     //glBindVertexArray(0);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);
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
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
