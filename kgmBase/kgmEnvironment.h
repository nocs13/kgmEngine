#pragma once
#include "kgmTypes.h"
#include "kgmString.h"


#include <stdlib.h>
#include <string.h>

class kgmFont;

class kgmEnvironment: public kgmObject
{
public:
 u32 wnd_width, wnd_height, 
      wnd_bpp, wnd_fscreen;

 kgmString dir_cwd, dir_data;

 float time_per_frame;

 char *map_keyboard,
	  *map_joystick,
	  *map_mouse;

 char* m_input;			//input map
public:
 kgmEnvironment(){
  wnd_width = 800;
  wnd_height = 600;
  wnd_bpp = 16;
  wnd_fscreen = 0;

  time_per_frame = 0.0f;

  map_keyboard = 0; //new char[128]; memset(map_keyboard, 0, 128);
  map_joystick = 0; //new char[32]; memset(map_joystick, 0, 32);
  map_mouse    = 0; //new char[8]; memset(map_mouse, 0, 8);

  m_input = 0;
 }
 ~kgmEnvironment(){
  delete [] map_keyboard;
  delete [] map_joystick;
  delete [] map_mouse;
 }

 bool load(char* path){
  return true;
 }
 bool store(char* path){
  return true;
 }

 void reset(){
  wnd_width = 800;
  wnd_height = 600;
  wnd_bpp = 16;
  wnd_fscreen = 0;
 }
};
