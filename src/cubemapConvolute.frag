#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

vec3 SphericalToCartesian(float theta, float phi)
{
    return vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
}

vec3 TangentToWorld(vec3 tangent, vec3 right, vec3 up, vec3 normal)
{
    return vec3(tangent.x * right + tangent.y * up + tangent.z * normal); 
}

vec3 getTextureCoordinates(float theta, float phi, vec3 right, vec3 up, vec3 normal)
{
    vec3 tangent = SphericalToCartesian(theta, phi);
    return TangentToWorld(tangent, right, up, normal);
}

vec3 getIrradiance(float theta, float phi, vec3 right, vec3 up, vec3 normal)
{
    vec3 coord = getTextureCoordinates(theta, phi, right, up, normal);
    return texture(environmentMap, coord).rgb * cos(theta) * sin(theta);
}

// Thanks to https://learnopengl.com/PBR/IBL/Diffuse-irradiance
void main()
{		
    vec3 normal = normalize(localPos);
    vec3 right = normalize(cross(vec3(0.0f, 1.0f, 0.0f), normal));
    vec3 up = normalize(cross(normal, right));

    const float sampleDelta = 0.025f;

    vec3 irradiance = vec3(0.0f);
    float nrSamples = 0.0f; 
    // Simply sample the map and get area irradiance
    for(float phi = 0.0f; phi < 2.0f * PI; phi += sampleDelta)
    {
        for(float theta = 0.0f; theta < 0.5f * PI; theta += sampleDelta)
        {
            irradiance += getIrradiance(theta, phi, right, up, normal);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0f / float(nrSamples));
  
    FragColor = vec4(irradiance, 1.0f);
}