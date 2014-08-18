#include <unistd.h>
#include <dirent.h>
#include "rlv_tr.h"

int main(void) 
{
	/* init */
	struct doc_frml test_frml, *p;
	if (redis_cli_init("127.0.0.1", DEFAULT_REDIS_PORT))
		return 1;

	/* test for redis_set_* */
	redis_set_add_hash("hash-set", "ASJDOF");
	redis_set_add_hash("hash-set", "POIDJS");
	redis_set_add_hash("hash-set", "LIOWQQ");

	redis_set_add_hash("animal-set", "dog");
	redis_set_add_hash("animal-set", "cat");

	printf("hash-set: \n");
	redis_set_printall("hash-set");

	printf("animal-set: \n");
	redis_set_printall("animal-set");

	redis_set_union("hash-set", "animal-set");
	printf("hash-set after union: \n");
	redis_set_printall("hash-set");

	redis_set_popeach("hash-set");

	printf("hash-set after pop-out: \n");
	if (1 == redis_set_printall("hash-set"))
		printf("empty set.\n");
	
	/* test for redis_frml_* */
	p = redis_frml_map_get("some_hash");
	printf("get null: %p\n", p);

	strcpy(test_frml.brw_id, "this is test.");
	redis_frml_map_set("some_hash", &test_frml);

	p = redis_frml_map_get("some_hash");
	printf("get pointer: %p (\"%s\")\n", p, p->brw_id);

	redis_frml_map_del("some_hash");

	/* destory */
	redis_cli_free();
}
