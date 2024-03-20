#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main()
{
    //vec4 tex0 = texture(texture_diffuse1, TexCoords);
    //vec4 tex1 = texture(texture_specular1, TexCoords);
    //vec4 tex2 = texture(texture_normal1, TexCoords);
    //FragColor = mix(tex0, tex1);
    FragColor = texture(texture_diffuse1, TexCoords);
}