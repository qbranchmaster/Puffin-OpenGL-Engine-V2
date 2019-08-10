#version 330 core

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 bright_color;

#define MAX_POINT_LIGHTS_COUNT 4

struct Material {
    bool has_ambient_texture;
    sampler2D ambient_texture;
    vec3 ka;

    bool has_diffuse_texture;
    sampler2D diffuse_texture;
    vec3 kd;

    bool has_specular_texture;
    sampler2D specular_texture;
    vec3 ks;

    bool has_emissive_texture;
    sampler2D emissive_texture;
    vec3 ke;

    bool has_normalmap_texture;
    sampler2D normalmap_texture;

    bool has_opacity_texture;
    sampler2D opacity_texture;

    int shininess;

    vec3 transparency;
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

struct Lighting {
    bool enabled;
    bool blinn_phong;
    DirectionalLight directional_light;
    float emission_factor;
};

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
    mat3 normal_matrix;
    mat4 dir_light_matrix;
};

struct Fog {
    bool enabled;
    vec3 color;
    float density;
};

struct Other {
    float gamma;
    vec3 bloom_threshold_color;
};

struct ShadowMapping {
    bool enabled;
    sampler2D shadow_map_texture;
    int shadow_map_size;
    int pcf_filter_count;
	float shadow_distance;
	samplerCube point_shadow_map_1;
	samplerCube point_shadow_map_2;
	samplerCube point_shadow_map_3;
	samplerCube point_shadow_map_4;
};

in VS_OUT {
    vec3 position_WORLD;
    vec3 position_VIEW;
    vec3 position_TANGENT;

    vec3 view_position_TANGENT;

    vec2 texture_coord_MODEL;

    vec3 normal_vector_VIEW;

    vec3 directional_light_direction_VIEW;
    vec3 directional_light_direction_TANGENT;

	vec3 point_light_position_VIEW[MAX_POINT_LIGHTS_COUNT];
    vec3 point_light_position_TANGENT[MAX_POINT_LIGHTS_COUNT];

    vec4 frag_pos_DIR_LIGHT;
} fs_in;

uniform Material material;
uniform Lighting lighting;
uniform Other other;
uniform ShadowMapping shadow_mapping;
uniform Fog fog;
uniform Matrices matrices;
uniform samplerCube skybox_texture;
uniform PointLight point_lights[MAX_POINT_LIGHTS_COUNT];
uniform int used_point_lights;

vec3 calcFog(vec3 input_color) {
    float distance = length(fs_in.position_VIEW);

    float fog_power = 1.0f - exp(-fog.density * distance);
    fog_power = clamp(fog_power, 0.0f, 1.0f);
    vec3 result = mix(input_color, fog.color, fog_power);
    return result;
}

float calcPointShadow(vec3 frag_pos, int light_index) {
	if (!point_lights[light_index].enabled) {
		return 0.0f;
	}

	vec3 frag_to_light = frag_pos - point_lights[light_index].position;
	float current_depth = length(frag_to_light);

	float bias = 0.1f;
	float offset = 0.1f;
	float shadow  = 0.0f;

	for (float x = -offset; x < offset; x += offset / (shadow_mapping.pcf_filter_count * 0.5f)) {
		for (float y = -offset; y < offset; y += offset / (shadow_mapping.pcf_filter_count * 0.5f)) {
			for (float z = -offset; z < offset; z += offset / (shadow_mapping.pcf_filter_count * 0.5f)) {
				float closest_depth = 0.0f;
				if (light_index == 0) {
					closest_depth = texture(shadow_mapping.point_shadow_map_1, frag_to_light +
						vec3(x, y, z)).r;
				}
				else if (light_index == 1) {
					closest_depth = texture(shadow_mapping.point_shadow_map_2, frag_to_light +
						vec3(x, y, z)).r;
				}
				else if (light_index == 2) {
					closest_depth = texture(shadow_mapping.point_shadow_map_3, frag_to_light +
						vec3(x, y, z)).r;
				}
				else if (light_index == 3) {
					closest_depth = texture(shadow_mapping.point_shadow_map_4, frag_to_light +
						vec3(x, y, z)).r;
				}

				// Recalculate from [0, 1] range to [0, ShadowDist]
				closest_depth *= shadow_mapping.shadow_distance;

				if (current_depth - bias > closest_depth) {
					shadow += 1.0f;
				}
			}
		}
	}

	shadow /= (shadow_mapping.pcf_filter_count * shadow_mapping.pcf_filter_count *
		shadow_mapping.pcf_filter_count);

	return shadow;
}

float calculateShadow(vec4 frag_pos) {
    if (!shadow_mapping.enabled) {
        return 1.0f;
    }

    vec3 proj_coords = frag_pos.xyz / frag_pos.w;
    proj_coords = proj_coords * 0.5f + 0.5f;

    float current_depth = proj_coords.z;
    if (current_depth > 1.0f) {
        return 1.0f;
    }

    float total_texels = (shadow_mapping.pcf_filter_count * 2.0f + 1.0f) *
        (shadow_mapping.pcf_filter_count * 2.0f + 1.0f);
    float texel_size = 1.0f / shadow_mapping.shadow_map_size;
    float total = 0.0f;

    for (int x = -shadow_mapping.pcf_filter_count;
        x <= shadow_mapping.pcf_filter_count; x++)
    {
        for (int y = -shadow_mapping.pcf_filter_count;
            y <= shadow_mapping.pcf_filter_count; y++)
        {
            float closest_depth = texture(shadow_mapping.shadow_map_texture,
                proj_coords.xy + vec2(x, y) * texel_size).r;

            if (current_depth > closest_depth) {
                total += 1.0f;
            }
        }
    }

    total /= total_texels;
    float shadow = 1.0f - total;

    return shadow;
}

vec3 gammaCorrection(vec3 input_color) {
    vec3 result = pow(input_color, vec3(other.gamma));
    return result;
}

void calculateDirectionalLight(inout vec3 ambient, inout vec3 diffuse,
    inout vec3 specular) {
    if (!lighting.directional_light.enabled) {
        return;
    }

    // Ambient
    ambient = lighting.directional_light.ambient_color;

    // Get normal vector
    vec3 normal_vector = vec3(0.0f, 1.0f, 0.0f);
    if (material.has_normalmap_texture)
    {
        vec3 normal_vector_TANGENT = texture(material.normalmap_texture,
            fs_in.texture_coord_MODEL).rgb;
        normal_vector_TANGENT = normalize(normal_vector_TANGENT * 2.0f - 1.0f);
        normal_vector = normal_vector_TANGENT;
    }
    else {
        normal_vector = fs_in.normal_vector_VIEW;
    }

    // Get view direction
    vec3 view_direction = vec3(0.0f, 1.0f, 0.0f);
     if (material.has_normalmap_texture) {
        vec3 view_direction_TANGENT = normalize(fs_in.view_position_TANGENT -
            fs_in.position_TANGENT);
        view_direction = view_direction_TANGENT;
    }
    else {
        vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
        view_direction = view_direction_VIEW;
    }

    // Get light direction
    vec3 light_direction = vec3(0.0f, 1.0f, 0.0f);
    if (material.has_normalmap_texture) {
        light_direction = fs_in.directional_light_direction_TANGENT;
    }
    else {
        light_direction = fs_in.directional_light_direction_VIEW;
    }

    // Diffuse
    float diffuse_power = max(dot(normal_vector, -light_direction), 0.0f);
    diffuse = lighting.directional_light.diffuse_color * diffuse_power;

    // Specular
    if (lighting.blinn_phong) {
        vec3 halfway = normalize(-light_direction + view_direction);
        float specular_power = pow(max(dot(normal_vector, halfway), 0.0f),
            material.shininess * 4);
        specular = lighting.directional_light.specular_color * specular_power;
    }
    else {
        vec3 reflected_ray = normalize(reflect(light_direction, normal_vector));
        float specular_power = pow(max(dot(reflected_ray, view_direction), 0.0f),
            material.shininess);
        specular = lighting.directional_light.specular_color * specular_power;
    }
}

void calculatePointLight(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular,
	int light_index) {
	if (!point_lights[light_index].enabled) {
		return;
	}

	float vertex_dist = length(fs_in.point_light_position_VIEW[light_index] - fs_in.position_VIEW);
	float attenuation = 1.0f / (1.0f + point_lights[light_index].linear_factor *
	    vertex_dist + point_lights[light_index].quadratic_factor * vertex_dist * vertex_dist);

	// Get light direction
	vec3 light_direction = vec3(0.0f, 1.0f, 0.0f);
	if (material.has_normalmap_texture) {
		vec3 light_direction_TANGENT = normalize(fs_in.point_light_position_TANGENT[light_index] -
			fs_in.position_TANGENT);
		light_direction = light_direction_TANGENT;
	}
	else {
		vec3 light_direction_VIEW = normalize(fs_in.point_light_position_VIEW[light_index] -
			fs_in.position_VIEW);
		light_direction = light_direction_VIEW;
	}

	// Get normal vector
	vec3 normal_vector = vec3(0.0f, 1.0f, 0.0f);
	if (material.has_normalmap_texture) {
		vec3 normal_vector_TANGENT = texture(material.normalmap_texture,
			fs_in.texture_coord_MODEL).rgb;
		normal_vector_TANGENT = normalize(normal_vector_TANGENT * 2.0f - 1.0f);
		normal_vector = normal_vector_TANGENT;
	}
	else {
		normal_vector = fs_in.normal_vector_VIEW;
	}

	// Get view direction
	vec3 view_direction = vec3(0.0f, 1.0f, 0.0f);
	if (material.has_normalmap_texture) {
		vec3 view_direction_TANGENT = normalize(fs_in.view_position_TANGENT -
			fs_in.position_TANGENT);
		view_direction = view_direction_TANGENT;
	}
	else {
		vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
		view_direction = view_direction_VIEW;
	}

	// Ambient
	ambient += point_lights[light_index].color * attenuation * 0.1f;

	// Diffuse
	float diffuse_power = max(dot(normal_vector, light_direction), 0.0f);
	diffuse += point_lights[light_index].color * diffuse_power * attenuation;

	// Specular
	vec3 reflected_ray = normalize(reflect(-light_direction, normal_vector));
	float specular_power = pow(max(dot(reflected_ray, view_direction), 0.0f),
		material.shininess);
	specular += point_lights[light_index].color * specular_power * attenuation;
}

vec4 calculateLighting() {
    // Get base color
    vec4 ambient_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 diffuse_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 specular_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 emissive_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    if (material.has_ambient_texture) {
        ambient_color = texture(material.ambient_texture, fs_in.texture_coord_MODEL);
        ambient_color.rgb = gammaCorrection(ambient_color.rgb);
        // Use opacity texture as 'dirt' texture
        if (material.has_opacity_texture) {
            vec4 dirt_color = texture(material.opacity_texture,
                fs_in.texture_coord_MODEL);
            dirt_color.rgb = gammaCorrection(dirt_color.rgb);
            ambient_color = ambient_color * dirt_color;
        }
    }
    else {
        ambient_color = vec4(material.ka, 1.0f);
    }

    if (material.has_diffuse_texture) {
        diffuse_color = texture(material.diffuse_texture,
            fs_in.texture_coord_MODEL);
        diffuse_color.rgb = gammaCorrection(diffuse_color.rgb);
		// Use opacity texture as 'dirt' texture
        if (material.has_opacity_texture) {
            vec4 dirt_color = texture(material.opacity_texture, fs_in.texture_coord_MODEL);
            dirt_color.rgb = gammaCorrection(dirt_color.rgb);
            diffuse_color = diffuse_color * dirt_color;
        }
    }
    else {
        diffuse_color = vec4(material.kd, 1.0f);
    }

    if (material.has_specular_texture) {
        specular_color = texture(material.specular_texture, fs_in.texture_coord_MODEL);
    }
    else {
        specular_color = vec4(material.ks, 1.0f);
    }

    if (material.has_emissive_texture) {
        emissive_color = texture(material.emissive_texture, fs_in.texture_coord_MODEL);
        emissive_color.rgb = gammaCorrection(emissive_color.rgb);
    }
    else {
        emissive_color = vec4(material.ke, 1.0f);
    }

    if (!lighting.enabled) {
        return diffuse_color;
    }

	vec4 final_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // Calculate lighting
    vec3 ambient_light_factor = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse_light_factor = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular_light_factor = vec3(0.0f, 0.0f, 0.0f);

    // Directional light
    calculateDirectionalLight(ambient_light_factor, diffuse_light_factor,
        specular_light_factor);

	float dir_light_shadow = calculateShadow(fs_in.frag_pos_DIR_LIGHT);

	final_color = vec4(ambient_light_factor, 1.0f) * ambient_color + dir_light_shadow *
		(vec4(diffuse_light_factor, 1.0f) * diffuse_color + vec4(specular_light_factor, 1.0f) *
		specular_color);

	// Point lights
	for (int i = 0; i < MAX_POINT_LIGHTS_COUNT; i++) {
		if (i < used_point_lights) {
			vec3 ambient_pf = vec3(0.0f);
			vec3 diffuse_pf = vec3(0.0f);
			vec3 specular_pf = vec3(0.0f);

			calculatePointLight(ambient_pf, diffuse_pf, specular_pf, i);
			float point_light_shadow = calcPointShadow(fs_in.position_WORLD, i);

			final_color += (vec4(ambient_pf, 1.0f) * ambient_color + (1.0f - point_light_shadow) *
				(vec4(diffuse_pf, 1.0f) * diffuse_color + vec4(specular_pf, 1.0f) * specular_color));
		}
	}

    // Add emissive factor
    final_color = final_color + emissive_color * lighting.emission_factor;

    return final_color;
}

void main() {
    vec4 result_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    result_color = calculateLighting();

	// Skybox reflection on transparent material
	if (length(material.transparency) < 0.5f) {
		vec3 reflected = reflect(fs_in.position_VIEW, fs_in.normal_vector_VIEW);
		reflected = normalize(vec3(inverse(matrices.view_matrix) * vec4(reflected, 0.0f)));
		vec4 reflection_color = texture(skybox_texture, reflected);

		result_color = mix(result_color, reflection_color, 0.20f);

        // Calculate transparency
        float tr_value = length(material.transparency);
        tr_value = clamp(tr_value, 0.0f, 1.0f);

        result_color = vec4(result_color.rgb, tr_value);
	}

    frag_color = result_color;

	if (fog.enabled) {
        frag_color.rgb = calcFog(frag_color.rgb);
	}

    float brightness = dot(frag_color.rgb, other.bloom_threshold_color);
    if (brightness > 1.0f) {
        bright_color = frag_color;
    }
    else {
        bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}