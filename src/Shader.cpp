#include "Shader.hpp"
Shader::Shader(const char* vertexPath,const char* fragmentPath)
{

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderfile;
    std::ifstream fShaderfile;
   
    vShaderfile.open(vertexPath);
    if(!vShaderfile.is_open())
    {
        throw std::runtime_error("failed to open file");
    }
    fShaderfile.open(fragmentPath);
    if(!fShaderfile.is_open())
    {
        throw std::runtime_error("failed to open file");
    } 
std::stringstream vshaderStream ,fshaderStream;
vshaderStream << vShaderfile.rdbuf();
fshaderStream << fShaderfile.rdbuf();
    vertexCode = vshaderStream.str();
    fragmentCode =fshaderStream.str();

       const char* vshaderCode= vertexCode.c_str();
       const char* fshaderCode= fragmentCode.c_str();

       //compiling and linking

int success;
char infolog[512];
       
       unsigned int vertex, fragment;
       vertex=glCreateShader(GL_VERTEX_SHADER);
       fragment=glCreateShader(GL_FRAGMENT_SHADER);
       glShaderSource(vertex,1,&vshaderCode,NULL);
       glShaderSource(fragment,1,&fshaderCode,NULL);
       glCompileShader(vertex);
       glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
       if(!success)
       {
           glGetShaderInfoLog(vertex,512,NULL,infolog);
           std::cout<<"VERT COMPILATION ERROR\n"<<std::endl;
       }

       //fragmeant 
       glCompileShader(fragment);
       glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
       if(!success)
       {
           glGetShaderInfoLog(fragment,512,NULL,infolog);
           std::cout<<"frag COMPILATION ERROR\n"<<infolog<<std::endl;
       };
       ID= glCreateProgram();
       glAttachShader(ID,vertex);
       glAttachShader(ID,fragment);
       glLinkProgram(ID);
       glGetProgramiv(ID,GL_LINK_STATUS,&success);
       if(!success)
       {
           glGetProgramInfoLog(ID,512,NULL,infolog);
           std::cout<<"program COMPILATION ERROR\n"<<infolog<<std::endl;
       };
       glDeleteShader(vertex);
       glDeleteShader(fragment);
}
Shader::~Shader(){
    glDeleteProgram(ID);
};
void Shader::use(){
    glUseProgram(ID);
};
void Shader::setBool(const std::string &name, bool value) const
{
    GLCall(glUniform1i(glGetUniformLocation(ID,name.c_str()),int(value)));
}
void Shader::setInt(const std::string &name,int value) const 
{
   GLCall( glUniform1i(glGetUniformLocation(ID,name.c_str()),value));
}void Shader::setFloat(const std::string &name,float value) const 
{
   GLCall( glUniform1f(glGetUniformLocation(ID,name.c_str()),value));
}
void Shader::set4f(const std::string &name,vec4f value)
{
   GLCall( glUniform4f(glGetUniformLocation(ID,name.c_str()),value.x,value.y,value.z,value.w));
}