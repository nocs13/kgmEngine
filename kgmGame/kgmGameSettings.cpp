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
 FILE* file = fopen("kgmEngine.conf", "rb");

 if(!file)
	 return;

 char* str = new char[2048];

 while(!feof(file))
 {
   memset(str, 0, 2048);

   if(!fgets(str, 2048, file))
     break;

   if(strlen(str) == 0)
     continue;

   int si;

   for(int si = 0; si < strlen(str); si++)
     if(!isspace(str[si]))
       break;

   if(str[si] == '#')
     continue;

   char *s1 = &str[si];
   char *s2 = strchr(s1, ' ');

   if(!s2)
     continue;

   kgmString key(s1, (u32)(s2 - s1));

   while(*s2 != '\0'){
       if(isspace(*s2))
         s2++;
   }

   if(strlen(s2) > 0)
   {
       kgmString value(s2, strlen(s2));

       m_parameters.add(key, value);
   }
 }

 delete [] str;

 fclose(file);
}
void kgmGameSettings::save(){
 FILE* file = fopen(m_name, "wb");
 if(!file)
	 return;
 fclose(file);
}
