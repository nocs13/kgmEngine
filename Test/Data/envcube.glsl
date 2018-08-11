varying vec4 PxColor;
varying vec3 txCoord;
varying vec3 normal;
varying vec3 position;

void kgm_main(out vec4 pos)
{
   vec3 normal = normalize(a_Normal);

   v_UV = a_UV;
   txCoord.x = a_Vertex.x;
   txCoord.y = a_Vertex.y;
   txCoord.z = a_Vertex.z;
   PxColor = g_vColor * a_Color;
   //txCoord = (transpose(g_mView * g_mTran) * vec4(normal, 1.0)).xyz;
   txCoord = (transpose(g_mView * g_mTran) * vec4(a_Vertex, 1.0)).xyz;
   //position = g_mView * g_mTran * vec4(a_Vertex, 0.0)

   pos = g_mProj * g_mView * g_mTran * vec4(a_Vertex, 1);
}

//Fragment Shader
varying vec3 txCoord;
varying vec4 PxColor;
uniform samplerCube g_txEnvironment;

void kgm_main(out vec4 col)
{
  col = PxColor * textureCube(g_txEnvironment, txCoord);
  //col = vec4(1, 0, 0, 1);
}
