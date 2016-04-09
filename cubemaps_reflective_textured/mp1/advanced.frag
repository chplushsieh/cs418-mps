#version 330 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;

void main()
{
    // Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
    
    // Reflection
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 reflect_color = texture(skybox, R);
    
    color = reflect_color + diffuse_color;

}