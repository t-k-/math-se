def score(li_query_brw):
	sort_by_name(li_query_brw)
	set_frml and tr_root = empty
	for α in li_query_brw:
		li_doc_brw, li_doc_frml = search_match(α)
		set_frml += li_doc_frml
		flag = 0
		for β in li_doc_brw:
			if tr_insert(tr_root, β):
				β.state = unmark
				β.score = 0
			if β.state == unmark:
				flag = 1
				β.score = score(α, β)
		if not flag:
			break
		for frml in li_doc_frml:
			for var in frml:
				max_score = 0
				max_child = None
				for β in var:
					if β.state == unmark:
						if max_score < β.score
							max_child = β
							max_score = β.score
						β.score = 0
				if max_score > 0:
					var.score += max_score
					max_child.state = mark 
		if not α.next or α.name != a.next.name:
			for frml in set_frml:
				max_var = max_score_child(frml)
				if max_var:
					frml.score += max_var.score 
				for var in frml:
					test = (var == max_var)
					next_state = cross if test else unmark
						for β in var:
							if β.state is mark:
								β.state = next_state
					var.score = 0
			set_frml = empty
	# return the formula scores in a list.
	# with #doc_brw info as subscore.
	return rlv_tr_score(tr_root) 
}
