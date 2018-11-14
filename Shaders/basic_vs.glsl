#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 coords;

out vec2 tex_coords;

struct Matrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

uniform Matrices matrices;

void main() {
    tex_coords = coords;
    gl_Position = matrices.projection_matrix * matrices.view_matrix *  matrices.model_matrix *
        vec4(position, 1.0);
}