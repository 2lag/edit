#pragma once
#include "../../wincludes.h"

s32 menu_subdropdown_toggle( bool &check, bool &toggle );
s32 menu_dropdown_toggle( bool &toggle, const s32 idx );
s32 menu_edit_ctrl( bool &toggle, s32 idx );
s32 menu_new_wnd( const bool toggle );
s32 menu_toggle_style( s32 idx );