#version 330 core

in VS_OUT {
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform sampler2D image;
uniform bool horizontal;
uniform float weight[5] = float[](0.227027f, 0.1945946f, 0.1216216f, 0.054054f,
    0.016216f);

void main() {
    vec2 tex_offset = 1.0 / textureSize(image, 0);
    vec3 result = texture(image, fs_in.tex_coord).rgb * weight[0];

    if (horizontal) {
        for (int i = 1; i < 5; i++) {
            result += texture(image, fs_in.tex_coord +
                vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
            result += texture(image, fs_in.tex_coord -
                vec2(tex_offset.x * i, 0.0f)).rgb * weight[i];
        }
    }
    else {
        for (int i = 1; i < 5; i++) {
            result += texture(image, fs_in.tex_coord +
                vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, fs_in.tex_coord -
                vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
        }
    }

    frag_color = vec4(result, 1.0f);
}