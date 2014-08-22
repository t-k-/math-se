#include <unistd.h>
#include <dirent.h>
#include "rlv_tr.h"

void print_fun(const char *value)
{
	printf("value: %s\n", value);
}

void free_map_fun(const char *map)
{
	struct doc_frml *df = redis_frml_map_get(map);
	printf("free formula ...\n");
	rlv_tr_print(df);
	free(df);
	redis_del(map);
}

#define PROCESS(_line) \
	line = strdup(_line); \
	rlv_process_str(line, "tmp"); \
	free(line)

int main(void) 
{
	struct doc_frml test_frml, *p;
	struct doc_var *into_var;
	int res;
	uint weight[WEIGHT_MAX_LEN];

	/* init redis client */
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

	redis_set_popeach("hash-set", &print_fun);

	printf("hash-set after pop-out: \n");
	if (1 == redis_set_printall("hash-set"))
		printf("empty set.\n");
	
	/* test for redis_frml_* */
	p = redis_frml_map_get("some_hash");
	printf("get null: %p\n", p);

	test_frml.score = 3.14f;
	redis_frml_map_set("some_hash", &test_frml);

	p = redis_frml_map_get("some_hash");
	printf("get pointer: %p(= %f)\n", p, p->score);

	redis_del("some_hash");

	/* test for rlv_tr_insert */
	printf("sizeof(struct doc_frml)=%ld\n", 
	        sizeof(struct doc_frml));

	printf("sizeof(struct doc_var)=%ld\n", 
	        sizeof(struct doc_var));

	printf("sizeof(struct doc_brw)=%ld\n", 
	        sizeof(struct doc_brw));

	p = malloc(sizeof(struct doc_frml));
	LIST_CONS(p->sons);
	strcpy(p->id, "DOREME");
	p->score = 0.1f;

	weight[0] = 1;
	weight[1] = 2;
	weight[2] = 3;
	weight[3] = 0;
	rlv_tr_insert(p, "alpha", "ABCD", weight);
	weight[0] = 4;
	weight[1] = 5;
	weight[2] = 6;
	weight[3] = 0;
	rlv_tr_insert(p, "beta", "FIOJ", weight);
	weight[0] = 4;
	weight[1] = 5;
	weight[2] = 6;
	weight[3] = 0;
	rlv_tr_insert(p, "alpha", "OKMM", weight);

	rlv_tr_print(p);

	char vname[] = "alpha", vhash[] = "bbb";
	res = rlv_tr_test(p, vname, vhash, &into_var);
	printf("res = %d, into_var=%s\n", res, (into_var==NULL)?"no":into_var->vname);
	if (res) 
		rlv_tr_qk_insert(into_var, p, vname, vhash, weight);
	rlv_tr_print(p);
	printf("free relevance tree...\n");
	rlv_tr_free(p);

	/* test for rlv_process_str */
	char *line;
	PROCESS("04a8bf5d416a9d2eae327d3d239a256b28b6c3de "
    "42e92bfc15a38e15a797c46a16e1f2d6fc5eada6 _xi 1 18 20 47 55");
	PROCESS("04a8bf5d416a9d2eae327d3d239a256b28b6c3de "
    "42e92bfc15a38e15a797c46a16e1f2bbbbbbbbbb _xi 1 18 20 47 55");
	PROCESS("04a8bf5d416a9d2eae327d3d23uuuuuuuuuuuuuu "
    "42e92bfc15a38e15a797c46a16e1f2bbbbbbbbbb _pi 2 16 18 40 50");
	PROCESS("04a8bf5d416a9d2eae327d3d23uuuuuuuuuuuuuu "
    "42e92bfc15a38e15a797c46a16e1f2bbbbbbbbbb _pi 2 16 18 40 50");
	PROCESS("04a8bf5d416a9d2eae327d3d239rrrrrrrrrrrrr "
    "42e92bfc15a38e15a797c46a16e1f2d6fc5eada6 _xi 3 11 21 37 45");

	redis_set_popeach("tmp", &free_map_fun);
	redis_del("tmp");

	redis_cli_free();
}
