attribute vec2 a_UV1;

varying vec4  position;
varying vec2  v_UV1;
varying float clipping;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   v_UV1 = a_UV1;
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
varying vec2  v_UV1;
varying float clipping;

uniform sampler2D g_txColor1;
uniform sampler2D g_txColor2;


void kgm_main(out vec4 col)
{
  if (clipping < 0.0)
    discard;

  vec3 color0 = texture2D(g_txNormal,   v_UV1);
  vec3 color1 = texture2D(g_txSpecular, v_UV1);
  vec3 color2 = texture2D(g_txColor1,   v_UV1);
  vec3 color3 = texture2D(g_txColor2,   v_UV1);

  vec3 level = texture2D(g_txColor, v_UV);

  float blevel = 1.0 - (level.x + level.y + level.z);

  vec3 color = blevel * color0  + level.x * color1 +
               level.y * color2 + level.z * color3;

  col = vec4(v_color * color, 1.0);
}
