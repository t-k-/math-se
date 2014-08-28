#ifndef NULL
#define NULL ((void*)0)
#endif

/* macro BOOL allow a program to undefine and redefine it */
#ifndef BOOL
#define BOOL uint
#endif

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

#define OUT_FBODY "body.output"
#define OUT_FHEAD "head.output"

#define STR_BUFLEN 2048

#define _MY_DEBUG

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

#define COLOR_RST     "\e[0m"
#define COLOR_RED     "\033[1m\033[31m"      /* Bold Red */
#define COLOR_GREEN   "\033[1m\033[32m"      /* Bold Green */
#define COLOR_BLUE    "\033[1m\033[34m"      /* Bold Blue */
#define COLOR_MAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define COLOR_CYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define COLOR_GRAY    "\033[1m\033[30m"      /* Bold Gray */
