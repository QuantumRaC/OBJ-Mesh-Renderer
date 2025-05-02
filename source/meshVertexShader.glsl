#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 MVP;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 UV; // for texture

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    FragPos = vec3(modelMatrix * vec4(position, 1.0));
    Normal = normalize(normalMatrix * normal);
    UV = texCoord; // for texture
}
