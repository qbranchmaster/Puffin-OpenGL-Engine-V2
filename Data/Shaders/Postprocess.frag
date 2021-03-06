#version 330 core

struct Color {
    sampler2D screen_texture;
    sampler2D depth_texture;

    sampler2D glow_bloom_texture;
    bool glow_bloom_enabled;

    int effect;
    vec3 tint_color;
    float kernel_size;

    float gamma;
    float exposure;

    bool dof_enabled;
    float aperture;
    float focus_distance;
    float dof_max_blur;
    float camera_aspect;
};

struct ChromaticAberration {
	bool enabled;
	sampler2D lens_texture;
	vec3 max_channels_offset;
};

in VS_OUT {
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform Color color;
uniform ChromaticAberration chromatic_aberration;

vec3 depthOfFieldTexturesSample(vec2 tex_coords, vec2 coords_offset,
    vec2 blur_value, vec2 aspect_correct) {
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    result = vec3(texture2D(color.screen_texture, tex_coords.xy +
        (coords_offset * aspect_correct) * blur_value) +
        texture2D(color.glow_bloom_texture, tex_coords.xy + (coords_offset *
        aspect_correct) * blur_value));

    return result;
}

vec3 depthOfField(vec3 input_color, vec2 input_coords) {
    vec2 uv = input_coords;
    vec2 aspectcorrect = vec2(1.0f, color.camera_aspect);
    vec4 depth = texture2D(color.depth_texture, uv);
    float factor = depth.x - color.focus_distance;
    vec2 dof_blur = vec2(clamp(factor * color.aperture, -color.dof_max_blur,
        color.dof_max_blur));

    vec2 dof_blur9 = dof_blur * 0.9f;
    vec2 dof_blur7 = dof_blur * 0.7f;
    vec2 dof_blur4 = dof_blur * 0.4f;

    vec3 dof_color = vec3(0.0f);
    dof_color += input_color;

    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f,   0.40f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.15f,  0.37f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f,  0.29f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.37f,  0.15f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.40f,  0.0f),  dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.37f, -0.15f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f, -0.29f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.15f, -0.37f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f,  -0.4f),  dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.15f,  0.37f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f,  0.29f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.37f,  0.15f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.4f,   0.0f),  dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.37f, -0.15f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f, -0.29f), dof_blur, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.15f, -0.37f), dof_blur, aspectcorrect);

    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.15f,  0.37f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.37f,  0.15f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.37f, -0.15f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.15f, -0.37f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.15f,  0.37f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.37f,  0.15f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.37f, -0.15f), dof_blur9, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.15f, -0.37f), dof_blur9, aspectcorrect);

    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f,  0.29f), dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.40f,  0.0f),  dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f, -0.29f), dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f, - 0.4f),  dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f,  0.29f), dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.4f,   0.0f),  dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f, -0.29f), dof_blur7, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f,   0.4f),  dof_blur7, aspectcorrect);

    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f,  0.29f), dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.4f,   0.0f),  dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.29f, -0.29f),  dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f,  -0.4f),  dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f,  0.29f), dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.4f,   0.0f),  dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2(-0.29f, -0.29f), dof_blur4, aspectcorrect);
    dof_color += depthOfFieldTexturesSample(uv, vec2( 0.0f,   0.4f),  dof_blur4, aspectcorrect);

    dof_color = dof_color / 41.0f;

    return dof_color;
}

vec3 sampleAccumTextures(vec2 tex_coords) {
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	if (chromatic_aberration.enabled) {
		float offset = texture2D(chromatic_aberration.lens_texture, tex_coords).r;
		offset = clamp(offset, 0.0f, 1.0f);

		vec2 r_offset = vec2(chromatic_aberration.max_channels_offset.r * offset, 0.0f);
		vec2 g_offset = vec2(chromatic_aberration.max_channels_offset.g * offset, 0.0f);
		vec2 b_offset = vec2(chromatic_aberration.max_channels_offset.b * offset, 0.0f);

		vec4 r_value = texture2D(color.screen_texture, tex_coords - r_offset);
		vec4 g_value = texture2D(color.screen_texture, tex_coords - g_offset);
		vec4 b_value = texture2D(color.screen_texture, tex_coords - b_offset);

		result = vec3(r_value.r, g_value.g, b_value.b);
	}
	else {
		result = texture(color.screen_texture, tex_coords).rgb;
	}

    if (color.glow_bloom_enabled) {
		result = result + texture(color.glow_bloom_texture, tex_coords).rgb;
    }

    // Depth of field
    if (color.dof_enabled) {
        result = depthOfField(result, tex_coords);
    }

    return result;
}

vec3 noEffect() {
    return sampleAccumTextures(fs_in.tex_coord);
}

vec3 negative() {
    return vec3(1.0f - noEffect());
}

vec3 grayScale() {
    vec3 texel_color = noEffect();
    float average = (texel_color.r + texel_color.g + texel_color.b) / 3.0f;
    return vec3(average, average, average);
}

vec3 tintColor() {
    vec3 texel_color = noEffect();
    return texel_color * color.tint_color;
}

vec3 calcPostprocess(vec2 offsets[9], float kernel[9]) {
    vec3 sample_tex[9];
    for (int i = 0; i < 9; i++) {
        sample_tex[i] = sampleAccumTextures(fs_in.tex_coord.st + offsets[i]);
    }

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 9; i++) {
        result += sample_tex[i] * kernel[i];
    }

    return result;
}

vec3 sharpen(vec2 offsets[9]) {
    float kernel[9] = float[](
        -1.0f, -1.0f, -1.0f,
        -1.0f,  9.0f, -1.0f,
        -1.0f, -1.0f, -1.0f);

    return calcPostprocess(offsets, kernel);
}

vec3 blur(vec2 offsets[9]) {
    float kernel[9] = float[](
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    );

    return calcPostprocess(offsets, kernel);
}

vec3 edge(vec2 offsets[9]) {
    float kernel[9] = float[](
        1.0f,  1.0f, 1.0f,
        1.0f, -8.0f, 1.0f,
        1.0f,  1.0f, 1.0f);

    return calcPostprocess(offsets, kernel);
}

void main() {
    float offset = 1.0f / color.kernel_size;
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),
        vec2(0.0f, offset),
        vec2(offset, offset),
        vec2(-offset, 0.0f),
        vec2(0.0f, 0.0f),
        vec2(offset, 0.0f),
        vec2(-offset, -offset),
        vec2(0.0f, -offset),
        vec2(offset, -offset));

    vec3 result_color = vec3(0.0f, 0.0f, 0.0f);

    switch (color.effect) {
    case 0: // None
        result_color = noEffect();
        break;
    case 1: // Negative
        result_color = negative();
        break;
    case 2: // Grayscale
        result_color = grayScale();
        break;
    case 3: // Sharpen
        result_color = sharpen(offsets);
        break;
    case 4: // Blur
        result_color = blur(offsets);
        break;
    case 5: // Edge
        result_color = edge(offsets);
        break;
    case 6: // Tint
        result_color = tintColor();
        break;
    }

    // Tone mapping
    vec3 mapped = vec3(1.0f) - exp(-result_color * color.exposure);

    // Gamma correction
    mapped = pow(mapped, vec3(1.0f / color.gamma));

    frag_color = vec4(mapped, 1.0f);
}