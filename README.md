todo :
- check setcursor/cscroll_ishovered/line_count's getclientrect/getclientrect in general
- - make sure theyre only called when needed
- fix not being able to full screen drag on vert monitor ( wnd_drag.cpp )
- add menu bar & functionality + keybinds in new file ( wnd_type -> editproc -> wm_keydown )
- - make "one time renderer" so you pass info and it draws itself
- - - file
- - - - new
- - - - open
- - - - exit
- - - tools
- - - - macro
- - - - - clear
- - - - - - wipes macro storage
- - - - - record
- - - - - - store keypresses in array/vector ?
- - - - - playback
- - - - - - plays back keypresses
- - - - mutli-cursor
- - - style cfg controller ( extern globalize these settings )
- - - - hide fps
- - - - hide caret pos
- - - - hide line count
- minimize var scope | const vars | clean, optimize, and create files ( make file checklist )
- add ss and make documentation
