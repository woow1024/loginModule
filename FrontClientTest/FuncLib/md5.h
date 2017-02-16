#pragma once

// #ifdef __cplusplus

 extern "C"
 {
// #endif

namespace yt
{
	typedef struct
	{
		unsigned int total[2];
		unsigned int state[4];
		unsigned char buffer[64];
	}
	md5_context;

	void md5_starts( md5_context *ctx );
	void md5_update( md5_context *ctx, unsigned char *input, unsigned int length );
	void md5_finish( md5_context *ctx, unsigned char digest[16] );
	char *md5_string(const unsigned char *in, char *out, unsigned int in_len);
	char *md5_string2(const unsigned char *in, char *out, unsigned int in_len);
}
// #ifdef __cplusplus
 }
// #endif
