todo :
- finish functionality for :
- - Style
- - - add symbol indicating toggled status in wnd/funcs/menu/wnd_menu.cpp
- - - draw over toggled off components
- organize/optimize menu functionality code, & check for memory leaks, minimize var scope & const unchanging vars, clean/optimize files ( file checklist )
- - match any "new" statements with "delete"
- - make sure hooks are managed with unhooks on exit
- swap setwindowtexta calls to append text to beginning/end of text instead of overwriting
- fix flickering background bug (set background color to current of what i try to force it to then just add your own outline instead of using the subpixel glow)
- do functionality for :
- - Tools
- - - Find
- - - Replace
- - - Macro
- - - multi-Cursor
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ ? )
- fix not being able to full screen drag on vert monitor ( wnd_drag.cpp )
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode
- test current and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, then make repo public
- put in todo :
- - Tools -> Plugins ( Custom scripting language )
