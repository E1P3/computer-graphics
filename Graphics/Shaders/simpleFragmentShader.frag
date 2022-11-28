#version 330

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;

void main()
{
    vec3 lightColor = vec3 (1.0, 1.0, 1.0);
    vec3 objectColor = vec3 (1.0, 1.0, 1.0);
    //vec3 color = texture(texture_diffuse1, TexCoord).rgb;
    vec3 color = vec3(1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * color;

    float diffStrength = 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-vec3(-1.0,-1.0,-1.0)); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * color * diffStrength;

    float specularStrength = 0.001;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular);
    gl_FragColor = vec4(result, 1.0);
} 