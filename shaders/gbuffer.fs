#version 400
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gPicking;

in vec2 TexCoords;
in vec3 FragPos;

in mat3 TBN;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;

uniform vec4 color_diffuse;
uniform float power_specular;
uniform vec4 color_normal;

uniform bool has_diffuse;
uniform bool has_specular;
uniform bool has_normal;

uniform vec4 oID;

void main()
{    
    gPicking = oID;
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    if(has_normal)
    {
        gNormal = normalize(TBN * (texture(texture_normal, vec2(TexCoords.x, TexCoords.y)).rgb * 2.0 - 1.0));
    }
    else
        gNormal = vec3(0.0, 1.0, 0.0) * TBN;
    // and the diffuse per-fragment color
    if(has_diffuse)
        gAlbedoSpec.rgb = texture(texture_diffuse, TexCoords).rgb * color_diffuse.rgb;
    else
        gAlbedoSpec.rgb = color_diffuse.rgb;
        
    // store specular intensity in gAlbedoSpec's alpha component
    if(has_specular)
        gAlbedoSpec.a = texture(texture_specular, TexCoords).r * power_specular;
    else
        gAlbedoSpec.a = power_specular;
}