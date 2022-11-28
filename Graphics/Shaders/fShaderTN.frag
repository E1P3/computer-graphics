#version 330

in vec3 FragPos;
in vec3 LightPos;
in vec3 ViewPos;
in vec2 Texture;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.2;
float diffStrength = 0.7;
float specularStrength = 0.01;

void main()
{

    vec3 lightColor = vec3 (1.0, 1.0, 1.0);
    vec3 color = texture(texture_diffuse, Texture).rgb;
    //vec3 color = vec3(0.5,0.5,0.5);
    vec3 normal = texture(texture_normal, Texture).rgb;
    normal = normalize(normal);

    vec3 ambient = ambientStrength * color;

    vec3 lightDir = normalize(-vec3(-1.0,-1.0,-1.0)); 
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal); 
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = vec3(specularStrength) * spec;  

    vec3 result = (ambient + diffuse + specular);

    gl_FragColor = vec4(result, 1.0);
}  