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

void wnd_title_draw( const HDC hdc, const RECT wnd_sz );
void wnd_title_cls( const bool mouse_over );
void wnd_title_max( const HWND hwnd, const bool mouse_over );
void wnd_title_min( const HWND hwnd, const bool mouse_over );