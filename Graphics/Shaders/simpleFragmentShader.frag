#version 330

in vec3 FragPos;
in vec3 LightPos;
in vec3 ViewPos;
in vec2 Texture;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float diffStrength = 0.7;
float specularStrength = 0.2;

void main()
{

    vec3 lightColor = vec3 (1.0, 1.0, 1.0);
    vec3 color = texture(texture_diffuse, Texture).rgb;
    
    vec3 normal = texture(texture_normal, Texture).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(LightPos - FragPos); 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * color * diffStrength;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal); 
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular);

    gl_FragColor = vec4(result, 1.0);
}  