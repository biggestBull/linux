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
let g:colors_name="mygocolor"

hi goStatement	ctermfg=5 cterm=bold 
hi goDirective ctermfg=1 
hi Comment ctermfg=4
hi Keyword  ctermfg=6
hi Conditional ctermfg=5
hi Repeat ctermfg=5
hi Function ctermfg=2 

hi String ctermfg=darkgrey
hi Character ctermfg=darkgrey
hi Number ctermfg=darkgrey
hi Special ctermfg=darkgrey

hi Operator ctermfg=1 cterm=bold
hi Type	ctermfg=4 cterm=bold
hi goMethod	ctermfg=6 cterm=bold

hi diyType	ctermfg=2 cterm=bold 

hi Error	cterm=bold ctermfg=7 ctermbg=1
hi goLable ctermfg=3

"vim: sw=4
