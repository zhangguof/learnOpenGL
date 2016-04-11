#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
//layout (location = 2) in vec2 texCoord;
layout (location = 2) in vec3 noraml;

//out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;



//uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//out vec4 vertexColor;
void main()
{

    gl_Position = projection * view * model * vec4(position.x,position.y,position.z,1.0);
    FragPos = vec3(model * vec4(position,1.0f));
    
    
    //TexCoord = vec2(texCoord.x,1-texCoord.y);
    Normal = noraml;
    TexCoords = texCoord;
}