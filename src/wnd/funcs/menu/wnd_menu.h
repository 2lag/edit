#pragma once
#include "../../wincludes.h"

struct WND_MENU : public WND_OBJ {
  const char *txt;
};

inline WND_MENU wnd_menu_create( const RECT r,
                                 const COLORREF col,
                                 const bool next,
                                 const char *txt ) {
  return { r, CreateSolidBrush( col ), next, txt };
}

void wnd_clear_menus( const s32 ignore = -1, const bool overryde = false );

void wnd_menu_draw( const HDC hdc );
void wnd_menu_draw_dropdown( const HWND hwnd, const s8 idx );
void wnd_menu_draw_sub_dropdown( const HWND hwnd );