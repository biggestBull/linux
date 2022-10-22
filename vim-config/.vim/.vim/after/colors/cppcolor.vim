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

if version > 580
    " no guarantees for version 5.8 and below, but this makes it stop
    " complaining
    hi clear
    if exists("syntax_on")
	syntax reset
    endif
endif

let g:colors_name="cppcolor"

" 设置背景色
hi Normal ctermfg=231 ctermbg=236 cterm=NONE

" 注释
hi Comment ctermfg=61

hi normal ctermfg=lightgray

" goto break return continue 等转变流程的关键字, 以及asm
hi cStatement ctermfg=118
" if else switch 
hi cConditional ctermfg=201
" 循环 关键字
hi cRepeat ctermfg=198

" 格式化控制符：%d  %o %x %u 这种
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

hi cIncluded ctermfg=208

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

" 我写的: start
" 函数调用,放弃
"hi callFunction	ctermfg=2
" 类名
hi className	ctermfg=1

" 赋值
hi FZOperator ctermfg=5

" 算术运算符:加减乘除位运算  
hi SSOperator ctermfg=1
" 逻辑运算符  
hi LJOperator ctermfg=44
hi GXOperator ctermfg=3

" 括号
hi DKHOperator ctermfg=201
hi ZKHOperator ctermfg=2
hi XKHOperator ctermfg=3

" 我写的：end

"vim: sw=4
