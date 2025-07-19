#version 330 core

#define inv_float(x) (1.0f - x)

// Shader to prefilter a hdr cubemap
// As all IBL and PBR concepts, implemented with the help of:
// https://learnopengl.com/PBR/IBL/Specular-IBL

out vec4 FragColor;

in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.1415926535897932385;
const uint SAMPLE_COUNT = 1024u;

// Mirror binary around its decimal point
// From: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html#subsec-vdCexample
float radicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10;
}

// Generate hammersley sequence
// Low discrepancy to increase resulting specular sample quality
vec2 hammersley2d(uint i, uint N) 
{
    float iDn = float(i) / float(N);
    float inv = radicalInverse_VdC(i);
    return vec2(iDn, inv);
}

// Get cartesian coordinates from spherical
vec3 SphericalToCartesian(vec2 Xi, float roughness)
{
    float phi = 2.0f * PI * Xi.x;
    float cosTheta = sqrt((inv_float(Xi.y)) / (1.0f + (pow(roughness, 4) - 1.0f) * Xi.y));
    float sinTheta = sqrt(inv_float(pow(cosTheta, 2)));
	
    return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 normal, float roughness)
{
    vec3 H = SphericalToCartesian(Xi, roughness);

    // tangent space to world space
    vec3 up = vec3(1.0f, 0.0f, 0.0f);
    if(abs(normal.z) < 0.999f)
        up = vec3(0.0f, 0.0f, 1.0f);
   
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);
	
    return normalize(tangent * H.x + bitangent * H.y + normal * H.z);
}  

float DistributionGGX(vec3 NdotH, float alpha)
{
    float alpha2 = pow(alpha, 2);
    float denom = pow(NdotH, 2) * (alpha2 - 1.0f) + 1.0f;
    denom = safe_div(PI * pow(denom, 2));
    return alpha2 / denom;
}

// Implemented with help from:
// https://chetanjags.wordpress.com/2015/08/26/image-based-lighting/
void main()
{		
    vec3 normal = normalize(localPos);    
    vec3 R = normal;
    vec3 V = R;

    float totalWeight = 0.0f;   
    vec3 prefilteredColor = vec3(0.0f);     
    for(uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        vec2 Xi = hammersley2d(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, normal, roughness);
        vec3 L = normalize(2.0f * dot(V, H) * H - V);

        float NdotL = max(dot(normal, L), 0.0f);
        prefilteredColor += texture(environmentMap, L).rgb * NdotL;
        //float NdotH = max(dot(normal, H), 0.0f);
        //float D = DistributionGGX(NdotH, roughness);
        //float pdf = (D * NdotH / (4.0 * HdotV)) + 0.0001; 
        //
        //float saTexel  = 4.0 * PI / (6.0 * pow(CUBEMAP_PER_FACE_RES, 2));
        //float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);
        //
        //float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel); 
        totalWeight += NdotL;

    }
    prefilteredColor /= totalWeight;

    FragColor = vec4(prefilteredColor, 1.0f);
}  