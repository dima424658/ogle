#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct lightPoint
{
	vec3 position;
	vec3 color;
    float linear;
    float quadratic;
};

uniform int lightCount;
uniform lightPoint lights[16];

uniform vec3 viewPos;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    vec3 lighting  = Diffuse; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);

    for(int i = 0; i < lightCount; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].color;
        // specular
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].color * spec * Specular;
        // attenuation
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;        
    }      

    FragColor = vec4(lighting, 1.0);
}
