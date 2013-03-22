#include "kgmGameSettings.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static short g_gsid = 0x0101;

kgmGameSettings::kgmGameSettings(){
 m_gsid = 0xf0f0f0f0;
 load();
}

kgmGameSettings::~kgmGameSettings(){
 save();
}

void kgmGameSettings::load(){
 FILE* file = fopen("kgmEngine.conf", "r");

 if(!file)
	 return;

 char* str = new char[2048];

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
 FILE* file = fopen("kgmEngine.conf", "wb");

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
