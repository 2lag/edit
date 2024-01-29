functionality :
- fix macro playback
- - figure out what voodoo is causing playback to not work right ??
- - make ctrl, shift, & alt functionality, all work properly
- multi-Cursor - TBD
- test current abilities and implement missing support for alternative pointless encoding formats ( utf-16, utf-32, etc. )

clean up :
- change function names to not have wnd_ and generally just be more descriptive
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- - will fix previous conception of need for a global mouse position, while also improving performance
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, append error to local log w/ date & time if error is before text editor is working, or if text editor is not working
- check all files for missed comment-notes & add comments to non-self-descriptive code
- full, final, in-depth reoptimization ( use valgrind as well )
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, make repo public, then start dev on debian making :
- - 3d app/game with sdl, vsc, and g++ [here](https://www.libsdl.org/)
- - keyboard program using sine wave generation [here](https://www.youtube.com/watch?v=5xd9BMxoXqo)
- - custom driver ( access mouse and get input being sent ( if possible ) ) [here]()
- - operating system [here](https://www.youtube.com/watch?v=9t-SPC7Tczc)