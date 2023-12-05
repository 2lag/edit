#pragma once
#include "../../wincludes.h"

s32 wnd_menu_dropdown_toggle( bool &toggle, s32 idx );
s32 wnd_menu_subdropdown_toggle( bool &check, bool &toggle );
s32 wnd_menu_new_wnd( bool &toggle );
s32 wnd_menu_edit_ctrl( bool &toggle, s32 idx );

s32 wnd_menu_style_toggle( s32 idx );