#version 330 core

//in vec2 TexCoord;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



uniform Material material;
uniform Light light;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
void main()
{
    //ambien
    //float ambienStrength = 0.2f;
    //vec3 ambient = ambienStrength * lightColor;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
    
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    //vec3 diffuse = diff * lightColor;
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
    
    //specular
    //float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflecDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
    
    vec3 result = ambient + diffuse + specular;
    color = vec4(result,1.0f);
}
