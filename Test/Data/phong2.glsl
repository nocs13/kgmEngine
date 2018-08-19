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
varying vec3  VV;
varying vec3  eye;
varying float clipping;

struct Light
{
  vec4 position;
  vec4 direction;
  vec4 color;
};

uniform vec3   g_vEye;
uniform vec3   g_vLook;
uniform float  g_fShine;
uniform Light  g_vLights[8];

void kgm_main( out vec4 color )
{
  if (clipping < 0.0)
    discard;

  vec3 col;

  vec3 normal = normalize(v_N);
  vec3 view   = normalize(g_vEye - v_V);

  {
    vec3 lightDir = normalize(-g_vLights[0].direction.xyz);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(g_vLook, reflectDir), 0.0), g_fShine);

    // combine results
    vec3 diffuse  = g_vLights[0].color.xyz  * diff * vec3(texture2D(g_txColor, v_UV));
    vec3 specular = spec * vec3(texture2D(g_txSpecular, v_UV));

    //col = (diffuse + specular);
    col = vec3(texture2D(g_txColor, v_UV));
  }

  color.xyz = col;
}
