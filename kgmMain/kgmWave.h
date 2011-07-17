// KRIFF.h: interface for the KRIFF class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;


struct WAVE_FMT{
 ushort wFormatTag; 
 ushort nChannels; 
 uint nSamplesPerSec; 
 uint nAvgBytesPerSec; 
 ushort nBlockAlign; 
 ushort wBitsPerSample; 
 ushort cbSize; 
}; 

class kgmWave  
{
public:
uint      riff_id;
uint      riff_size;
uint      riff_format;
uint      fmt_id;
uint      fmt_size;
WAVE_FMT  fmt_format;
uint      data_id;
uint      data_size;
char*     data; 

public:

kgmWave(){
 data = 0;
}
virtual ~kgmWave(){
 if(data)
	 free(data);
}

bool openWav(char *file)
{
    FILE *in = 0l;

	if(!file || !strlen(file))
		return false;
	in = fopen(file,"rb");
	if(!in)
		return false;

	fread((char*)&riff_id,4,1,in);
	fread((char*)&riff_size,4,1,in);
	fread((char*)&riff_format,4,1,in);

	fread((char*)&fmt_id,4,1,in);
	fread((char*)&fmt_size,4,1,in);
	fread((char*)&fmt_format,16,1,in); //16
	fseek(in,fmt_size - 16,SEEK_CUR);

	fread((char*)&data_id,4,1,in);
	fread((char*)&data_size,4,1,in);



	if(data_size > 0)
	{
 	 data = new char[data_size];
	 if(!data){
	  fclose(in);
	  return false;
	 }
	 else
      fread(data, data_size, 1, in);
	}

	fclose(in);
	return true;
}

bool from_mem(char *mem, uint msize)
{
 if(!mem)
 	return false;
 char *pM = mem;

 memcpy(&riff_id, pM, sizeof(int)); pM += 4;
 memcpy(&riff_size, pM, sizeof(int)); pM += 4;
 memcpy(&riff_format, pM, sizeof(int)); pM += 4;
 memcpy(&fmt_id, pM, sizeof(int)); pM += 4;
 memcpy(&fmt_size, pM, sizeof(int)); pM += 4;
 memcpy(&fmt_format, pM, sizeof(WAVE_FMT)); pM += fmt_size;
 memcpy(&data_id, pM, sizeof(int)); pM += 4;
 memcpy(&data_size, pM, sizeof(int)); pM += 4;
 data = new char[data_size];
 memcpy(data, pM, data_size);
 fmt_format.cbSize = sizeof(fmt_format);
 return true;
}
};
