#version 330 core
    layout (location=0) in vec3 apos;
    layout (location=1) in vec3 aColor;
    layout (location=2) in vec2 aTexCoord;
 out vec3 ourColor;
 out vec2 TexCoord;
 uniform mat4 transform;
 uniform float offset;
    void main()
    {
    gl_Position=transform*vec4(apos.x+offset,apos.y,apos.z,1.0);
    ourColor=aColor;
    TexCoord=aTexCoord;

    };