#define MAX_LIGHTS 32

uniform vec4  g_vPosLights[MAX_LIGHTS];
uniform vec4  g_vDirLights[MAX_LIGHTS];
uniform vec4  g_vColLights[MAX_LIGHTS];
uniform float g_vCntLights;

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

  v_I = g_vLight.w;

  v_L = g_vLight.xyz;

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
uniform vec4 g_vPosLights[MAX_LIGHTS];
uniform vec4 g_vDirLights[MAX_LIGHTS];
uniform vec4 g_vColLights[MAX_LIGHTS];

varying vec3  VV;
varying vec3  eye;
varying float clipping;

void kgm_main( out vec4 color )
{
  if (clipping < 0.0)
    discard;

  vec3 NN = normalize(v_N);
  vec3 LN = normalize(v_L - v_V);
  vec3 R  = normalize(-reflect(LN, NN));
  vec3 E  = normalize(v_Y - VV);

  vec4 col = texture2D(g_txColor, v_UV);
       col.rgb *= (v_color.rgb * v_L_color);

  //Normal
  {
    vec3 bump = texture2D(g_txNormal, v_UV).xyz * 2.0 - 1.0;
    bump = normalize(bump);
    NN   = normalize(NN + bump);
  }

  float distance = 1.0 + length(v_L - v_V);
  //float intensity = max(0.0, (v_I * dot(NN, LN) / distance));
  float intensity = max(0.0, dot(NN, LN) * (3.0 * v_I / distance));

  //Specular
  if(v_shine > 1.5)
  {
    vec3  specular = texture2D(g_txSpecular, v_UV).rgb;
    vec3  viewDir  = normalize(eye - v_V);
    vec3  reflDir  = reflect(-LN, NN);
    float spec     = pow(max(dot(viewDir, reflDir), 0.0), v_shine);

    specular = specular * spec;
    col.xyz +=  specular;
  }

  col.xyz *= intensity;
  col.xyz = clamp(col.xyz, 0.0, 1.0);

  color = col;
}
