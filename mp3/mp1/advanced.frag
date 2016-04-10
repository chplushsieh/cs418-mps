#version 330 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 color;

uniform vec3 cameraPos;

uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;

void main()
{
    // Texture
    vec3 objectColor = texture(texture_diffuse1, TexCoords).rgb;
    //vec3 objectColor = vec3(0.5f, 0.2f, 0.5f);
    
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    
    vec3 surfaceToLight = normalize(lightPos - Position);
    vec3 surfaceToView = normalize(cameraPos - Position);
    vec3 normal = normalize(Normal);
    
    // Ambient
    vec3 ambient = 0.05 * objectColor;
    
    // Diffuse
    float diff = max(dot(normal, surfaceToLight), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 1.0f;
    
//    // phong
//    vec3 reflectDir = reflect(-surfaceToLight, normal);
//    float spec = pow(max(dot(surfaceToView, reflectDir), 0.0), 32);
    //binn-phong
    vec3 halfwayDir = normalize(surfaceToLight + surfaceToView);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0f);
    
    vec3 specular = specularStrength * spec * lightColor;
    
    // Reflection
    float reflectionStrength = 0.5f;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normal);
    vec3 reflect = reflectionStrength * texture(skybox, R).rgb;
    
    // Simple attenuation
    float distance = length(lightPos - Position);
    float attenuation = 1.0 / distance * distance;
    
    diffuse *= attenuation;
    specular *= attenuation;
    
    // gamma correction
    objectColor = pow(objectColor, vec3(1.0/2.2));
    color = vec4((ambient + diffuse + specular) * (objectColor + reflect), 1.0f);
}