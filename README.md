finish :
- fix flickering bg bug ( set bg color to current of what i try to force it to then just add your own outline instead of using hacky subpixel glow )
- fix not being able to full screen drag on vert monitor ( wnd_drag.cpp )
- organize/optimize menu functionality code, & check for memory leaks, minimize var scope & const unchanging vars, clean/optimize files ( file checklist )
- - match any "new" statements with "delete", make sure hooks are managed with unhooks on exit, etc
- plan functionality for :
- - Tools
- - - Find
- - - Replace
- - - Macro
- - - multi-Cursor
- - - Tools -> Plugins ( Custom scripting language )
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ ? )
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode
- test current and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, then make repo public