todo :
- finish functionality for :
- - File
- - - Open
- - - - swap with fread calls ( figure out how to detect chartype tho )
- - - - test with [these (english only ofc)](https://github.com/arc80/plywood/tree/9c606056faf89f0918b81f5af09c23fefaf9a12d/repos/plywood/src/apps/AutodetectTest/tests)
- check for memory leaks ( test everything )
- do functionality for :
- - Style ( make a raw byte cfg in same dir for settings ( & load on start ) )
- - - hide Fps
- - - hide caret Pos
- - - hide Line count
- - Tools
- - - Macro
- - - multi-Cursor
- - - Plugins ( Change to Scripts ? )
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ )
- organize/optimize menu functionality code
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode
- fix not being able to full screen drag on vert monitor ( wnd_drag.cpp )
- minimize var scope & const unchanging vars, clean/optimize files ( make file checklist )
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- add ss and make documentation
