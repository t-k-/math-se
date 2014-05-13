#define OUT_FBODY "body.output"
#define OUT_FHEAD "head.output"

#define STR_BUFLEN 2048

//#define _MY_DEBUG

#ifdef _MY_DEBUG
#define OUTPUT(...) \
	printf(__VA_ARGS__)
#else
#define OUTPUT(...)
#endif
