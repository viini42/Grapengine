#version 330 core

#define MAX_LIGHTS 2

in vec2 out_texture_coords;
in vec4 out_color;
in vec3 out_normal;
in vec3 out_frag_pos;

out vec4 fragColor;

uniform sampler2D u_texture;

uniform vec3 u_ambientColor;
uniform float u_ambientStrength;

uniform vec3 u_lightPos[MAX_LIGHTS];
uniform vec3 u_lightColor[MAX_LIGHTS];
uniform float u_lightStrength[MAX_LIGHTS];

void main()
{
  // Get fragment normal
  vec3 frag_normal = normalize(out_normal);

  // Calculate ray direction
  vec3 light_directions[MAX_LIGHTS];
  for (int i = 0; i < MAX_LIGHTS; i++)
  light_directions[i] = normalize(u_lightPos[i] - out_frag_pos);


  // Calculate ray inclination
  vec3 diffuses = vec3(0, 0, 0);
  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    float cosine = dot(frag_normal, light_directions[i]);// -1 to 1
    float diff = max(0, cosine);
    float dist_inv = 1 / distance(out_frag_pos, u_lightPos[i]);
    diffuses = diffuses + (dist_inv * u_lightStrength[i] * diff * u_lightColor[i]);
  }

  vec3 ambient = u_ambientStrength * u_ambientColor;

  vec3 object_color = (texture2D(u_texture, out_texture_coords) * out_color).rgb;
  vec3 result = (ambient + diffuses) * object_color;
  fragColor = vec4(result, out_color.a);
}