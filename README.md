# Edit
![image](https://github.com/2lag/edit/assets/96544487/d08660b2-9398-4da5-8c56-b01ae2f3b8ea)  

## Locations
| File | Contains |
| ------------- | ------------- |
| source.cpp | WinMain, FPS & config init |
| typedef.h | Custom type definitions |
| wincludes.h | Hectic includes, globally required externs, misc. functions/macros |
| wnd_proc.cpp/h | Main window procedure, handles most of everything |
| drag.cpp/h | All functionality relating to window dragging |
| fps.cpp/h | Functions to claculate and draw FPS |
| resize.cpp/h | All functionality relating to window resizing |
| title.cpp/h | Drawing & functionality of the custom title bar |
| caret_pos.cpp/h | Drawing & calculation of caret position |
| edit_ctl.cpp/h | Edit control creation & procedure, styling, and helper functions for window menus |
| line_count.cpp/h | Drawing & calculation of line count |
| macro.cpp/h | Macro record/playback functionality |
| scroll.h | 100% custom scrollbar that took 2 months of my time |
| hooks.h | Initialize & unhook methods |
| key_hook.cpp/h | Low-level keyboard hook, manages non-default keybinds |
| mouse_hook.cpp/h | Low-level mouse hook, calls functions that must ignore default functionality and interaction |
| menu.cpp/h | Drawing window menu |
| menu_funcs.cpp/h | Hectic file containing majority of menu functionality & sub-window procedures |
| config.cpp/h | All config functionality |
| overloads.h | Overloads for easy working with POINTs and RECTs |
| util.h | Small helper functions for cleanliness |  

## Possible additions for the future :
- [ ] Swap mouse hook for [raw input](https://learn.microsoft.com/en-us/windows/win32/inputdev/raw-input)
- [ ] Globalize wnd_pos the same way wnd_sz is done
- [ ] Better error logs & handling
- [ ] Drag 'n Drop to open files
- [ ] Add support for multiple tabs
- [ ] Multi-Cursor
- [ ] Implement support for alternative encoding ( UTF-16, UTF-32, etc. )
- [ ] Swap Windows API for X-Plat GUI/Interface ( Fighting Microsoft is not fun )
