#pragma once
#include "../../wincludes.h"

s32 wnd_menu_subdropdown_toggle( bool &check, bool &toggle );
s32 wnd_menu_dropdown_toggle( bool &toggle, const s32 idx );
s32 wnd_menu_edit_ctrl( bool &toggle, s32 idx );
s32 wnd_menu_new_wnd( const bool toggle );

s32 wnd_menu_style_toggle( s32 idx );

void record_macro( u8 key );
s32 playback_macro();