#version 330 core

in GS{
	vec3 Color;
	vec3 Pos;
	vec3 Normal;
} fsIn;
out vec4 FragColor;

uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1;
	vec3 lightColor = vec3(1.0f,1.0f,1.0f);
	vec3 lightPos = vec3(1.5f,1.5f,1.5f);
    float specularStrength = 0.4;
	int Shininess = 32;

    vec3 ambient = ambientStrength * lightColor;

	
    vec3 norm = normalize(fsIn.Normal);
    vec3 lightDir = normalize(lightPos - fsIn.Pos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - fsIn.Pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (diffuse+ambient+specular) * fsIn.Color;
	FragColor = vec4(result, 1.0f);
}