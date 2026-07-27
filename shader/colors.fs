#version 330 core
out vec4 FragColor;
//in vec2 TexCoord;  

///uniform sampler2D texture1;
//uniform sampler2D texture2;
uniform float a;
uniform vec3 objectColor;
uniform vec3 lampColor;
void main()
{
    FragColor = vec4(lampColor*objectColor,1.0f);
}   