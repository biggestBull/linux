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

hi Comment ctermfg=darkgrey

"

hi cStatement ctermfg=1
hi cConditional ctermfg=6
hi cRepeat ctermfg=4

"格式化控制符
hi cSpecial ctermfg=1   
hi cFormat ctermfg=1   

" 预处理
hi cPreProc ctermfg=1
hi cDefine ctermfg=2
hi PreCondit ctermfg=1

hi cStructure ctermfg=1

hi cTypedef ctermfg=2
hi cType ctermfg=14

hi cConstant ctermfg=2

hi cStorageClass ctermfg=55

" c       -----end

" 写在注释中的标记
hi cTodo ctermfg=1

hi Label ctermfg=5
hi String ctermfg=brown
hi Character ctermfg=darkyellow
hi Number ctermfg=blue
hi cppFloat ctermfg=darkred cterm=bold

hi cppCast ctermfg=104

hi Include ctermfg=darkgrey

hi cppStatement ctermfg=16	
hi cppModifier	ctermfg=12
hi cppType		ctermfg=1

hi cppBasictype		ctermfg=14

hi cppTypedecorateconst ctermfg=2
hi cppTypedecorateregister ctermfg=darkyellow
hi cppTypedecoratevolatile ctermfg=darkred

hi cppExceptions ctermfg=1
hi cppOperator	ctermfg=71

hi cppBooleantrue ctermfg=2
hi cppBooleanfalse ctermfg=1

hi cppUsing ctermfg=1

" form vim90  ---end 

hi cppStructure ctermfg=green cterm=bold
hi Function ctermfg=162

hi cppAccess ctermfg=3
hi cCustomClass ctermfg=2
hi cppStorageClass ctermfg=25
hi cppStorageClassmutable ctermfg=1

hi cppSTLbool ctermfg=3
hi cppSTLconstant ctermfg=131
hi cppSTLexception ctermfg=1
hi cppSTLfunction ctermfg=222
hi cppSTLfunctional ctermfg=112
hi cppSTLios ctermfg=156
hi cppSTLiterator ctermfg=79
hi cppSTLiterator_tag ctermfg=69
hi cppSTLnamespace ctermfg=122
hi cppSTLtype ctermfg=209
hi cppSTLcast ctermfg=119

"vim: sw=4
