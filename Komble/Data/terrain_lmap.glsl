#define MAX_LIGHTS 32

//uniform vec4  g_vPosLights[MAX_LIGHTS];
//uniform vec4  g_vDirLights[MAX_LIGHTS];
//uniform vec4  g_vColLights[MAX_LIGHTS];
//uniform float g_fCntLights;

varying vec3  VV;
varying vec3  eye;
varying float clipping;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  vec4 position = g_mTran * vec4(a_Vertex, 1.0);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  VV = vec4(g_mView * vec4(v_V, 1.0)).xyz;

  v_N = normalize(mRot * a_Normal);

  eye = g_vEye;

  v_Y = -vec3(g_mView * vec4(v_V, 1.0));

  v_shine = g_fShine;

  v_UV = a_UV;

  if (g_iClipping > 0)
  {
    clipping = dot(position, g_vClipPlane);
  }
  else
  {
    clipping = 1.0;
  }

  pos = ( g_mProj * g_mView * vec4(v_V, 1.0) );
}

//Fragment Shader
#define MAX_LIGHTS 32

uniform vec4  g_vPosLights[MAX_LIGHTS];
uniform vec4  g_vDirLights[MAX_LIGHTS];
uniform vec4  g_vColLights[MAX_LIGHTS];
uniform float g_fCntLights;

varying vec3  VV;
varying vec3  eye;
varying float clipping;

void kgm_main( out vec4 color )
{
  if (clipping < 0.0)
    discard;

  vec3 NN = normalize(v_N);

  vec3 col = vec3(0.0);

  int count = int(g_fCntLights);

  for (int i = 0; i < count; i++)
  {
    float len = distance(g_vPosLights[i].xyz, v_V);

    vec3  lnor = normalize(g_vPosLights[i].xyz - v_V);

    float pow = max(0.1, dot(NN, lnor)) * g_vPosLights[i].w / (len + 1.0);

    vec3 lcol = g_vColLights[i].xyz * pow;

    col += lcol;
  }

  col = clamp(col, vec3(0.2), vec3(1.0));

  color = vec4(col, 1);
}
