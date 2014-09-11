#include "tknet.h"

/* some static structs */
struct traversal_arg {
	tree_it_fun *each_do;
	void        *extra;
};

struct foreach_arg {
	tree_depth_t         depth;
	struct traversal_arg *t_arg;
};

/* traversal methods implements */
LIST_IT_CALLBK(tree_post_order_DFS)
{
	LIST_OBJ(struct tree_node, node, ln);
	P_CAST(f_arg, struct foreach_arg, pa_extra);
	struct foreach_arg lo_f_arg = {f_arg->depth + 1, f_arg->t_arg};

	list_foreach(&node->sons, &tree_post_order_DFS, &lo_f_arg);

	return (*f_arg->t_arg->each_do)(pa_head, pa_now, pa_fwd, 
				lo_f_arg.depth, f_arg->t_arg->extra);
}

LIST_IT_CALLBK(tree_pre_order_DFS)
{
	LIST_OBJ(struct tree_node, node, ln);
	P_CAST(f_arg, struct foreach_arg, pa_extra);
	struct foreach_arg lo_f_arg = {f_arg->depth + 1, f_arg->t_arg};
	BOOL res;

	res = (*f_arg->t_arg->each_do)(pa_head, pa_now, pa_fwd, 
				lo_f_arg.depth, f_arg->t_arg->extra);

	list_foreach(&node->sons, &tree_pre_order_DFS, &lo_f_arg);

	return res;
}

/* go through a tree, see tree.h for detail. */
void
tree_foreach(struct tree_node *root, list_it_fun *traversal, 
		tree_it_fun *each_do, BOOL if_exclude_root, void *extra)
{
	struct list_it tmp = list_get_it(&root->ln);
	struct traversal_arg t_arg = {each_do, extra};
	struct foreach_arg   f_arg = {0, &t_arg};

	if (if_exclude_root) {
		f_arg.depth ++;
		list_foreach(&root->sons, traversal, &f_arg);
	} else
		list_foreach(&tmp, traversal, &f_arg);
}
