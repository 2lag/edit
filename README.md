todo :
- finish functionality for :
- - Style
- - - add config creation & call in WinMain after debug console called
- - - - dont forget to init "bool menu_style_toggle[ OBJ_BASE_COUNT / 2 ] = { true };" then and there
- - - follow comments to finish wnd_menu_funcs' wnd_menu_style_toggle
- - - add notation for toggled status in menu
- swap setwindowtexta calls to append text to end of current text instead of overwriting their entire work loll
- do functionality for :
- - Tools
- - - Find
- - - Replace
- - - Macro
- - - multi-Cursor
- - - Plugins ( Change to Scripts ? )
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ ? )
- fix not being able to full screen drag on vert monitor ( wnd_drag.cpp )
- organize/optimize menu functionality code, & check for memory leaks, minimize var scope & const unchanging vars, clean/optimize files ( file checklist )
- - match any "new" statements with "delete"
- - make sure hooks are managed with unhooks on exit
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode
- test current and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, then make repo public