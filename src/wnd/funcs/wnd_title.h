#pragma once
#include "../wincludes.h"

struct WND_OBJ {
  RECT r;
  HBRUSH col;
  bool next;
};

inline WND_OBJ wnd_obj_create( RECT r, COLORREF col, bool next ) {
  return { r, CreateSolidBrush( col ), next };
}

void wnd_title_draw( HDC hdc, POINT m_pos, RECT wnd_sz );
void wnd_title_cls( HWND hwnd, bool mouse_over );
void wnd_title_max( HWND hwnd, bool mouse_over );
void wnd_title_min( HWND hwnd, bool mouse_over );