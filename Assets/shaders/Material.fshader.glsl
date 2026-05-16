#version 400 core

#define MAX_LIGHTS 100
#define MAX_TEXTURES 16

in vec2 out_texture_coords;
in vec4 out_color;
in vec3 out_normal;
in vec3 out_frag_pos;
flat in int out_tex_id;

out vec4 fragColor;

uniform sampler2D u_textures[MAX_TEXTURES];

uniform vec3 u_ambientColor;
uniform float u_ambientStrength;

uniform vec3 u_lightPos[MAX_LIGHTS];
uniform vec3 u_lightColor[MAX_LIGHTS];
uniform float u_lightStrength[MAX_LIGHTS];
uniform float u_specularStrength[MAX_LIGHTS];
uniform float u_specularShininess[MAX_LIGHTS];

uniform int u_lights_count;

uniform vec3 u_viewPos;

float get_attenuation(int i)
{
  float dist = max(distance(out_frag_pos, u_lightPos[i]), 0.25);
  float constant = 1.0;
  float linear = 0.09;
  float quadratic = 0.032;
  return 1.0 / (constant + linear * dist + quadratic * dist * dist);
}

vec3 get_ambient()
{
  vec3 ambient = u_ambientStrength * u_ambientColor;
  return ambient;
}

vec3 get_diffuse(in vec3 frag_normal, in vec3 light_directions[MAX_LIGHTS], in int lights_count)
{
  vec3 diffuses = vec3(0, 0, 0);
  for (int i = 0; i < lights_count; i++)
  {
    float cosine = dot(frag_normal, light_directions[i]);
    float diff = max(0, cosine);
    float atten = get_attenuation(i);
    diffuses = diffuses + (atten * u_lightStrength[i] * diff * u_lightColor[i]);
  }
  return diffuses;
}

vec3 get_specular(in vec3 frag_normal, in vec3 light_directions[MAX_LIGHTS], in int lights_count)
{
  vec3 view_direction = normalize(u_viewPos - out_frag_pos);
  vec3 speculars = vec3(0, 0, 0);
  for (int i = 0; i < lights_count; i++)
  {
    vec3 reflect_dir = reflect(-light_directions[i], frag_normal);
    float spec = pow(max(dot(view_direction, reflect_dir), 0.0), u_specularShininess[i]);

    float atten = get_attenuation(i);
    speculars = speculars + atten * u_specularStrength[i] * spec * u_lightColor[i];
  }
  return speculars;
}

void main()
{
  vec3 frag_normal = normalize(out_normal);
  int lights_count = min(u_lights_count, MAX_LIGHTS);

  vec3 light_directions[MAX_LIGHTS];
  for (int i = 0; i < lights_count; i++)
  {
    light_directions[i] = normalize(u_lightPos[i] - out_frag_pos);
  }

  vec4 texel = vec4(1.0);
  if (out_tex_id >= 0 && out_tex_id < MAX_TEXTURES)
    texel = texture(u_textures[out_tex_id], out_texture_coords);

  vec3 object_color = out_color.rgb * texel.rgb;
  float alpha = out_color.a * texel.a;
  vec3 result =
    (get_ambient() + get_diffuse(frag_normal, light_directions, lights_count)
     + get_specular(frag_normal, light_directions, lights_count))
    * object_color;
  fragColor = vec4(result, alpha);
}
