#include <stdio.h>
#include <stdlib.h>
#include "mathtree.h"
#include "edit.h"

extern struct token_t *root;

char *to_scan_str(char *str, size_t *res_str_sz)
{
	char *str0;
	size_t str0_sz;
	str0_sz = strlen(str) + 3;
	str0 = malloc(str0_sz);
	sprintf(str0, "%s\n_", str);
	str0[str0_sz - 2] = '\0';

	*res_str_sz = str0_sz;
	return str0;
}

struct list_it tex2brwords(char *str) 
{
	struct list_it ret;
	char *str0;
	size_t str0_sz;
	str0 = to_scan_str(str, &str0_sz);

	YY_BUFFER_STATE buffer = 
		yy_scan_buffer(str0, str0_sz);

	yyparse();
	ret = matree_ls_brw(root);
	matree_release(root); 

	yy_delete_buffer(buffer);
	free(str0);

	return ret;
}
