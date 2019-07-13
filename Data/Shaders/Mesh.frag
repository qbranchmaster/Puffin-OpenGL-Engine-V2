#version 330 core
layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 bright_color;

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

    vec4 frag_pos_DIR_LIGHT;
} fs_in;

uniform Material material;
uniform Lighting lighting;
uniform Other other;
uniform ShadowMapping shadow_mapping;
uniform Fog fog;
uniform Matrices matrices;
uniform samplerCube skybox_texture;

vec3 calcFog(vec3 input_color) {
    float distance = length(fs_in.position_VIEW);

    float fog_power = 1.0f - exp(-fog.density * distance);
    fog_power = clamp(fog_power, 0.0f, 1.0f);
    vec3 result = mix(input_color, fog.color, fog_power);
    return result;
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

    float bias = 0.005f;

    for (int x = -shadow_mapping.pcf_filter_count;
        x <= shadow_mapping.pcf_filter_count; x++)
    {
        for (int y = -shadow_mapping.pcf_filter_count;
            y <= shadow_mapping.pcf_filter_count; y++)
        {
            float closest_depth = texture(shadow_mapping.shadow_map_texture,
                proj_coords.xy + vec2(x, y) * texel_size).r;

            if (current_depth > closest_depth + bias) {
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
    ambient = vec3(0.0f, 0.0f, 0.0f);
    diffuse = vec3(0.0f, 0.0f, 0.0f);
    specular = vec3(0.0f, 0.0f, 0.0f);

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

vec3 calculateLighting() {
    // Get base color
    vec3 ambient_color = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse_color = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular_color = vec3(0.0f, 0.0f, 0.0f);
    vec3 emissive_color = vec3(0.0f, 0.0f, 0.0f);

    if (material.has_ambient_texture) {
        ambient_color = texture(material.ambient_texture,
            fs_in.texture_coord_MODEL).rgb;
        ambient_color.rgb = gammaCorrection(ambient_color.rgb);
        // Use opacity texture as 'dirt' texture
        if (material.has_opacity_texture) {
            vec4 dirt_color = texture(material.opacity_texture,
                fs_in.texture_coord_MODEL);
            dirt_color.rgb = gammaCorrection(dirt_color.rgb);
            ambient_color = ambient_color * dirt_color.rgb;
        }
    }
    else {
        ambient_color = material.ka;
    }

    if (material.has_diffuse_texture) {
        diffuse_color = texture(material.diffuse_texture,
            fs_in.texture_coord_MODEL).rgb;
        diffuse_color.rgb = gammaCorrection(diffuse_color.rgb);
		// Use opacity texture as 'dirt' texture
        if (material.has_opacity_texture) {
            vec4 dirt_color = texture(material.opacity_texture,
                fs_in.texture_coord_MODEL);
            dirt_color.rgb = gammaCorrection(dirt_color.rgb);
            diffuse_color = diffuse_color * dirt_color.rgb;
        }
    }
    else {
        diffuse_color = material.kd;
    }

    if (material.has_specular_texture) {
        specular_color = texture(material.specular_texture,
            fs_in.texture_coord_MODEL).rgb;
    }
    else {
        specular_color = material.ks;
    }

    if (material.has_emissive_texture) {
        emissive_color = texture(material.emissive_texture,
            fs_in.texture_coord_MODEL).rgb;
        emissive_color.rgb = gammaCorrection(emissive_color.rgb);
    }
    else {
        emissive_color = material.ke;
    }

    if (!lighting.enabled) {
        return diffuse_color;
    }

    // Calculate lighting
    vec3 ambient_light_factor = vec3(0.0f, 0.0f, 0.0f);
    vec3 diffuse_light_factor = vec3(0.0f, 0.0f, 0.0f);
    vec3 specular_light_factor = vec3(0.0f, 0.0f, 0.0f);

    // Directional light
    calculateDirectionalLight(ambient_light_factor, diffuse_light_factor,
        specular_light_factor);

    // Light is calculated. Calculate final object color.
    vec3 final_color = vec3(0.0f, 0.0f, 0.0f);

    // Ambient
    vec3 ambient = ambient_light_factor * ambient_color;

    // Diffuse
    vec3 diffuse = diffuse_light_factor * diffuse_color;

    // Specular
    vec3 specular = specular_light_factor * specular_color;

    // Sum up all lights
    final_color = ambient + calculateShadow(fs_in.frag_pos_DIR_LIGHT) *
        (diffuse + specular);

    // Add emissive factor
    final_color = final_color + emissive_color * lighting.emission_factor;

    return final_color;
}

void main() {
    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);
    result_color = calculateLighting();

	// Skybox reflection on transparent material
	if (length(material.transparency) < 0.5f) {
		vec3 reflected = reflect(fs_in.position_VIEW, fs_in.normal_vector_VIEW);
		reflected = normalize(vec3(inverse(matrices.view_matrix) * vec4(reflected, 0.0f)));
		vec4 reflection_color = texture(skybox_texture, reflected);

		result_color = mix(result_color, reflection_color.rgb, 0.15f);
	}

    // Calculate transparency
    float tr_value = length(material.transparency);
    tr_value = clamp(tr_value, 0.0f, 1.0f);

    frag_color = vec4(result_color, tr_value);

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