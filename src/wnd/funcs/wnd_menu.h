#pragma once
#include "../wincludes.h"

struct WND_MENU : public WND_OBJ {
  LPCWSTR txt;
  POINT offset;
};

inline WND_MENU wnd_menu_create( RECT r, COLORREF col, bool next, LPCWSTR txt, POINT offset ) {
  return { r, CreateSolidBrush( col ), next, txt, offset };
}

void wnd_clear_menus( s32 ignore = -1, bool overryde = false );

void wnd_menu_draw( HDC hdc, RECT wnd_sz );
void wnd_menu_draw_dropdown( HWND hwnd, s8 idx );
void wnd_menu_draw_sub_dropdown( HWND hwnd );