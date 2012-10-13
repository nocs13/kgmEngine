#pragma once
#include <inttypes.h>

#ifndef null
  #define null 0L
#endif

#ifndef nil
  #define nil 0L
#endif

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned int			u32;

typedef char				s8;
typedef short				s16;
typedef int				s32;

typedef float				f32;
typedef double				f64;

typedef int64_t				s64;
typedef int64_t         		u64;

/*#ifdef WIN32
 typedef __int64		u64;
 typedef __int64		s64;
#endif

#ifdef LINUX
 typedef unsigned	long long	int	u64;
 typedef signed	long long	int	s64;
#endif*/






