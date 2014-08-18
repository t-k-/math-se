#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
#include "mathtree.h"

#define BRW_HASH_LEN 40
#define DOC_HASH_LEN 40

//char doc_id[DOC_HASH_LEN];
//     |
//     |
//    _|_
//    \ /
//     * 
struct doc_frml {
	char brw_id[BRW_HASH_LEN];
	struct list_it sons;
};

struct doc_var {
	struct doc_frml *doc_fthr;
	char vname[VAR_NAME_MAX_LEN];
	struct list_it sons;
};

struct doc_brw {
	struct doc_var *var_fthr;
	uint *weight;
	float score;
};

#define DEFAULT_REDIS_PORT 6379

int redis_cli_init(const char*, ushort);

void redis_cli_free();

void redis_set_add_hash(const char*, const char*);

void redis_set_union(const char*, const char*);

int redis_set_printall(const char*);

void redis_set_popeach(const char*);

struct doc_frml *redis_frml_map_get(const char*);

int redis_frml_map_set(const char*, struct doc_frml*);

void redis_frml_map_del(const char*);
