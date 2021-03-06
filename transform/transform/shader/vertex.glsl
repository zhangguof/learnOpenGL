#version 330 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;
out vec3 ourColor;
out vec2 TexCoord;

//uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//out vec4 vertexColor;
void main()
{
    //gl_Position = transform * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = projection * view * model * vec4(position.x,position.y,position.z,1.0);
    ourColor = vec3(0.0f,0.0f,1.0f);
    TexCoord = vec2(texCoord.x,1-texCoord.y);
}