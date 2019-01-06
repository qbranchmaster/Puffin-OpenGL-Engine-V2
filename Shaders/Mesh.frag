#version 330 core

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

    int shininess;

    vec3 transparency;


    bool has_normalmap_texture;
    sampler2D normalmap_texture;
    float reflectivity;
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
    DirectionalLight directional_light;
};

struct Matrices {
    mat4 view_matrix;
    mat4 projection_matrix;
    mat4 model_matrix;
    mat3 normal_matrix;
};

in VS_OUT {
    vec3 position_WORLD;
    vec3 position_VIEW;

    vec2 texture_coord_MODEL;

    vec3 normal_vector_VIEW;

    vec3 directional_light_direction_VIEW;
} fs_in;

out vec4 frag_color;

uniform Material material;
uniform Lighting lighting;

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
    normal_vector = fs_in.normal_vector_VIEW;

    // Get view direction
    vec3 view_direction = vec3(0.0f, 1.0f, 0.0f);
    vec3 view_direction_VIEW = normalize(-fs_in.position_VIEW);
    view_direction = view_direction_VIEW;

    // Get light direction
    vec3 light_direction = vec3(0.0f, 1.0f, 0.0f);
    light_direction = fs_in.directional_light_direction_VIEW;

    // Diffuse
    float diffuse_power = max(dot(normal_vector, -light_direction), 0.0f);
    diffuse = lighting.directional_light.diffuse_color * diffuse_power;

    // Specular
    vec3 reflected_ray = normalize(reflect(light_direction, normal_vector));
    float specular_power = pow(max(dot(reflected_ray, view_direction), 0.0f),
        material.shininess);
    specular = lighting.directional_light.specular_color * specular_power;
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
    }
    else {
        ambient_color = material.ka;
    }

    if (material.has_diffuse_texture) {
        diffuse_color = texture(material.diffuse_texture,
            fs_in.texture_coord_MODEL).rgb;
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
    final_color = ambient + diffuse + specular;

    // Add emissive factor
    final_color = final_color + emissive_color;

    return final_color;
}

void main() {
    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);
    result_color = calculateLighting();

    // Calculate transparency
    float tr_value = length(material.transparency);
    tr_value = clamp(tr_value, 0.0f, 1.0f);

    frag_color = vec4(result_color, tr_value);
}