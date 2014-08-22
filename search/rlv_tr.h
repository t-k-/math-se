#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>
#include "mathtree.h"

#define BRW_HASH_LEN 40
#define DOC_HASH_LEN 40

struct doc_frml {
	struct list_it sons;
	char id[DOC_HASH_LEN];
	float score;
};

struct doc_var {
	struct doc_frml *doc_fthr;
	struct list_it sons;
	struct list_node ln;
	char vname[VAR_NAME_MAX_LEN];
	float score;
};

struct doc_brw {
	struct list_node ln;
	struct doc_var *var_fthr;
	uint *weight;
	float score;
	char id[BRW_HASH_LEN];
};

#define DEFAULT_REDIS_PORT 6379

typedef void (*retstr_callbk)(const char *);

int redis_cli_init(const char*, ushort);

void redis_cli_free();

void redis_set_add_hash(const char*, const char*);

void redis_set_union(const char*, const char*);

int redis_set_printall(const char*);

void redis_set_popeach(const char*, retstr_callbk);

struct doc_frml *redis_frml_map_get(const char*);

int redis_frml_map_set(const char*, struct doc_frml*);

void redis_frml_map_del(const char*);

struct doc_var *rlv_tr_test(struct doc_frml*, char*);

void rlv_tr_insert(struct doc_frml*, char*, char*, uint*);

void rlv_tr_print(struct doc_frml*);
