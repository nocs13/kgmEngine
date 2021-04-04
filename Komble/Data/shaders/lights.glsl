
varying vec4   light;
varying vec4   lights[12];

void main(void)
{
  vec4 pos = g_mTran * vec4(g_Vertex, 1.0);

  position = vec3(pos);
  normal   = normalize(g_Normal);

  shininess    = g_fShine;
  eye          = g_vEye;
  light.xyz    = vec3(g_vLight.xyz - pos.xyz);
  light.w      = g_vLight.w / length(light.xyz);

  for(int i = 0; i < g_iLights; i++)
  {
    vec3 ldir = pos.xyz - g_vLights[i].xyz;
    lights[i].w = g_vLights[i].w / length(ldir);
    lights[i].xyz = normalize(ldir);
  }

  gl_Position  = g_mProj * g_mView * pos;
  texcoord     = g_Texcoord;
}

//Fragment Shader
varying vec4      light;
varying vec4      lights[12];


void main( void )
{
  vec4 tcolor     = texture2D(g_txColor,    texcoord);
  vec4 tnormal    = texture2D(g_txNormal,   texcoord);
  vec4 tspecular  = texture2D(g_txSpecular, texcoord);
  vec4 spec       = vec4(0.0);

  vec3 b = normalize(tnormal.xyz * 2.0 - 1.0);
  vec3 n = normalize(tnormal.xyz + normalize(normal));
  vec3 e = normalize(eye);
  vec3 l = normalize(light.xyz);


  float intensity = light.w * max(dot(n,l), 0.1);


  for(int i = 0; i < g_iLights; i++)
  {
    vec3  l  = normalize(lights[i].xyz);
    float sh = dot(l, eye);

    intensity += (lights[i].w) * max(dot(normal,l), 0.01);
  }


  {
    vec3 h = normalize(l + e);
    float kspec = max(dot(h, n), 0.0);
    spec = tspecular * pow(kspec, shininess * 100.0);
  }

  vec4 fcolor = vec4(0,0,0,0);
  fcolor.xyz  = intensity * tcolor.xyz + spec.xyz;
  fcolor.w     = tcolor.w;
  gl_FragColor = fcolor;
}
