# parser:
# input: url, tex list
# output: parser.output
#	url, (tex, tree, path[], brw[])[]

# trpa *cowpie_parse(tex)

# {path[], brw[]} trpa_gen_path_brw(trpa *)

# brw: {n, vname, nsons[]}

# index:
# input: parser.output
# output: 
#	./col/bdb {#id}->{#brw, url, tex, tree}
#	./col/path/to/brw/posting {max, min, records[]}

# brw_record: {frml_id, brw_idx, brw}

# score tree: 
#	root: { score, n_var, id}
#	var[]: { n_leaf, vname, {max, who}[n_query_brw] }
#	leaf: { father, state }

# query_brw: { dir, brw, gutter }
# search_path: { dir, src[] }

# search:
def score_tr_mark_cross(li_query_brw, score_tr):
	i = 0, j = 0
	for q_brw in li_query_brw:
		for pointer in q_brw.gutter:
			if pointer->state == unmark:
				score = score_cache(q_brw.id, pointer->brw_id)
				if score > pointer->father.max[i]:
					if pointer->father.who[i]:
						pointer->father.who[i]->state = unmark
					pointer->father.max[i] = score
					pointer->father.who[i] = pointer
					pointer->state = mark

		for var in score_tr.root:
			var.score += var.max[i]
			if var.score > var.father.max:
				var.father.max = var.score
				var.father.who = var

		i += 1

		stage_flag = 0
		if last_iteration():
			stage_flag = 1
		else if next_vname() != q_brw.vname:
			stage_flag = 1

		if stage_flag:
			for var in score_tr.root:
				test = (var == score_tr.root.who)
				next_state = cross if test else unmark
				for who in var.who[j:i]:
					if who != None:
						who.state = next_state
			var[].clear(score, max, who)
			score_tr.root.score += score_tr.root.max 
			score_tr.root.max = 0 
			score_tr.root.who = None
			j = i
	state[].clear()
	return clear(score_tr.root.score)

def merge_search(li_search_path, li_query_brw, score_tr, cnt):
	given matched doc_brw in li_search_path[i]:
	@ first_call:
		if doc_brw.id in ranking_set:
			return SKIP_THIS_ID 
		else:
			score_tr_clean(score_tr)
	@ every_call but not SKIP_THIS_ID:
		for query_brw in li_search_path[i].src:
			n_query_brw = len(li_query_brw)
			score_leaf = score_tr_insert(score_tr, doc_brw, 
			                             n_query_brw)
			query_brw.gutter.add_pointer(score_leaf) 
			score_cache(doc_brw.brw, query_brw.brw)
	@ end_call but not SKIP_THIS_ID:
		score_tr_mark_cross(li_query_brw, score_tr)
		add_to_ranking(score_tr.root.score, doc.id)
		cnt += 1
		if cnt >= threshold:
			return ABORT_ALL	

def main():
	li_merge_point = trpa_merge_points(tr_query) # in depth order
	cnt = 0
	score_tr = new_score_tr() 
	for mp in li_merge_point:
		li_query_brw = trpa_gen_path_brw(mp)
		li_search_path = search_path(li_query_brw) # unique path
		# each search path may contain multiple source query brw(s)
		li_query_brw = sort_by_vname(li_query_brw)
		if not merge_search(li_search_path, li_query_brw, 
		                    score_tr, &cnt)
			break
	score_tr_destroy(score_tr)
