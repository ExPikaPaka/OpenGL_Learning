#version 330 core

in vec3 fragPos;  
in vec2 texCoord;  
in vec3 normal;
  
out vec4 fragColor;


#define MAX_TEXTURES 4
struct Material {
    sampler2D diffuseTex[MAX_TEXTURES];
    sampler2D specularTex[MAX_TEXTURES];
    sampler2D emissiveTex[MAX_TEXTURES];

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float emissivity;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Light type
    // 0 - point (no attenuation)
    // 1 - point (with attenuation)
    // 2 - directional
    // 3 - spotlight (no attenuation)
    // 4 - spotlight (with attenuation)
    int type;

    // Point light parameters
    float constant;
    float linear;
    float quadratic;

    // Spotlight parameters
    float cutoff;
    float outerCutoff;
};


#define MAX_LIGHTS 32 

uniform Material material;
uniform Light light[MAX_LIGHTS];
uniform int lights_count;
uniform int diffuseTex_count;
uniform int specularTex_count;
uniform int emissiveTex_count;
uniform vec3 viewPos;


vec3 calcPointLight(Light light, vec3 normal, vec3 viewDir);
vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(Light light, vec3 normal, vec3 viewDir);


void main() {
    // Calculating multiple lights
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 result = vec3(0);

    // looping trough all lights
    for(int i = 0; i < lights_count; i++) {
        if(light[i].type == 0 || light[i].type == 1) { // 0/1 - point (with/without attenuation)
            result += calcPointLight(light[i], norm,viewDir);
        }
        if(light[i].type == 2) { // 2 - directional
            result += calcDirLight(light[i], norm,viewDir);    
        }
        if(light[i].type == 3 || light[i].type == 4) { // 3/4 - spotlight (with/without attenuation)
            result += calcSpotLight(light[i], norm,viewDir);    
        }
    }
    fragColor = vec4(result, 1);
}


// Calculates point light with/without attenuation
vec3 calcPointLight(Light light, vec3 normal, vec3 viewDir) {
    // Ambient component
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex[0], texCoord));

    // Diffuse component
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * light.diffuse * material.diffuse * vec3(texture(material.diffuseTex[0], texCoord));

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = spec * light.specular * material.specular * vec3(texture(material.specularTex[0], texCoord));

    // Color result
    vec3 result = ambient + diffuse + specular;

    // Attenuation component
    if(light.type == 1) { // Point light type with attenuation
        float dist = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

        float ambBrightness = ambient.r + ambient.g + ambient.b;
        float attenBrightness = (result.r + result.g + result.b) * attenuation;

        if(attenBrightness > ambBrightness) { // Decrease brightness
            result *= attenuation;
        } else { // Use ambient if it becomes too dark
            result = ambient;
        }
    }

    // Emmision component
   // result += vec3(texture(material.emissiveTex[0], texCoord)) * material.emissivity / lights_count;

    return result;
}


// Calculates direct light
vec3 calcDirLight(Light light, vec3 normal, vec3 viewDir) {
    // Ambient component
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex[0], texCoord));

    // Diffuse component
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * light.diffuse * material.diffuse * vec3(texture(material.diffuseTex[0], texCoord));

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = spec * light.specular * material.specular * vec3(texture(material.specularTex[0], texCoord));
    
    // Result color
    vec3 result = ambient + diffuse + specular;

    // Emmision component
    //result += vec3(texture(material.emissiveTex[0], texCoord)) * material.emissivity / lights_count;

    return result;
}


// Calculates spot ligth with/without attenuation
vec3 calcSpotLight(Light light, vec3 normal, vec3 viewDir) {
    // Ambient component
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuseTex[0], texCoord));

    // Diffuse component
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0);
    vec3 diffuse = diff * light.diffuse * material.diffuse * vec3(texture(material.diffuseTex[0], texCoord));

    // Specular component
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess);
    vec3 specular = spec * light.specular * material.specular * vec3(texture(material.specularTex[0], texCoord));

    // Color result
    vec3 result = ambient + diffuse + specular;

    // Conus cutoff calculating 
    float theta = dot(normalize(light.direction), -lightDir);
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = smoothstep(0.0, 1.0, (theta - light.outerCutoff) / epsilon);

    if(theta > light.cutoff) { // Inner cutoff (Do not change light)
        if(light.type == 4) { // Spot light type with attenuation
            float dist = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

            float ambBrightness = ambient.r + ambient.g + ambient.b;
            float attenBrightness = (result.r + result.g + result.b) * attenuation;

            if(attenBrightness > ambBrightness) { // Decrease brightness
                result *= attenuation;
            } else { // Use ambient if it becomes too dark
                result = ambient;
            }
        }
    } else { // Outer cutoff (Decrease light as flashlight has diameter)
        if(light.type == 4) { // Spot light type with attenuation
            float dist = length(light.position - fragPos);
            float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

            float ambBrightness = ambient.r + ambient.g + ambient.b;
            float attenBrightness = (result.r + result.g + result.b) * attenuation * intensity;

            if(attenBrightness > ambBrightness) { // Decrease brightness
                ambient *= intensity;
                diffuse *= intensity;
                specular *= intensity;
                result = (ambient + diffuse + specular) * attenuation;
            } else { // Use ambient if it becomes too dark
                result = ambient;
            }
        } else {
            diffuse *= intensity;
            specular *= intensity;
            result = (ambient + diffuse + specular);
        }
    }

    // Emmision component
    //result += vec3(texture(material.emissiveTex[0], texCoord)) * material.emissivity / lights_count;
   
    return result;
}