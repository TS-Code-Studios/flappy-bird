#version 330 core

in vec2 textureCoords;
in vec4 vertexColor;

out vec4 FragColor;

uniform sampler2D image;

void main() {
    FragColor = vec4(vertexColor) * texture(image, textureCoords);
}