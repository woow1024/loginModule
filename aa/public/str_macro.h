#ifndef _CHAR_HELPER_MACRO_H
#define _CHAR_HELPER_MACRO_H

#define RELEASE_PARAM(in)	\
	free(in);	\
	in = NULL;

#define EMPTY_STRING					 ""
#define EMPTY_CHAR						 ''
#define DRIVE_IDENTIFICATION			 ':'					
#define PATH_SPLIT_WINDOW_CHAR			 '\\'					
#define PATH_SPLIT_UNIX_CHAR			 '/'


#endif	// _CHAR_HELPER_MACRO_H