varying vec4  PxColor;
varying vec4  position;
varying vec4  clipplane;
varying float clipping;

void kgm_main(out vec4 pos)
{
   v_UV = a_UV;
   PxColor = g_vColor * a_Color;
   position = g_mTran * vec4(a_Vertex, 1);
   pos = g_mProj * g_mView * position;

   clipplane = g_vClipPlane;
   clipping = 0.0;

   if (g_iClipping > 0)
   {
     clipping = 1.5;
   }
}

//Fragment Shader
varying vec4  PxColor;
varying vec4  position;
varying vec4  clipplane;
varying float clipping;

void kgm_distance(out float distance, in vec4 plane, in vec3 point)
{
  float d1 = plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
  float d2 = sqrt(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);

  if (d2 < 0.001)
    d2 = 0.001;

  distance = d1 / d2;
}

void kgm_main(out vec4 col)
{
  if (clipping == 1.5)
  {
    float distance = 0.0;
    kgm_distance(distance, clipplane, position.xyz);

    //if (distance <= 0.0)
    //  discard;
  }

  col = v_color * PxColor * texture2D(g_txColor, v_UV);
}
