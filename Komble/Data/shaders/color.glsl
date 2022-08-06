#version 120

struct Light          
{                     
  vec4   pos;         
  vec4   dir;         
};                    

#define MAX_LIGHTS 16 

uniform mat4   g_mView;           
uniform mat4   g_mProj;           
uniform mat4   g_mTran;           
uniform vec4   g_vColor;          
uniform vec4   g_vSpecular;       
uniform vec4   g_vClipPlane;      
uniform vec3   g_vUp;             
uniform vec3   g_vEye;            
uniform vec3   g_vLook;           
uniform float  g_fTime;           
uniform float  g_fShine;          
uniform float  g_fRandom;         
uniform float  g_fAmbient;        
uniform float  g_fLightPower;     
uniform int    g_iClipping = 0;   

uniform Light  g_sLights[MAX_LIGHTS]; 

varying vec3   v_N;               
varying vec3   v_V;               
varying vec3   v_Y;               
varying vec2   v_UV;              
varying float  v_I;               
varying float  v_shine;           
varying vec4   v_color;           
varying vec4   v_specular;        

attribute vec3 a_Vertex;          
attribute vec3 a_Normal;          
attribute vec4 a_Color;           
attribute vec2 a_UV;              ;

varying vec4  position;
varying float clipping;

void process(out vec4 pos)
{
   v_UV = a_UV;
   position = g_mTran * vec4(a_Vertex, 1);
   pos = g_mProj * g_mView * position;
   v_color = g_vColor;

   if (g_iClipping > 0)
   {
     clipping = dot(position, g_vClipPlane);
   }
   else
   {
     clipping = 1.0;
   }
}

void main(void)                    
{                                  
   vec4 position;                  
   v_specular = g_vSpecular;       
   v_color = g_vColor * a_Color;   
   process(position);             
   gl_Position = position;        
}

//Fragment Shader
#version 120

struct Light          
{                     
  vec4   pos;         
  vec4   dir;         
};                    

#ifdef GL_ES         
precision lowp float;
#endif               

uniform sampler2D g_txColor;
uniform sampler2D g_txNormal;
uniform sampler2D g_txSpecular;

varying vec3   v_N;
varying vec3   v_V;
varying vec3   v_Y;
varying vec2   v_UV;
varying float  v_I;
varying float  v_shine;
varying vec4   v_color;
varying vec4   v_specular;;

varying vec4  position;
varying float clipping;

void process(out vec4 col)
{
  if (clipping < 0.0)
    discard;

  col = v_color * texture2D(g_txColor, v_UV);
}

void main( void )
{
    vec4 color;                     
    process(color);                
    gl_FragColor = color; 
}
