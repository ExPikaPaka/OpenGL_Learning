#version 330 core

in vec3 fragPos;  
in vec2 texCoord;  
in vec3 normal;
  
out vec4 fragColor;


struct Material {
    sampler2D diffuseTex[4];
    sampler2D specularTex;
    sampler2D emissiveTex;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float emissivity;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Light type
    // 0 - directional
    // 1 - point
    // 2 - spotlight
    int type;

    // Point light parameters
    float constant;
    float linear;
    float quadratic;
};


uniform Material material;
uniform Light light; 
uniform vec3 viewPos;



void main() {
    fragColor = vec4(vec3(texture(material.diffuseTex[0], texCoord)), 1.0);
}