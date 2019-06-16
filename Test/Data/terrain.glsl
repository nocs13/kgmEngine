uniform vec2 g_vUVScale;

varying vec4  position;
varying vec2  uvscale;
varying float clipping;

void kgm_main(out vec4 pos)
{
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
  vec3 color.rgb = level;//  = blevel  * color0;
       //color += level.r * color1;
       //color += level.g * color2;
       //color += level.b * color3;

  //col = vec4(v_color.xyz * color, 1.0);
  col = vec4(level, 1.0);
}
