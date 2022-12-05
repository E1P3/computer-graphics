#version 330 core
out vec4 FragColor;

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
    float strength;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

in VERTEX {
    vec3 fragment_position;
    vec3 normal;
    vec2 uv;
    vec4 fragment_position_light_space;
} vertex;

uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 dirLightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragment_position_light_space, vec3 direction)
{
    // perform perspective divide
    vec3 projCoords = fragment_position_light_space.xyz / fragment_position_light_space.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vertex.normal);
    vec3 lightDir = normalize(-direction);
    float bias = max(0.000001 * (1.0 - dot(normal, lightDir)), 0.0000001f);

    // check whether current frag pos is in shadow
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 getDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //vec3 lightColor = vec3(0.5, 0.88, 1.0);
    vec3 lightColor = vec3(1.0);
    vec3 lightDir = normalize(light.direction);

    float diff = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

    vec3 ambient = light.ambient * lightColor;
    vec3 diffuse = light.diffuse * diff * lightColor;
    vec3 specular = light.specular * spec * lightColor;

    float shadow = ShadowCalculation(vertex.fragment_position_light_space, light.direction); 

    return (ambient + (1 - shadow) * (diffuse + specular));

}

vec3 getPointLight(PointLight light, vec3 normal, vec3 fragment_position, vec3 viewDir)
{
    vec3 lightColor = vec3(light.strength);
    vec3 lightDir = normalize(light.position - fragment_position);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);

    float distance = length(light.position - fragment_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    vec3 ambient  = light.ambient  * lightColor;
    vec3 diffuse  = light.diffuse  * diff * lightColor;
    vec3 specular = light.specular * spec * lightColor;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{           
    vec3 color = texture(texture_diffuse1, vertex.uv).rgb;
    vec3 normal = normalize(vertex.normal);
    vec3 viewDir = normalize(viewPos - vertex.fragment_position);
    
    vec3 lighting = getDirLight(dirLight, normal, viewDir);

    for(int i = 0; i < 4; i++){
        lighting += getPointLight(pointLights[i], normal, vertex.fragment_position, viewDir);
    }
        

    lighting = lighting * color;    
    
    FragColor = vec4(lighting, 1.0);
}