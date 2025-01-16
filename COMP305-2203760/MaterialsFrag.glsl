#version 330 core
out vec4 FragColor;
  
struct Material {
    vec3 ambient; // Colour the surface reflects for ambient lighting. (usually the same as base colour)
    vec3 diffuse; // Colour of the surface for diffuse lighting.
    vec3 specular; // Sets the colour of the specular highlight on the surface.
    float shininess; // The higher the shininess, the more intense the reflection. (the intensity of the specular highlight)
};

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform bool hasTexture;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse - based on the light's direction vector and the object's normal vectors.
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); // Difference vector between the light's position and the fragment's position.
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular - based on the same as diffuse and the camera view direction
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    // End colour
    vec3 result = (ambient + diffuse) * (hasTexture ? texture(texture_diffuse1, TexCoords).xyz : vec3(1.0f)) + specular;
    FragColor = vec4(result, 1.0);
}