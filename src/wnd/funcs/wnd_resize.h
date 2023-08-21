#pragma once
#include "../wincludes.h"

enum WND_RESIZE_CASES {
  EDGE_TOP_LEFT  = 1,
  EDGE_TOP         ,
  EDGE_TOP_RIGHT   ,
  EDGE_RIGHT       ,
  EDGE_BOTTOM_RIGHT,
  EDGE_BOTTOM      ,
  EDGE_BOTTOM_LEFT ,
  EDGE_LEFT
};

void wnd_resize_get_side( POINT m_pos, RECT wnd_sz );
void wnd_resize_get_cursor( POINT m_pos, RECT wnd_sz );
void wnd_resize_on( HWND hwnd, POINT m_pos, RECT wnd_sz );
void wnd_resize_off();
void wnd_resize_title( HWND hwnd, bool mouse_over );
void wnd_resize_check_bounds( HWND hwnd, LPPOINT wnd_pos, LPPOINT wnd_sz, POINT m_delta );
void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz );