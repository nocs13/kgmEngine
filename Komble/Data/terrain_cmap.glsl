#define MAX_LIGHTS 16

uniform vec4  g_vPosLights[MAX_LIGHTS];
uniform vec4  g_vDirLights[MAX_LIGHTS];
uniform vec4  g_vColLights[MAX_LIGHTS];
uniform float g_fCntLights;

uniform vec2 g_vUVScale;

varying vec4  position;
varying vec2  uvscale;
varying float clipping;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  vec4 position = g_mTran * vec4(a_Vertex, 1.0);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  v_N = normalize(mRot * a_Normal);

  v_UV = a_UV;

  uvscale = g_vUVScale;
  position = g_mTran * vec4(a_Vertex, 1);
  pos = g_mProj * g_mView * position;

  if (g_iClipping > 0)
  {
    clipping = dot(position, g_vClipPlane);
  }
  else
  {
    clipping = 1.0;
  }
}

//Fragment Shader
#define MAX_LIGHTS 16

uniform vec4  g_vPosLights[MAX_LIGHTS];
uniform vec4  g_vDirLights[MAX_LIGHTS];
uniform vec4  g_vColLights[MAX_LIGHTS];
uniform float g_fCntLights;

varying vec4  position;
varying vec2  uvscale;
varying float clipping;

uniform sampler2D g_txColor1;
uniform sampler2D g_txColor2;

void kgm_main(out vec4 col)
{
  if (clipping < 0.0)
    discard;

  vec2 uv1 = vec2(v_UV.x * uvscale.x, v_UV.y * uvscale.y);

  vec3 color0 = texture2D(g_txNormal,   uv1).rgb;
  vec3 color1 = texture2D(g_txSpecular, uv1).rgb;
  vec3 color2 = texture2D(g_txColor1,   uv1).rgb;
  vec3 color3 = texture2D(g_txColor2,   uv1).rgb;

  vec3 level = texture2D(g_txColor, v_UV).rgb;

  float blevel = 1.0 - (level.r + level.g + level.b);

  //vec3 color = blevel * color0  + level.x * color1 +
  //             level.y * color2 + level.z * color3;
  vec3 color = blevel  * color0;
       color += level.r * color1;
       color += level.g * color2;
       color += level.b * color3;

  vec3 icol = vec3(0.0);

  vec3 NN = normalize(v_N);

  int count = int(g_fCntLights);

  for (int i = 0; i < 1; i++)
  {
    float len = distance(g_vPosLights[i].xyz, v_V);

    vec3  lnor = normalize(g_vPosLights[i].xyz - v_V);

    float pow = 0.0;

    if (g_vPosLights[i].w < 0.0)
    {
      pow = max(0.1, dot(NN, lnor));
    }
    else if (length(g_vDirLights[i].xyz) < 0.9)
    {
      vec3 ldir = normalize(g_vDirLights[i].xyz);

      float angle = acos( max( dot(ldir, -lnor), 0.0) );

      if (angle > (0.5 * g_vDirLights[i].w))
        pow = 0.1;
      else
        pow = g_vDirLights[i].w * max(0.1, dot(NN, lnor)) / (1.0 + len);
    }
    else
    {
      pow = g_vDirLights[i].w * max(0.1, dot(NN, lnor)) / (1.0 + len);
    }

    pow = clamp(pow, 0.25, 0.99);

    vec3 lcol = g_vColLights[i].xyz * pow;

    icol += lcol;
  }

  icol = clamp(icol, vec3(0.2), vec3(1.0));

  col = vec4(v_color.rgb * color * icol, 1.0);
  //col = vec4(NN.rgb, 1.0);
  //col = vec4(color, 1.0);
  //col = vec4(1, 0, 0, 1.0);
}
