#version 330 core

in vec3 fragPos;  
in vec3 normal;
  
out vec4 fragColor;


struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform Light light; 
uniform vec3 viewPos;


void main() {
    // ambient
    vec3 ambient = material.ambient * light.ambient;
    
    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * material.diffuse * light.diffuse;
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0), material.shininess);
    vec3 specular = spec * material.specular * light.specular;


    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}