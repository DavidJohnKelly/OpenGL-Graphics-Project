#version 330 core

#define safe_div(x) (max(x, 0.000001f))
#define inv_float(x) (1.0f - x)

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

#define PI 3.1415926535897932384626433832795

// Struct to store all pbr material information
struct Material
{
    vec3  albedo;
    float ao;
    float roughness;
    float metallic;

    vec3  normal;
};

struct Light 
{    
    // Point and Spot
    bool  emit;
    vec3  colour;
    float strength;
    vec3  position;

    // Spot
    vec3  direction;
    float cutoff;
    float outerCutoff;
};  

// Struct for pbr lighting calculations
struct LightPBR
{
    vec3  colour;
    float strength;
    vec3  direction;
};

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_arm;
uniform sampler2D texture_displacement;

uniform samplerCube irradianceMap;

uniform Light pointLights[MAX_POINT_LIGHTS];
uniform Light spotLights[MAX_SPOT_LIGHTS];

uniform int numPointLights;
uniform int numSpotLights;

in VERT_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 ViewPos;
    vec3 ViewDir;
    vec3 Normal;
    mat3 TBN; // Try to find a way to avoid doing this in fragment shader. Inefficient
} frag_in;

out vec4 FragColor;

// Concepts from https://learnopengl.com/PBR/Theory
// Mostly just standard maths formulas for the reflectance equation

// Used for microfacets in order to simulate roughness
float DistributionGGX(vec3 N, vec3 H, float alpha)
{
    float alpha2 = pow(alpha, 2);
    float NdotH = max(dot(N, H), 0.0f);
    float denom  = (pow(NdotH, 2) * (alpha2 - 1.0f) + 1.0f);
    denom = safe_div(PI * pow(denom, 2));
    return alpha2 / denom;
}

// Approximate self shadowing on microfacets
float GeometrySchlickGGX(vec3 N, vec3 X, float k)
{
    float NdotX = safe_div(dot(N,X)); // avoid artefacts on low viewing angles
    float denom = safe_div(NdotX * inv_float(k) + k);
    return NdotX / denom;
}
  
// Main function to approximate microfacets on surface
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float ggx1 = GeometrySchlickGGX(N, V, k / 2.0f);
    float ggx2 = GeometrySchlickGGX(N, L, k / 2.0f);
    return ggx1 * ggx2;
}

// Aproximate fresnel term
// Used for viewing angles in pbr
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + inv_float(F0) * pow(inv_float(cosTheta), 5.0f);
}

// Same equation, just taking roughness into account for IBL
// Basically accounts for different environment lighting maps by adjusting roughness as needed
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(inv_float(roughness)), F0) - F0) * pow(clamp(inv_float(cosTheta), 0.0f, 1.0f), 5.0f);
}  

// Only uses IBL for diffuse
// In other words, use for ambient lighting, not specular reflections
vec3 CalcPBR(Material material, LightPBR light, vec3 F0, vec3 viewDir)
{
    vec3 L = normalize(light.direction);
    vec3 H = normalize(viewDir + L);

    // Scale distance based on light strength
    float distance = length(light.direction) * (1.0f / light.strength);
    float attenuation = 1.0f / pow(distance, 2); // Inverse square law for attenuation
    vec3 radiance = light.colour * attenuation;        
        
    // cook-torrance brdf
    float NDF = DistributionGGX(material.normal, H, material.roughness);        
    float G = GeometrySmith(material.normal, viewDir, L, material.roughness);      
    vec3 F = fresnelSchlick(max(dot(H, viewDir), 0.0f), F0);       
        
    vec3 kD = (vec3(1.0f) - F) * inv_float(material.metallic);
        
    float denom = 4.0f * safe_div(dot(material.normal, viewDir)) * safe_div(dot(material.normal, L));
    vec3 specular = (NDF * G * F) / denom;  // Need to add specular IBL
            
    float NdotL = max(dot(material.normal, L), 0.0005f);              
    return (kD * material.albedo / PI + specular) * radiance * NdotL; 
}

vec3 CalcPointLight(Light light, Material material, vec3 F0, vec3 viewDir)
{
    light.position = frag_in.TBN * light.position;
    vec3 lightDirection = light.position - frag_in.FragPos;

    LightPBR pbrLight;
    pbrLight.colour = light.colour;
    pbrLight.strength = light.strength;
    pbrLight.direction = lightDirection;

    return CalcPBR(material, pbrLight, F0, viewDir);
} 

vec3 CalcSpotLight(Light light, Material material, vec3 F0, vec3 viewDir) 
{
    light.position = frag_in.TBN * light.position;
    vec3 lightDir = light.position - frag_in.FragPos;

    // Calculate cone of the spotlight
    float cosTheta = dot(normalize(lightDir), normalize(-frag_in.TBN * light.direction));
    float cosOuterTheta = cos(radians(light.outerCutoff));
    
    if (cosTheta < cosOuterTheta) {
        return vec3(0.0);
    }

    float cosInnerTheta = cos(radians(light.cutoff));
    float spotFactor = smoothstep(cosOuterTheta, cosInnerTheta, cosTheta); // Avoid conditional
    
    LightPBR pbrLight;
    pbrLight.strength = light.strength;
    pbrLight.colour = light.colour;
    pbrLight.direction = lightDir;

    vec3 lighting = CalcPBR(material, pbrLight, F0, viewDir);

    return (lighting * spotFactor);
}


// Just following the standard IBL Diffuse formula for the reflectance equation
vec3 CalcAmbientIBL(Material material, vec3 F0)
{
    vec3 kS = fresnelSchlickRoughness(max(dot(frag_in.Normal, frag_in.ViewDir), 0.0f), F0, material.roughness);
    vec3 kD = inv_float(kS) * inv_float(material.metallic);	  
    vec3 irradiance = texture(irradianceMap, frag_in.Normal).rgb;
    vec3 diffuse = irradiance * material.albedo;
    return (kD * diffuse) * material.ao;
}

void main()
{
    Material material;

    // Approximate albedo from diffuse texture
    vec3 albedo = texture(texture_albedo, frag_in.TexCoords).rgb;
    albedo = pow(albedo, vec3(2.2f));
    material.albedo = albedo;

    // ARM textures
    material.ao = texture(texture_arm, frag_in.TexCoords).r;
    material.roughness = texture(texture_arm, frag_in.TexCoords).g;
    material.metallic = texture(texture_arm, frag_in.TexCoords).b;

    // Normal mapping
    vec3 normal = texture(texture_normal, frag_in.TexCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);
    material.normal = normal;

    vec3 F0 = vec3(0.04f); 
    F0 = mix(F0, material.albedo, material.metallic);

    vec3 result = vec3(0.0f);

    // Add all lighting information
    for(int i = 0; i < numPointLights; i++)
    {
        if(pointLights[i].emit)
        {
             result += CalcPointLight(pointLights[i], material, F0, frag_in.ViewDir);
        }
    }

    for(int i = 0; i < numSpotLights; i++)
    {
        if(spotLights[i].emit)
        {
             result += CalcSpotLight(spotLights[i], material, F0, frag_in.ViewDir);
        }
    }

    // IBL ambient lighting
    result += CalcAmbientIBL(material, F0);
    // HDR tonemapping
    result = result / (result + vec3(1.0));
    // Gamma correction
    result = pow(result, vec3(1.0/2.2));  
    FragColor = vec4(result, 1.0f);
}