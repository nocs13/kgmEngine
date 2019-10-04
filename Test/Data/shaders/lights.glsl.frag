#version 450
#define MAX_LIGHTS 8

layout(binding = 1) uniform sampler2D txColor;
layout(binding = 2) uniform sampler2D txNormal;
layout(binding = 3) uniform sampler2D txSpecular;
layout(binding = 4) uniform sampler2D txFlexible;


struct Light
{
  vec3  pos;
  vec3  dir;
  vec3  col;
  float angle;
  float power;
};

struct Data
{
  vec4 color;
  vec4 position;
  vec4 posinview;
  vec3 nor;
  vec3 eye;
  vec3 eyedir;
  vec2 uv;

  float shine;
  float lcount;

  Light lights[MAX_LIGHTS];
};

layout(location = 0) in Data  data;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec4 tcolor     = texture(txColor,    data.uv);
  vec4 tnormal    = texture(txNormal,   data.uv);
  vec4 tspecular  = texture(txSpecular, data.uv);
  vec4 spec       = vec4(0.0);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(tnormal.xyz + normalize(data.nor));
  vec3 e = normalize(data.eye);
  vec3 l = normalize(data.lights[0].pos);

  float intensity = data.lights[0].power * max(dot(n,l), 0.1);

  vec4 lcolor = vec4(data.lights[0].col, 1.0);

  for (int i = 0; i < data.lcount; i++)
  {
    vec3  l  = normalize(data.lights[i].pos);
    float sh = dot(l, data.eye);

    intensity += (data.lights[i].power) * max(dot(data.nor, l), 0.01);
  }

  {
    vec3 h = normalize(l + e);
    float kspec = max(dot(h, n), 0.0);
    spec = tspecular * pow(kspec, data.shine * 100.0);
  }

  vec4 fcolor = vec4(0, 0, 0, 0);
  fcolor.xyz  = intensity * tcolor.xyz + spec.xyz;
  fcolor.w    = tcolor.w;

  fragColor = vec4(normalize(data.lights[0].pos), 1.0);
  //fragColor = tnormal;
}
