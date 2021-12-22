#version 330 core
out vec4 FragColor;

in GS{
	vec3 Pos;
	vec3 Normal;
	vec2 TexCoord;
} fsIn;


// texture sampler
uniform sampler2D texture1;


uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 pos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material = {
    0.01*vec3(1.0),
    0.55*vec3(1.0),
    0.7*vec3(1.0),
    0.25
};
uniform Light light = {
    vec3(1.5),

    vec3(1.0),
    vec3(1.0),
    vec3(1.0)
};

void main()
{
	vec3 color=vec3(texture(texture1, fsIn.TexCoord));
    vec3 finalAmbient = material.ambient * light.ambient;
    vec3 finalDiffuse = material.diffuse * light.diffuse;
    vec3 finalSpecular = material.specular * light.specular;


	
    vec3 norm = normalize(fsIn.Normal);
    vec3 lightDir = normalize(light.pos - fsIn.Pos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse =  diff * finalDiffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fsIn.Pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
    vec3 specular = spec * finalSpecular;  

    vec3 result = (finalAmbient + diffuse + specular) * color;
	FragColor =vec4(result, 1.0f);
}