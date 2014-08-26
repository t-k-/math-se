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
	struct list_it sons;
	struct list_node ln;
	char vname[VAR_NAME_MAX_LEN];
	float score;
};

enum brw_state {
	bs_unmark,
	bs_mark,
	bs_cross
};

struct doc_brw {
	struct list_node ln;
	uint *weight;
	float score;
	char id[BRW_HASH_LEN];
	enum brw_state state;
};

#define DEFAULT_REDIS_PORT 6379

typedef void (*retstr_callbk)(const char *, void *);

int redis_cli_init(const char*, ushort);

void redis_cli_free();

void redis_shutdown();

void redis_set_add_hash(const char*, const char*);

void redis_set_union(const char*, const char*);

int redis_set_printall(const char*);

void redis_set_popeach(const char*, retstr_callbk);

void redis_set_popeach_ext(const char*, retstr_callbk, void*);

void redis_set_members(const char*, retstr_callbk);

struct doc_frml *redis_frml_map_get(const char*);

int redis_frml_map_set(const char*, struct doc_frml*);

void redis_frml_map_del(const char*);

struct doc_brw *rlv_tr_test(struct doc_frml*, char*, char*, 
                            struct doc_var **);

struct doc_brw *rlv_tr_insert(struct doc_frml*, char*, char*, 
                              uint*);

struct doc_brw *rlv_tr_qk_insert(struct doc_var*, 
                                 struct doc_frml*, char*, 
                                 char*, uint*, char **);

void rlv_tr_print(struct doc_frml*);

void rlv_tr_free(struct doc_frml*);

struct doc_brw *rlv_process_str(const char*, char**, const char*);

void print_weight(uint*);

char *hash2str(char*);
