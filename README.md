todo :
- fix scroll bar jumping
- - write out math and needs, maybe use :
```
s32 char_idx = (s32)SendMessageW( parent, EM_LINEINDEX, curr_line - 1, 0 );
SendMessageW( parent, EM_SETSEL, char_idx, char_idx );
```
- fix min size
- - remove debug print info
- add update scrollbar pos when clicked in text box
- add scrolling using scroll wheel while hovered over text box
- simplify/optimize cscroll & edit control
- add line count ( use edit_getlinecount/edit_getfirstvisibleline )
- add cursor/caret position in bottom middle
- minimize variable scope, clean up, & optimize everything in all files
- - "const all vars unless they cant be"
- add icon
- add menu bar & functionality
- - file
- - macro
- - theme
