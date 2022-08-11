" Vim color file
" Maintainer:	Hans Fugal <hans@fugal.net>
" Last Change:	$Date: 2004/06/13 19:30:30 $
" Last Change:	$Date: 2004/06/13 19:30:30 $
" URL:		http://hans.fugal.net/vim/colors/desert.vim
" Version:	$Id: desert.vim,v 1.1 2004/06/13 19:30:30 vimboss Exp $

" cool help screens
" :he group-name
" :he highlight-groups
" :he cterm-colors

set background=dark
if version > 580
    " no guarantees for version 5.8 and below, but this makes it stop
    " complaining
    hi clear
    if exists("syntax_on")
	syntax reset
    endif
endif
let g:colors_name="cppcolor"

hi Comment ctermfg=61

"

hi cStatement ctermfg=118
hi cConditional ctermfg=201
hi cRepeat ctermfg=93

"格式化控制符
hi cSpecial ctermfg=6  
hi cFormat ctermfg=brown   

" 预处理
hi cPreProc ctermfg=86
hi cDefine ctermfg=159
hi PreCondit ctermfg=56

hi cStructure ctermfg=178

hi cTypedef ctermfg=159
hi cType ctermfg=105

hi cConstant ctermfg=94

hi cStorageClass ctermfg=76

" c       -----end

" 写在注释中的标记
hi cTodo ctermfg=1

hi Label ctermfg=135
hi String ctermfg=180
hi Character ctermfg=100
hi Number ctermfg=94
hi cppFloat ctermfg=96

hi cppCast ctermfg=209

hi Include ctermfg=8

hi cppStatement ctermfg=162	
hi cppModifier	ctermfg=13
hi cppType		ctermfg=2

hi cppBasictype		ctermfg=105

hi cppTypedecorateconst ctermfg=47
hi cppTypedecorateregister ctermfg=darkyellow
hi cppTypedecoratevolatile ctermfg=darkyellow

hi cppExceptions ctermfg=124
hi cppOperator	ctermfg=193

hi cppBooleantrue ctermfg=2
hi cppBooleanfalse ctermfg=196

hi cppUsing ctermfg=226

" form vim90  ---end 

hi cppStructure ctermfg=220
hi Function ctermfg=130

hi cppAccess ctermfg=122
hi cCustomClass ctermfg=2
hi cppStorageClass ctermfg=193
hi cppStorageClassmutable ctermfg=darkyellow

hi cppSTLbool ctermfg=100
hi cppSTLconstant ctermfg=100
hi cppSTLexception ctermfg=196
hi cppSTLfunction ctermfg=130
hi cppSTLfunctional ctermfg=214
hi cppSTLios ctermfg=157
hi cppSTLiterator ctermfg=84
hi cppSTLiterator_tag ctermfg=69
hi cppSTLnamespace ctermfg=40
hi cppSTLtype ctermfg=79
hi cppSTLcast ctermfg=209

"vim: sw=4
