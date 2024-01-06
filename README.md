finish :
- organize/optimize, minimize var scopes & const unchanging vars, clean/optimize all files, proper bug handling, etc
- - wnd_menu_funcs.cpp/h
- - wnd_func.cpp/h
- - source.cpp
- globalize window size, prev window size, window pos, prev window pos, ( mouse pos, prev mouse pos ( global & make func to convert to local ) ) ( avoids recomputation/recalling )
- make sure fps is calculating properly
- run profiler and optimize more
- plan functionality for :
- - Tools
- - - Find
- - - Replace
- - - Macro
- - - multi-Cursor
- - - Scripts
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ ? )
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- test current and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, concat to local log w/ date & time if error is before text editor is working, or if text editor is not working
- full, final, in-depth reoptimization
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- change file names to be more descriptive ( wnd_title -> titlebar )
- add ss, make documentation, then make repo public