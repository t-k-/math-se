def score_tr_mark_cross(li_query_brw, score_tr):
	res = 0
	for q_brw in li_query_brw:
		for pointer in q_brw.gutter:
			if pointer->state == unmark:
				score = score_cache(q_brw.id, pointer->brw_id)
				if score > pointer->father.max[q_brw]:
					pointer->father.max[q_brw] = score
					pointer->father.who[q_brw] = pointer
					pointer->state = mark
		stage_flag = 0
		if last_iteration():
			stage_flag = 1
		else if next_vname() != brw.vname:
			stage_flag = 1

		if stage_flag:
			max_var_score = 0
			max_var = None
			for var in score_tr.root:
				var_score = sum(var[i].score)
				if var_score > max_var_score:
					max_var_score = var_score
					max_var = var
			score_tr.root.score += max_var_score
			for var in score_tr.root:
				test = (var == max_var)
				next_state = cross if test else unmark
				for who in var[i].who:
					who.state = next_state
				clear(var)
	res = score_tr.root.score
	clear(score_tr)
	return res

def merge_search(li_search_path, li_query_brw):
	cnt = 0
	given matched doc[i]:
		score_tr = score_tr_clean()
		given matched doc_brw[i] in search_path[i]:
			brw_node = score_tr_insert(score_tr, doc_brw, li_query_brw.len)
			score_cache(doc_brw, search_path[i].brw)
			for src_brw in search_path[i].source_brw:
				src_brw.gutter.add_pointer(brw_node) 
		score = score_tr_mark_cross(li_query_brw, score_tr)
		if doc[i] in ranking:
			modify_ranking(score, doc[i])
		else:
			add_to_ranking(score, doc[i])
		cnt += 1
	return cnt

def main():
	li_merge_point = merge_points(tr_query) # in depth order
	cnt = 0
	for mp in li_merge_point:
		li_query_brw = query_brw(mp)
		li_search_path = search_path(li_query_brw) # unique path
		# each search path may contain multiple source query brw(s)
		li_search_path = sort_by_vname(li_search_path)
		cnt += merge_search(li_search_path, li_query_brw)
		if cnt >= threshold:
			break
