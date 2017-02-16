
#ifndef __DISPATCHERINT_H
#define __DISPATCHERINT_H 1

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char			uint8   ;
typedef char					int8    ;
typedef unsigned short int		uint16  ;
typedef short int				int16   ;
typedef unsigned int			uint32  ;
typedef int						int32   ;

#if defined(WIN32) || defined(_WINDOWS)
typedef unsigned __int64		uint64  ;
typedef __int64					int64   ;
#else

#include <stdint.h>
typedef uint64_t 				uint64  ;
typedef int64_t					int64   ;
#endif

enum TTFieldType{
	TTUNTYPE ,
	TTINT8 ,
	TTUINT8 ,
	TTUINT16 ,
	TTINT16 ,
	TTUINT32 ,
	TTINT32 ,
	TTUINT64 ,
	TTINT64 ,
	TTFLOAT ,
	TTDOUBLE ,
	TTBOOL ,
	TTTIMESTAMP ,
	TTSTRING ,
	TTRAWDATA
} ;


#endif  /** __DISPATCHERINT_H */



