#pragma once

typedef unsigned int			uint;
typedef unsigned short			ushort;
typedef unsigned char			uchar;

typedef unsigned char			uint8;
typedef signed   char			int8;

typedef unsigned short			ushort16;
typedef signed   short			short16;

typedef unsigned int			uint32;
typedef signed   int			int32;

typedef float					float32;
typedef double					float64;

typedef unsigned char			u8;
typedef signed   char			s8;

typedef unsigned short			u16;
typedef signed   short			s16;

typedef unsigned int			u32;
typedef signed   int			s32;

typedef float					f32;
typedef double					f64;

#ifdef WIN32
 //typedef unsigned __int64		u64;
 //typedef signed   __int64		s64;
#endif

#ifdef LINUX
 typedef unsigned	long long		u64;
 typedef signed		long long		s64;
#endif




