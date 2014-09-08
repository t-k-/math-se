#include <stdio.h>
#include <stdlib.h>
#include "mathtree.h"
#include "edit.h"

extern struct token_t *root;
char *to_scan_str(char*, size_t*);

void parser_out(FILE *fout) 
{
	if (root) {
		matree_print(root, stdout); 
		printf("\n");
		matree_print_brword(root, stdout);

		matree_print(root, fout); 
		fprintf(fout, "\n");
		matree_print_brword(root, fout);
		
		matree_release(root); 
		fprintf(fout, "\n");
	}
}

int main(int argc, char *argv[]) 
{
	char *str, *str0;
	size_t str0_sz;
	char *url;
	FILE *fout;

	if (argc != 2) {
		printf("invalid argument format. \n");
		return 1;
	}

	url = argv[1];
	fout = fopen("parser.output", "w");
	fprintf(fout, "%s\n", url);

	/* disable auto-complete */
	rl_bind_key('\t', rl_abort);

	while (1) {
		str = readline("edit: ");
		if (str == NULL) {
			printf("\n");
			break;
		} 

		/* user can use UP and DOWN key to 
		search through the history. */
		add_history(str); 

		str0 = to_scan_str(str, &str0_sz);

		YY_BUFFER_STATE buffer = 
			yy_scan_buffer(str0, str0_sz);

		yyparse();

		if (parser_error_flag) {
			parser_error_flag = 0;
			printf("[ %s ]\n", parser_error_dscr);
		} else {
			fprintf(fout, "%s\n", str);
			parser_out(fout);
		}

		yy_delete_buffer(buffer);
		free(str);
		free(str0);
	} 

	fclose(fout);
	printf("[ goodbye~ ]\n");

	return 0;
}
