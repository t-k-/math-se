struct yy_buffer_state;  

typedef struct yy_buffer_state
*YY_BUFFER_STATE;
 
typedef size_t yy_size_t;
 
YY_BUFFER_STATE
yy_scan_buffer(char *base, yy_size_t size);

void
yy_delete_buffer(YY_BUFFER_STATE buffer);

#include "readline/readline.h"
#include "readline/history.h"
