#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 UV; // NEW

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 viewPos;

uniform vec3 specColor1;
uniform vec3 diffColor1;
uniform vec3 specColor2;
uniform vec3 diffColor2;

uniform vec3 ambientColor1;
uniform vec3 ambientColor2;
uniform sampler2D texture1; // for texture

uniform int chosenPart;

out vec4 color;

void main() {
    vec3 texColor = texture(texture1, UV).rgb; // for texture

    vec3 ambient = ambientColor1 + ambientColor2;
    vec3 norm = normalize(Normal);

    vec3 lightDir1 = normalize(lightPos1 - FragPos);
    vec3 lightDir2 = normalize(lightPos2 - FragPos);

    float diff1 = max(dot(norm, lightDir1), 0.0);
    float diff2 = max(dot(norm, lightDir2), 0.0);

    vec3 diffuse = diff1 * diffColor1 + diff2 * diffColor2;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    vec3 reflectDir2 = reflect(-lightDir2, norm);

    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32);

    vec3 specular = spec1 * specColor1 + spec2 * specColor2;

    vec3 result = (ambient + diffuse + specular) * texColor;

    if (chosenPart == 1)
        result *= vec3(1.2); // highlight

    color = vec4(result, 1.0);
}
