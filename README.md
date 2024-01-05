finish :
- organize/optimize, minimize var scopes & const unchanging vars, clean/optimize all files, proper bug handling, etc
- - wnd_title ( same way as wnd_menu )
- - wnd_edit_scroll.h
- - wnd_menu_funcs.cpp/h
- - wnd_func.cpp/h
- - source.cpp
- swap all centered text to use the (v)(h)center_text macro
- globalize window size and previous window size, same w/ respective pos's set w to_pos_point @ same time ( avoids recomputation/recalling )
- - do same to mouse pos ( both global and local to window ? )
- make sure fps is calculating properly
- run profiler and optimize more
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, concat to local log w/ date & time if error is before text editor is working, or if text editor is not working
- plan functionality for :
- - Tools
- - - Find
- - - Replace
- - - Macro
- - - multi-Cursor
- - - Tools -> Plugins ( Custom scripting language )
- - - - figure out how to permit/enable custom scripting ( .edit file in ./scripts/ ? )
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- test current and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )
- full, final reoptimization
- add comments to non-self-explaning code
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, then make repo public