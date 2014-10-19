#include <stdint.h>

void *bdb_init(const char*); 

typedef int (*TCCMP)(const char *aptr, int asiz, const char *bptr, int bsiz, void *op);

void *bdb_init2(const char *, TCCMP); 

uint64_t bdb_records(void*);

int bdb_release(void*); 

void c_free(void*); 

int bdb_put2(void*, const char*, const char*);

char *bdb_get2(void*, const char*);

int bdb_put_int(void*, const char*, int, int);

int *bdb_get_int(void*, const char*, int);
