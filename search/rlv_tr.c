#include "rlv_tr.h"

static redisContext *redis_cntxt;

int redis_cli_init(const char *ip, ushort port)
{
	struct timeval timeout = {1, 500000}; /* 1.5 seconds */
	redis_cntxt = redisConnectWithTimeout(ip, port, timeout);
	if (redis_cntxt == NULL || redis_cntxt->err) {
		if (redis_cntxt) {
			printf("redis error: %s\n", redis_cntxt->errstr);
			redisFree(redis_cntxt);
		} else {
			printf("can't allocate redis context.\n");
		}
		return 1;
	}

	return 0;
}

void redis_cli_free()
{
	redisFree(redis_cntxt);
}

void redis_set_add_hash(const char *set, const char *hash)
{
	redisReply *r;
	r = redisCommand(redis_cntxt, "sadd %s %s", set, hash);
	freeReplyObject(r);
}

void redis_set_union(const char *set0, const char *set1)
{
	redisReply *r;
	r = redisCommand(redis_cntxt, "sunionstore %s %s %s", 
	                 set0, set0, set1);
	freeReplyObject(r);
}

int redis_set_printall(const char *set)
{
	size_t i;
	redisReply *r;
	r = redisCommand(redis_cntxt, "smembers %s", set);
	if (r->type == REDIS_REPLY_NIL) {
		freeReplyObject(r);
		return 0;
	}

	for (i = 0; i < r->elements; i++) {
		printf("(%zu) %s\n", i, r->element[i]->str);
	}
	
	freeReplyObject(r);
	return 1;
}

void redis_set_popeach(const char *set, retstr_callbk fun)
{
	redisReply *r;
	while (1) {
		r = redisCommand(redis_cntxt, "spop %s", set);
		if (r->type == REDIS_REPLY_NIL) {
			break;
		}

		fun(r->str);
		freeReplyObject(r);
	}

	freeReplyObject(r);
}

int redis_frml_map_set(const char *hash, struct doc_frml *ptr)
{
	char addr_str[32];
	redisReply *r;
	int ret_type;

	sprintf(addr_str, "%p", ptr);
	r = redisCommand(redis_cntxt, "set %s %s", hash, addr_str);
	ret_type = r->type;
	freeReplyObject(r);

	return !(r->type == REDIS_REPLY_STATUS);
}

struct doc_frml *redis_frml_map_get(const char *hash)
{
	struct doc_frml *ret;
	redisReply *r;

	r = redisCommand(redis_cntxt, "get %s", hash);

	if (r->type == REDIS_REPLY_STRING) {
		sscanf(r->str, "%p", &ret);
		freeReplyObject(r);
		return ret;
	} 

	freeReplyObject(r);
	return NULL;
}

void redis_del(const char *hash)
{
	struct doc_frml *ret;
	redisReply *r;

	r = redisCommand(redis_cntxt, "del %s", hash);
	freeReplyObject(r);
}

struct doc_var *rlv_tr_test(struct doc_frml *df, char *id)
{
	return NULL;
}

struct _var_find_arg {
	char *vname;
	struct doc_var *var;
};

static 
LIST_IT_CALLBK(_var_find)
{
	LIST_OBJ(struct doc_var, v, ln);
	P_CAST(vfa, struct _var_find_arg, pa_extra);

	if (0 == strcmp(v->vname, vfa->vname))
		vfa->var = v;
	
	LIST_GO_OVER;
}

uint brwsize(uint *weight)
{
	uint i, cnt = 0;
	for (i = 0; weight[i]; i++) {
		cnt ++;
	}
	return cnt;
}

void brwcpy(uint *w_dst, uint *w_src)
{
	uint i;
	for (i = 0; w_src[i]; i++) {
		printf("cpy [%d]\n", i);
		w_dst[i] = w_src[i];
	}
	w_dst[i] = 0;
}

void rlv_tr_insert(struct doc_frml *df, char *brw_id, 
                   char *vname, uint *weight)
{
	struct doc_var *var;
	struct doc_brw *new_brw;
	struct _var_find_arg vfa = {vname, NULL};

	list_foreach(&df->sons, _var_find, &vfa);

	if (vfa.var) {
		var = vfa.var;
	} else {
		var = malloc(sizeof(struct doc_var));
		var->doc_fthr = df;
		LIST_CONS(var->sons);
		LIST_NODE_CONS(var->ln);
		strcpy(var->vname, vname);
		var->score = 0.f;
		
		list_insert_one_at_tail(&var->ln, &df->sons, NULL, NULL);
	}

	new_brw = malloc(sizeof(struct doc_brw));
	new_brw->var_fthr = var;
	LIST_NODE_CONS(new_brw->ln);
	strcpy(new_brw->id, brw_id);
	new_brw = malloc(sizeof(uint) * brwsize(weight));
	brwcpy(new_brw->weight, weight);
	new_brw->score = 0.f;

	list_insert_one_at_tail(&new_brw->ln, &var->sons, NULL, NULL);
}

static LIST_IT_CALLBK(_print_brw)
{
	uint i;
	LIST_OBJ(struct doc_brw, brw, ln);
	printf("\tbrw #%s [%f] ", brw->id, brw->score);
	for (i = 0; brw->weight[i]; i++) {
		printf("%d-", brw->weight[i]);
	}
	printf("\n");
	
	LIST_GO_OVER;
}

static LIST_IT_CALLBK(_print_frml)
{
	LIST_OBJ(struct doc_var, v, ln);
	printf("\tvar #%s [%f]\n", v->vname, v->score);
	list_foreach(&v->sons, _print_brw, NULL);
	
	LIST_GO_OVER;
}

void rlv_tr_print(struct doc_frml *df)
{
	printf("formula #%s [%f]\n", df->id, df->score);
	list_foreach(&df->sons, _print_frml, NULL);
	return;
}

void process_str(const char *str, const char *ret_set)
{
	char brw_id[DOC_HASH_LEN];
	char vname[VAR_NAME_MAX_LEN];
	uint weight[WEIGHT_MAX_LEN];
	struct doc_frml *df;
	struct doc_var *test;
	uint i = 0;
	
	char *field = strtok((char*)str, " ");

	while (field) {
		switch (i) {
			case 0:
				strcpy(brw_id, field);
				break;
			case 1:
				df = redis_frml_map_get(field);
				if (!df) {
					df = malloc(sizeof(struct doc_frml));
					LIST_CONS(df->sons);
					strcpy(df->id, field);
					df->score = 0.f;

					redis_set_add_hash(ret_set, field);
					redis_frml_map_set(field, df);
					test = NULL;
				} else {
					test = rlv_tr_test(df, brw_id);
				}

				break;
			case 2:
				if (NULL == test)
					strcpy(vname, field);
				break;
			default:
				if (NULL == test)
					weight[i - 3] = atoi(field);
		}

		printf("field: %s\n", field);
		field = strtok(NULL, " ");
		i ++;
	}

	if (test == NULL) {
		rlv_tr_insert(df, brw_id, vname, weight);
		weight[i - 3] = 0;
	}
}
