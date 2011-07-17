#include "kgmGameSettings.h"
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
 FILE* file = fopen(m_name, "rb");
 if(!file)
	 return;
 fread(&m_gsid, 2, 1, file);
 fread(&m_width, 4, 1, file);
 fread(&m_height, 4, 1, file);
 fclose(file);
}
void kgmGameSettings::save(){
 FILE* file = fopen(m_name, "wb");
 if(!file)
	 return;
 fclose(file);
}
