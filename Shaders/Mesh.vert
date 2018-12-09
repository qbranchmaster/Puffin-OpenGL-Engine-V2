#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal_vector;

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

out vec3 nor;

uniform Matrices matrices;

void main() {
    nor = normalize(vec3(matrices.view_matrix * matrices.model_matrix *
		vec4(normal_vector, 0.0f)));
    gl_Position = matrices.projection_matrix * matrices.view_matrix *
        matrices.model_matrix * vec4(position, 1.0f);
}