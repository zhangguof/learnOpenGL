#version 330 core

//in vec2 TexCoord;

in vec3 Normal;
in vec3 FragPos;

out vec4 color;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

uniform vec3 lightPos;

//vertex color
uniform vec3 objectColor;
uniform vec3 lightColor;
void main()
{
    //ambien
    float ambienStrength = 0.2f;
    vec3 ambien = ambienStrength * lightColor;
    
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambien + diffuse) * objectColor;
    color = vec4(result,1.0f);
}
