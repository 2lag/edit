functionality :
- Find
- - same as open/save proc, open a similar menu_txt and search for then select text, then reset focus onto menu text in case they want more
- - close menu_txt when no more matches are found, or none existed in the first place
- - make sure to resize same way as menu_txt too
- Replace
- - open 2 menu_txts in the top, draw a line in between with setpixel, and do same closing as above
- Macro
- - Clear ( clear macro storage )
- - Record ( record keypresses, ignoring final 4 ( ctrl t, m, r ) ) also, add a toggled notice to it
- - Playback ( playback recorded keypresses )
- multi-Cursor
- - TBFD... and FML.
- test current abilities and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )

clean up :
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- - will fix previous conception of need for a global mouse position, while also improving performance
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, append error to local log w/ date & time if error is before text editor is working, or if text editor is not working
- change function names to not have wnd_ and generally just be more descriptive
- add comments to non-self-explaning code
- full, final, in-depth reoptimization
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, make repo public, then start dev on debian making 3d app/game with sdl, vsc, and g++