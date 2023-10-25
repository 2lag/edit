#pragma once
#include "../wincludes.h"

struct WND_MENU : public WND_OBJ {
  LPCWSTR txt;
  POINT offset;
};

inline WND_MENU wnd_menu_create( RECT r, COLORREF col, bool next, LPCWSTR txt, POINT offset ) {
  return { r, CreateSolidBrush( col ), next, txt, offset };
}

void wnd_menu_draw( HDC hdc, RECT wnd_sz );
void wnd_menu_hovered( MSLLHOOKSTRUCT* p_mouse );
void wnd_menu_clicked( HWND hwnd, POINT m_pos );