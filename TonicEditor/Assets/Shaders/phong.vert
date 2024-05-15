#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 viewProjection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalize(vec3(model * vec4(aNormal,0.0))); //to get rid of scaling and translate
    TexCoords = aTexCoords;
    
    gl_Position = viewProjection * vec4(FragPos, 1.0);
}