#version 330 core

layout(location = 0) out vec4 frag_color;
layout(location = 1) out vec4 bright_color;

struct Color {
    samplerCube cube_texture;
    vec3 light_color;
    float gamma;
    vec3 bloom_threshold_color;
};

struct Fog {
    bool enabled;
    vec3 color;
    float density;
    float transition_power;
    float height;
};

in VS_OUT {
    vec3 texture_coordinates;
} fs_in;

uniform Color color;
uniform Fog fog;

vec3 calcFog(vec3 input_color) {
    vec3 result = mix(input_color, fog.color, fog.density);

    if (fs_in.texture_coordinates.y <= fog.height && fs_in.texture_coordinates.y >= -0.2f) {
        float distance = 0.0f;
        if (fs_in.texture_coordinates.y < 0.0f) {
            distance = fog.height + abs(fs_in.texture_coordinates.y);
		}
        else {
            distance = fog.height - fs_in.texture_coordinates.y;
		}

        float fog_power = 1.0f - exp(-distance * fog.transition_power);
        fog_power = clamp(fog_power, 0.0f, 1.0f);
        result = mix(result, fog.color, fog_power);
    }

    return result;
}

vec3 gammaCorrection(vec3 input_color) {
    vec3 result = pow(input_color, vec3(color.gamma));
    return result;
}

void main() {
    vec4 cube_texel = texture(color.cube_texture, fs_in.texture_coordinates);
    cube_texel.rgb = gammaCorrection(cube_texel.rgb);

    vec3 texel_color = cube_texel.rgb * color.light_color;

	if (fog.enabled) {
        texel_color = calcFog(texel_color);
	}

    frag_color = vec4(texel_color, 1.0f);

    float brightness = dot(frag_color.rgb, color.bloom_threshold_color);
    if (brightness > 1.0f) {
        bright_color = frag_color;
    }
    else {
        bright_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}