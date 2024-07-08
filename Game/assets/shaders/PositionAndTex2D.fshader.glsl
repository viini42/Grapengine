#version 330 core

in vec2 out_texture_coords;
in vec4 out_color;

out vec4 fragColor;

uniform sampler2D u_texture;

void main()
{
  fragColor = texture2D(u_texture, out_texture_coords) * out_color;
}