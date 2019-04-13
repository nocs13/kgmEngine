varying vec3  VV;
varying vec3  eye;
varying vec4  px_color;

varying float clipping;

void kgm_main(out vec4 pos)
{
  mat3  mRot = mat3(g_mTran[0][0], g_mTran[0][1], g_mTran[0][2],
                    g_mTran[1][0], g_mTran[1][1], g_mTran[1][2],
                    g_mTran[2][0], g_mTran[2][1], g_mTran[2][2]);

  vec4 position = g_mTran * vec4(a_Vertex, 1.0);

  v_V  = vec4(g_mTran * vec4(a_Vertex, 1.0)).xyz;

  VV = vec4(g_mView * vec4(v_V, 1.0)).xyz;

  //v_N = normalize(mRot * a_Normal);
  v_N = a_Normal;

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

  px_color = g_vColor * a_Color;
  pos = ( g_mProj * g_mView * vec4(v_V, 1.0) );
}

//Fragment Shader
varying vec3  VV;
varying vec3  eye;
varying vec4  px_color;

varying float clipping;

struct Light
{
  vec4 position;
  vec4 direction;
  vec4 color;
};

uniform vec3   g_vEye;
uniform vec3   g_vLook;
uniform vec4   g_vSpecular;
uniform float  g_fShine;
uniform Light  g_vLights[8];

void kgm_main( out vec4 color )
{
  if (clipping < 0.0)
    discard;

  vec3 col = px_color.xyz;

  vec3 normal = normalize(v_N);
  vec3 view   = normalize(g_vEye - v_V);

  {
    vec3 bump  = vec3(texture2D(g_txNormal, v_UV).xyz * 2.0 - 1.0);

    normal = normalize(normal + bump);
  }

  {
    vec3 lightDir = g_vLights[0].direction.xyz;

    float ll = length(lightDir);

    if (ll < 0.2)
    {
      lightDir = v_V - g_vLights[0].position.xyz;
    }

    lightDir = normalize(lightDir);

    // diffuse shading
    float diff = 0.9 * max(dot(normal, -lightDir), 0.0);
    float angle = acos(max(dot(normal, -lightDir), 0.0));

    diff = clamp(diff, 0.01, 0.91);

    if (g_vLights[0].direction.w > 0.0)
    {
      if (angle > g_vLights[0].direction.w)
      {
        diff *= 0.1;
      }
    }

    // specular shading
    vec3  reflectDir = reflect(lightDir, normal);
    vec3  specular   = vec3(0, 0, 0);
    float shininess  = clamp(g_fShine, 0, 512);
    float spec       = pow(max(dot(g_vLook, -reflectDir), 0.0), shininess);

    if (shininess > 1.0)
    {
      vec3 scolor = g_vSpecular.xyz + texture2D(g_txSpecular, v_UV).xyz;

      diff *= 0.7;
      scolor = clamp(scolor, 0.0, 1.0);
      specular = diff * spec * scolor;
      specular = clamp(specular, 0.0, 1.0);
    }

    // combine results
    vec3 diffuse  = diff * g_vLights[0].color.xyz * vec3(texture2D(g_txColor, v_UV));

    col *= diffuse;
    col += specular;
  }

  for (int i = 1; i < 8; i++)
  {
    float lintensity = g_vLights[i].position.w;

    if (lintensity < 0.1)
      break;

    vec3 lcolor    = g_vLights[i].color.xyz;
    vec3 lposition = g_vLights[i].position.xyz;
    vec3 lightDir  = normalize(lposition - v_V);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(g_vLook, reflectDir), 0.0), g_fShine);

    // attenuation
    float distance    = length(lposition - v_V);
    float attenuation = lintensity / (.5 + distance);

    // combine results
    vec3 diffuse  = lcolor * diff * vec3(texture2D(g_txColor, v_UV));
    vec3 specular = spec * vec3(texture2D(g_txSpecular, v_UV));

    specular = clamp(specular, 0.0, 1.0);

    diffuse  *= attenuation;
    specular *= attenuation;

    col += (diffuse + specular);
    //col += (diffuse);
  }

  color.xyz = col;
}
