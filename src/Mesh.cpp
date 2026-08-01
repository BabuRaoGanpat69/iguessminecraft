#include "Mesh.h"
#include <GL/glext.h>
Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices,
           vector<Texture> textures) {
  this->vertices = vertices;
  this->indices = indices;
  this->textures = textures;

  setupMesh();
};
void Mesh::setupMesh()
{
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

    
    
}