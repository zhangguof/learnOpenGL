#version 330 core

//in vec2 TexCoord;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

out vec4 color;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

//uniform vec3 lightPos;

//vertex color
//uniform vec3 objectColor;
//uniform vec3 lightColor;
uniform vec3 viewPos;
void main()
{
    //ambien
    //float ambienStrength = 0.2f;
    //vec3 ambient = ambienStrength * lightColor;
    vec3 ambient = light.ambient * material.ambient;
    
    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm,lightDir),0.0);
    //vec3 diffuse = diff * lightColor;
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    //specular
    //float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflecDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflecDir),0.0),material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    vec3 result = ambient + diffuse + specular;
    color = vec4(result,1.0f);
}
