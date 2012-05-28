silent! syn clear CTagsFunction CTagsEnumeratorName CTagsDefinedName CTagsEnumerationValue CTagsClass CTagsNamespace CTagsMember CTagsConstant CTagsGlobalVariable CTagsType CTagsUnion CTagsStructure CTagsFunction CTagsExtern
syntax keyword CTagsFunction cmpintgp cmpstringp main output pop_sort quick_sort sort_inter sort_string


if exists('b:hlrainbow') && !exists('g:nohlrainbow')
	syn cluster cBracketGroup add=CTagsFunction 
	syn cluster cCppBracketGroup add=CTagsFunction 
	syn cluster cCurlyGroup add=CTagsFunction 
	syn cluster cParenGroup add=CTagsFunction 
	syn cluster cCppParenGroup add=CTagsFunction 
endif
