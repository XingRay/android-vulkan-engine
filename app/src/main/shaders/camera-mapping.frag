#version 450
#extension GL_ARB_separate_shader_objects: enable

layout (binding = 0) uniform sampler2D cam_sampler;

layout (location = 0) in vec2 i_tex_coords;

layout (location = 0) out vec4 o_color;

void main() {
    o_color = texture(cam_sampler, i_tex_coords);
}
