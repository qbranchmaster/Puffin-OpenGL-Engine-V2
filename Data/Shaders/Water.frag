#version 330 core

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 bright_color;

#define MAX_POINT_LIGHTS_COUNT 4

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

struct PointLight {
    bool enabled;
    vec3 position;
    vec3 color;
    float linear_factor;
    float quadratic_factor;
};

struct WaterTile {
	vec3 water_color;
	float wave_strength;
	float move_factor;
	int shininess;
	float ambient_factor;
	float specular_factor;
};

struct Postprocess {
    float gamma;
    vec3 bloom_threshold_color;
};

struct Fog {
    bool enabled;
    vec3 color;
    float density;
};

in VS_OUT {
	vec3 directional_light_direction_VIEW;
	vec3 position_VIEW;
	vec4 clip_space;
	vec2 texture_coords;
	vec3 to_camera_vector_WORLD;

	vec3 point_light_position_VIEW[MAX_POINT_LIGHTS_COUNT];
} fs_in;

uniform Matrices matrices;
uniform WaterTile water_tile;
uniform DirectionalLight directional_light;
uniform Postprocess postprocess;
uniform Fog fog;

uniform PointLight point_lights[MAX_POINT_LIGHTS_COUNT];
uniform int used_point_lights_count;

uniform sampler2D reflection_texture;
uniform sampler2D refraction_texture;
uniform sampler2D dudv_map;
uniform sampler2D normal_map;

vec3 calcFog(vec3 input_color) {
    float distance = length(fs_in.position_VIEW);

    float fog_power = 1.0f - exp(-fog.density * distance);
    fog_power = clamp(fog_power, 0.0f, 1.0f);
    vec3 result = mix(input_color, fog.color, fog_power);
    return result;
}

vec3 calcDirectionalLight(vec3 normal_vector) {
	if (!directional_light.enabled) {
		return vec3(0.0f, 0.0f, 0.0f);
	}

	vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	// Ambient
    ambient = (directional_light.ambient_color + directional_light.diffuse_color) / 2.0f *
		water_tile.ambient_factor;

	// Specular
	vec3 reflected_ray = normalize(reflect(fs_in.directional_light_direction_VIEW, normal_vector));
	vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
	float specular_power = pow(max(dot(reflected_ray, view_direction_VIEW), 0.0f),
		water_tile.shininess);
	specular = directional_light.specular_color * specular_power * water_tile.specular_factor;

	return (ambient + specular);
}

vec3 calcPointLight(vec3 normal_vector)
{
    vec3 ambient = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < used_point_lights_count; i++) {
		if (!point_lights[i].enabled) {
			continue;
		}

		float vertex_dist = length(fs_in.point_light_position_VIEW[i] - fs_in.position_VIEW);
		float attenuation = 1.0f / (1.0f + point_lights[i].linear_factor *
			vertex_dist + point_lights[i].quadratic_factor * vertex_dist * vertex_dist);

		vec3 light_direction_VIEW = normalize(fs_in.point_light_position_VIEW[i] -
			fs_in.position_VIEW);
		vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);

		// Ambient
		ambient += point_lights[i].color * attenuation;

		// Specular
		vec3 reflected_ray = normalize(reflect(-light_direction_VIEW, normal_vector));
		float specular_power = pow(max(dot(reflected_ray, view_direction_VIEW), 0.0f),
			water_tile.shininess);
		specular += point_lights[i].color * specular_power * attenuation;
	}

    return (ambient + specular);
}

vec3 gammaCorrection(vec3 input_color) {
    vec3 result = pow(input_color, vec3(postprocess.gamma));
    return result;
}

void main() {
	// Convert clip space to NDC
	vec2 ndc = (fs_in.clip_space.xy / fs_in.clip_space.w) / 2.0f + 0.5f;

	vec2 refract_tex_coords = ndc;
	vec2 reflection_tex_coords = vec2(ndc.x, 1.0f - ndc.y);

	// Calculate DUDV distortion value. RGB colors are from range [0.0, 1.0].
	// Distortion can also be negative, so scale color values to [-1.0, 1.0] range.
	// Use wave strength value to minimize distortion a bit.
	vec2 distorted_tex_coords = texture(dudv_map, vec2(fs_in.texture_coords.x +
		water_tile.move_factor, fs_in.texture_coords.y)).rg * 0.1f;
	distorted_tex_coords = fs_in.texture_coords + vec2(distorted_tex_coords.x,
		distorted_tex_coords.y + water_tile.move_factor);
	vec2 total_distortion = (texture(dudv_map, distorted_tex_coords).rg * 2.0f - 1.0f) *
		water_tile.wave_strength;

	refract_tex_coords += total_distortion;
	// After adding distortion, texture coordinates can be lower than 0.0 or higher than 1.0.
	// It can cause glitches. Clamping these values to values near 0.0 and 1.0 fixes the problem.
	refract_tex_coords = clamp(refract_tex_coords, 0.001f, 0.999f);

	reflection_tex_coords += total_distortion;
	reflection_tex_coords = clamp(reflection_tex_coords, 0.001f, 0.999f);

	// Fresnel effect
	float refractive_factor = dot(fs_in.to_camera_vector_WORLD, vec3(0.0f, 1.0f, 0.0f));
	// Higher the pow factor is, the water is more reflective and has less retraction
	refractive_factor = pow(refractive_factor, 0.5f);

    vec4 reflection_color = texture(reflection_texture, reflection_tex_coords);
	vec4 refraction_color = texture(refraction_texture, refract_tex_coords);

	vec4 result_color = mix(reflection_color, refraction_color, refractive_factor);
	result_color = mix(result_color, vec4(water_tile.water_color, 1.0f), 0.2f);

	// Calculate lighting
	vec4 normal_map_color = texture(normal_map, distorted_tex_coords);
	vec3 normal_vector = vec3(normal_map_color.r * 2.0f - 1.0f, normal_map_color.b,
		normal_map_color.g * 2.0f - 1.0f);
	normal_vector = vec3(matrices.view_matrix * vec4(normal_vector, 0.0f));
	normal_vector = normalize(normal_vector);

	vec3 result_dir_lighting = calcDirectionalLight(normal_vector);
	vec3 result_point_lighting = calcPointLight(normal_vector);

	result_color.rgb = (result_dir_lighting + result_point_lighting) * result_color.rgb;

	// Output color
	frag_color = result_color;

	if (fog.enabled) {
		frag_color.rgb = calcFog(frag_color.rgb);
	}

	float brightness = dot(frag_color.rgb, postprocess.bloom_threshold_color);
    if (brightness > 1.0f) {
        bright_color = frag_color;
    }
    else {
        bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}