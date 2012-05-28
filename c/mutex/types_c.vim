silent! syn clear CTagsFunction CTagsEnumeratorName CTagsDefinedName CTagsEnumerationValue CTagsClass CTagsNamespace CTagsMember CTagsConstant CTagsGlobalVariable CTagsType CTagsUnion CTagsStructure CTagsFunction CTagsExtern
syntax keyword CTagsGlobalVariable g_cond g_mutex g_num
syntax keyword CTagsFunction main thread_add thread_sub


if exists('b:hlrainbow') && !exists('g:nohlrainbow')
	syn cluster cBracketGroup add=CTagsGlobalVariable,CTagsFunction 
	syn cluster cCppBracketGroup add=CTagsGlobalVariable,CTagsFunction 
	syn cluster cCurlyGroup add=CTagsGlobalVariable,CTagsFunction 
	syn cluster cParenGroup add=CTagsGlobalVariable,CTagsFunction 
	syn cluster cCppParenGroup add=CTagsGlobalVariable,CTagsFunction 
endif
