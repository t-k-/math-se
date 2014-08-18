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

void redis_set_popeach(const char *set)
{
	redisReply *r;
	while (1) {
		r = redisCommand(redis_cntxt, "spop %s", set);
		if (r->type == REDIS_REPLY_NIL) {
			break;
		}

		printf("string: %s\n", r->str);
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

void redis_frml_map_del(const char *hash)
{
	struct doc_frml *ret;
	redisReply *r;

	r = redisCommand(redis_cntxt, "del %s", hash);
	freeReplyObject(r);
}
