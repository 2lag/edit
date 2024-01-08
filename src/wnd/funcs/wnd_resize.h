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

void wnd_resize_get_cursor( const POINT m_pos, const RECT wnd_sz );
void wnd_resize_on( const HWND hwnd, const POINT m_pos, const RECT wnd_sz );
void wnd_resize_off();
void wnd_resize_title( const HWND hwnd, const RECT wnd_sz, const POINT m_pos );
void wnd_resize( const HWND hwnd, const POINT m_pos, const RECT wnd_sz );