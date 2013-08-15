#include "kgmGameSettings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static short g_gsid = 0x0101;

kgmGameSettings::kgmGameSettings(){

  m_gsid = 0xf0f0f0f0;
  m_mouse_camera = true;
  m_name = "kgmEngine.conf";

  load();
}

kgmGameSettings::kgmGameSettings(const char* path){
  m_gsid = 0xf0f0f0f0;
  m_mouse_camera = true;

  if(path && strlen(path) < 128)
    m_name = path;
  else
    m_name = "kgmEngine.conf";

  load();
}

kgmGameSettings::~kgmGameSettings(){
  //save();
}

void kgmGameSettings::load(){
  FILE* file = fopen(m_name.data(), "r");

  if(!file)
    return;

  //char* str = new char[2048];

  while(!feof(file))
  {
    char key[64] = {0};
    char val[128] = {0};

    fscanf(file, "%s %s", key, val);

    if(!strlen(key) || key[0] == '#')
      continue;

    kgmString s_key(key);
    kgmString s_val(val);

    m_parameters.add(s_key, s_val);
  }


  fclose(file);
}

void kgmGameSettings::save(){
  FILE* file = fopen(m_name.data(), "w");

  if(!file)
    return;

  for(int i = 0; i < m_parameters.length(); i++)
  {
    kgmString key, val;

    m_parameters.get(i, key, val);

    fprintf(file, "%s %s", (char*)key, (char*)val);
  }

  fclose(file);
}

kgmString kgmGameSettings::get(char* key)
{
  kgmString val;
  m_parameters.get(key, val);

  return val;
}

