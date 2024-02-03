### clean up :
- move any mouse functionality to mousehook & same with any keyboard 1-by-1 & recode as needed
- - will fix previous conception of need for a global mouse position, while also improving performance
- clean up includes with function "find all"'s
- (void)XXX all functions with ignored return values, add checks for important ret values in all files, append error to local log w/ date & time if error is before text editor is working, or if text editor is not working
- check all files for missed comment-notes & add comments to non-self-descriptive code

### final clean up :
- full, final, in-depth reoptimization ( use valgrind as well )
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, make repo public

### Possible additions for the future :
- Drag 'n Drop to open files
- Add support for multiple tabs
- Multi-Cursor
- Implement support for alternative encoding ( UTF-16, UTF-32, etc. )
- Swap Windows API for X-Plat GUI/Interface ( Fighting Microsoft is not fun )