#version 330 core
out vec4 FragColor;
  

in vec2 o_texCoord;
in vec3 o_normal;  
in vec3 o_fragPos;
uniform sampler2D ourTexture;
uniform vec3 camPos;




struct DirectionalLight{
 vec3 direction;
 vec3 color;
};

struct PointLight {    
    vec3 position;
    vec3 function;  
    vec3 color;
};
#define NR_POINT_LIGHTS 1  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionalLight dirLight;
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec4 texColor = texture(ourTexture, o_texCoord);

    vec3 norm = normalize(o_normal);
    vec3 viewDir = normalize(camPos - o_fragPos);
    vec3 result = calculateDirectionalLight(dirLight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += calculatePointLight(pointLights[i], norm, o_fragPos, viewDir); 
    }
    
    FragColor = vec4(result, 1.0) * texColor;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    vec3 ambient = 0.1 * light.color;

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.8 * spec * light.color;  
    
    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.function.x + light.function.y * distance + 
  			     light.function.z * (distance * distance));    
    // combine results
    vec3 ambient  = light.color * 0.1;
    vec3 diffuse  = light.color  * diff;
    vec3 specular = light.color * spec * 0.8 ;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}