#version 450
#define MAX_LIGHTS 8

layout(binding = 1) uniform sampler2D txColor;
layout(binding = 2) uniform sampler2D txNormal;
layout(binding = 3) uniform sampler2D txSpecular;
layout(binding = 4) uniform sampler2D txFlexible;


struct Data
{
  vec4 color;
  vec4 position;
  vec4 posinview;

  vec4 lightpos[MAX_LIGHTS];
  vec4 lightdir[MAX_LIGHTS];
  vec4 lightcol[MAX_LIGHTS];

  vec3 nor;
  vec3 eye;
  vec3 eyedir;
  vec2 uv;

  float shine;
  float lcount;
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
  vec3 n = normalize(b + normalize(data.nor));
  vec3 e = normalize(data.eye);

  vec3 lnor = vec3(0.0);
  vec3 lpos = vec3(0.0);

  float intensity = 0.0;

  for (int i = 0; i < MAX_LIGHTS; i++)
  {
    vec3  l  = normalize(data.lightpos[i].xyz);

    if (data.lightpos[i].w < 0.001)
      break;

    lpos += l;

    intensity += dot(data.nor, l);
  }

  vec4 fcolor = vec4(0, 0, 0, 0);
  fcolor.xyz  = normalize(lpos); //vec3(1.0, 1.0, 1.0) * intensity;
  fcolor.w    = tcolor.w;

  fragColor = fcolor;
  //fragColor = tnormal;
}
