#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

uniform float ratio;
uniform samplerCube day;
uniform samplerCube night;

void main()
{    
    FragColor = mix(texture(day, TexCoord), texture(night, TexCoord), ratio);
}