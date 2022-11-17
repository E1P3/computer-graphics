#version 330

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float diffStrength;
uniform float specularStrength;

uniform vec3 viewPos;


void main()
{
    vec3 lightColor = vec3 (1.0, 1.0, 1.0);
    
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffStrength;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular) * objectColor;
    gl_FragColor = vec4(result, 1.0);
}  