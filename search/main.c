#include <stdio.h>
#include <stdlib.h>
#include "mathtree.h"

static
LIST_IT_CALLBK(test)
{
	int i;
	LIST_OBJ(struct query_brw, p, ln);
	printf("%s ", p->dir);
	printf("%s ", p->vname);
	for (i = 0;; i++) {
		if (p->weight[i] == 0)
			break;
		printf("%d ", p->weight[i]);
	}
	printf("\n");
	LIST_GO_OVER;
}

int main(int argc, char *argv[])
{
	struct list_it li_query_brw; 
	char *query = argv[1];
	if (argc != 2) {
		printf("invalid argument format.\n");
		return 1;
	}

	printf("%s\n", query);
	li_query_brw = tex2brwords(query);

	list_foreach(&li_query_brw, &test, NULL);
	li_brw_release(&li_query_brw);

	return 0;
}
