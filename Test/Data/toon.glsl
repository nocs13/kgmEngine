varying vec3 L_Dir;
varying vec3 L_Pos;
varying vec3 Y_Dir;
varying float shininess;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  v_N = normalize(mRot * a_Normal);

  Y_Dir = g_vEyeDir;

  L_Pos = g_vLight.xyz;

  if (length(g_vLightDirection.xyz) == 0.0)
  {
    L_Dir = g_vLight.xyz - v_V.xyz;
  }
  else
  {
    L_Dir = g_vLightDirection.xyz;
  }

  v_UV = a_UV;

  shininess = g_fShine;

  pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 L_Dir;
varying vec3 L_Pos;
varying vec3 Y_Dir;
varying vec3  specular;
varying float shininess;

void kgm_main(out vec4 col)
{
  float intensity;
  vec4 color;
  vec3 specular = vec3(0, 0, 0);

  intensity = dot(normalize(L_Dir), normalize(v_N));

  if (intensity > 0.95)
    intensity = 1.0;
  else if (intensity > 0.5)
    intensity = 0.6;
  else if (intensity > 0.25)
    intensity = 0.4;
  else
    intensity = 0.2;

  if (dot(v_N, L_Dir) >= 0.0)
  {
    vec3 L = normalize(L_Pos - v_V);
    vec3 E = normalize(v_V);
    vec3 R = normalize(reflect(L_Dir, v_N));
    specular = v_specular.xyz * pow(max(dot(R, E), 0.0), shininess);
    //specular = v_specular.xyz * pow(max(dot(v_N, -Y_Dir), 0.0), shininess);

    specular = clamp(specular, 0.0, 0.8);
  }

  color.xyz = intensity * v_color.xyz + specular;
  color.xyz = clamp(color.xyz, 0.0, 1.0);
  color.w   = v_color.w;

  col = color;
}
