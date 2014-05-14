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

static __inline int max(int a, int b)
{
	return (a>b)?a:b;
}

static __inline int min(int a, int b)
{
	return (a<b)?a:b;
}
