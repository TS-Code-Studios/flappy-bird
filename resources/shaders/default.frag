#version 330 core

in vec2 textureCoords;

out vec4 FragColor;

uniform sampler2D image;
uniform vec3 spriteColor;

void main() {
    FragColor = vec4(spriteColor, 1.0) * texture(image, textureCoords);
}