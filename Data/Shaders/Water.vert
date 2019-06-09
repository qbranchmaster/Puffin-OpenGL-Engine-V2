#version 330 core

layout(location = 0) in vec3 position;

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
};

struct DirectionalLight {
    bool enabled;
    vec3 direction;
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
};

out VS_OUT {
	vec3 directional_light_direction_VIEW;
	vec3 position_VIEW;
	vec4 clip_space;
	vec2 texture_coords;
	vec3 to_camera_vector_WORLD;
} vs_out;

uniform Matrices matrices;
uniform DirectionalLight directional_light;

uniform vec3 camera_position;
uniform int texture_tiling;

void main() {
	vec4 world_position = matrices.model_matrix * vec4(position, 1.0f);

	vs_out.directional_light_direction_VIEW = normalize(vec3(matrices.view_matrix *
		vec4(directional_light.direction, 0.0f)));
	vs_out.position_VIEW =  vec3(matrices.view_matrix * world_position);

	vs_out.clip_space = matrices.projection_matrix * matrices.view_matrix * world_position;
	vs_out.to_camera_vector_WORLD = normalize(camera_position - world_position.xyz);

	// Texture coordinates for DUDV and normal maps
	vs_out.texture_coords = vec2(position.x / 2.0f + 0.5f, position.z / 2.0f + 0.5f) *
		texture_tiling;

    gl_Position = vs_out.clip_space;
}