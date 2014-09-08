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

struct list_it tex2brwords(char *str, 
                           struct token_t **tr_root)
{
	struct list_it ret = LIST_NULL;
	char *str0;
	size_t str0_sz;
	str0 = to_scan_str(str, &str0_sz);

	YY_BUFFER_STATE buffer = 
		yy_scan_buffer(str0, str0_sz);

	yyparse();

	if (parser_error_flag) {
		parser_error_flag = 0;
		*tr_root = NULL;
		goto exit;
	}

	ret = matree_ls_brw(root);
	*tr_root = root;

exit:
	yy_delete_buffer(buffer);
	free(str0);

	return ret;
}
