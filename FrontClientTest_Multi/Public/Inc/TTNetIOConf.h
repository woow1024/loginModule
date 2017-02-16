#ifndef __TT_NETIO_CONF_H
#define __TT_NETIO_CONF_H 1


#ifdef TTNETIO_EXPORTS
	#define TTNETIO_API  __declspec(dllexport)
#else
	#if defined(LINUX) || defined(MINGW) || defined(_LIB) || defined(TTNETIO_USE_STATIC)
	#define TTNETIO_API
	#else
	#define TTNETIO_API  __declspec(dllimport)
	#endif
#endif

#endif  /** __TT_NETIO_CONF_H */


