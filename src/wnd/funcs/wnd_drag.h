#pragma once
#include "../wincludes.h"

void wnd_drag_on( const HWND hwnd, const POINT m_pos, const RECT wnd_sz );
void wnd_drag_off();
void wnd_drag( const HWND hwnd, const POINT m_pos );
void wnd_drag_resize( const HWND hwnd );