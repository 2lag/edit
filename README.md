### fix :
- remove mousehook and recode as needed
- - maybe same w/ keyhook?

### clean up :
- (void)XXX all functions with ignored return values, add checks for important ret values in all files
- check all files for missed comment-notes & add comments to non-self-descriptive code

### final clean up :
- optimization w valgrind as well as vs debugger
- test everything ( make list of stuff from each file's funcs )
- add ss, make documentation, make repo public

### Possible additions for the future :
- Swap mouse hook for [raw input](https://learn.microsoft.com/en-us/windows/win32/inputdev/raw-input)
- Globalize wnd_pos the same way wnd_sz is done
- Better error logs & overall handling
- Drag 'n Drop to open files
- Add support for multiple tabs
- Multi-Cursor
- Implement support for alternative encoding ( UTF-16, UTF-32, etc. )
- Swap Windows API for X-Plat GUI/Interface ( Fighting Microsoft is not fun )