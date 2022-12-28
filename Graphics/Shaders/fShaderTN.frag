#version 330

in vec3 FragPos;
in vec3 LightPos;
in vec3 ViewPos;
in vec2 Texture;
in mat3 TBN_mat;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ambientStrength = 0.1;
float diffStrength = 0.7;
float specularStrength = 0.01;

void main()
{

    vec3 lightColor = vec3 (1.0, 1.0, 1.0);
    vec3 color = texture(texture_diffuse1, Texture).rgb;
    vec3 normal = texture(texture_normal1, Texture).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN_mat * normal);

    vec3 ambient = ambientStrength * color;

    vec3 lightDir = TBN_mat *normalize(LightPos - FragPos); 
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color;

    vec3 viewDir = TBN_mat *normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal); 
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    vec3 specular = vec3(specularStrength) * spec;  

    vec3 result = (ambient + diffuse + specular);

    gl_FragColor = vec4(result, 1.0);
}  