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
