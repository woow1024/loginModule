#ifndef __TTINT_H
#define __TTINT_H

#if defined(WIN32) || defined(_WINDOWS) || defined(MINGW) || defined(MINGW32)
#include <windows.h>
#else
#include <stdint.h>
#endif

namespace tt{

#if defined(WIN32) || defined(_WINDOWS) || defined(MINGW) || defined(MINGW32)

	typedef char				int8		 ;
	typedef unsigned char		uint8       ;
	typedef short int			int16       ;
	typedef unsigned short int	uint16      ;
	typedef int					int32       ;
	typedef unsigned int		uint32      ;
	typedef __int64           int64       ;
	typedef unsigned __int64	uint64      ;

#if defined(WIN32) || defined(__WINDOWS)
	#define atoll _atoi64
#endif
#else

	typedef int8_t				int8		;
	typedef uint8_t				uint8		;
	typedef int16_t				int16		;
	typedef uint16_t			uint16		;
	typedef int32_t				int32		;
	typedef uint32_t			uint32      ;
	typedef int64_t				int64	    ;
	typedef uint64_t			uint64      ;
#endif 	
}

#endif /** __TTINT_H */

