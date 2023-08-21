#pragma once
#include "../wincludes.h"

void wnd_drag_on( HWND hwnd, POINT m_pos, bool mouse_over );
void wnd_drag_off();
void wnd_drag( HWND hwnd, POINT m_pos );
void wnd_drag_resize( HWND hwnd, POINT m_pos );