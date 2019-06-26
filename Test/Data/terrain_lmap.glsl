#define MAX_LIGHTS 32

//uniform vec4  g_vPosLights[MAX_LIGHTS];
//uniform vec4  g_vDirLights[MAX_LIGHTS];
//uniform vec4  g_vColLights[MAX_LIGHTS];
//uniform float g_vCntLights;

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
uniform float g_vCntLights;

varying vec3  VV;
varying vec3  eye;
varying float clipping;

void kgm_main( out vec4 color )
{
  if (clipping < 0.0)
    discard;

  float intensity = 0.0;

  for (int i = 0; i < g_vCntLights; i++)
  {
    float len = distance(g_vPosLights[i].xyz, v_V);

    vec3 ldir = normalize(g_vDirLights[i].xyz);

    if (ldir == vec3(0.0))
    {
      ldir = normalize(v_V - g_vPosLights[i].xyz);
    }

    float pow = dot(normalize(v_N), ldir);// * g_vPosLights[i].w / (len + 1.0);

    intensity += pow;
  }

  vec3 ldir = normalize(v_V.xyz);

  color = vec4(ldir, 1);
  //color = vec4(intensity, intensity, intensity, 1);
}
