functionality :
- Macro
- - Record ( record keypresses, ignoring final 4 ( ctrl t, m, r ) ) also, add a toggled notice to it
- - - if macro toggled, record to vector in the keyboard hook getting every key pressed and saving it
- - Clear ( clear macro storage )
- - Playback ( playback recorded keypresses )
- Exit ( in File )
- multi-Cursor
- - TBFD... and FML.
- test current abilities and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )

clean up :
- change function names to not have wnd_ and generally just be more descriptive
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- - will fix previous conception of need for a global mouse position, while also improving performance
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, append error to local log w/ date & time if error is before text editor is working, or if text editor is not working
- add comments to non-self-explaning code
- full, final, in-depth reoptimization
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, make repo public, then start dev on debian making 3d app/game with sdl, vsc, and g++
