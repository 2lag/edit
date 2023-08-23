#include "wnd_type.h"

LRESULT CALLBACK editproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
  RECT rect{};
  GetClientRect( hwnd, &rect );

  switch( uMsg ) {
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wParam;
    RECT rect;
    GetClientRect( hwnd, &rect );
    HBRUSH brush = CreateSolidBrush( COL_D_GRY );
    FillRect( hdc, &rect, brush );
    DeleteObject( brush );
    return 1;
  } break;
  }

  return CallWindowProc( (WNDPROC)dwRefData, hwnd, uMsg, wParam, lParam );
}

void wnd_type_create( HWND hwnd, HWND &txt_box ) {
  txt_box = CreateWindowExW( 0L,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
    ES_LEFT | ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE ), 0
  );

  SetWindowSubclass( txt_box, editproc, 0,
    (DWORD_PTR)GetWindowLongPtr( txt_box, GWLP_WNDPROC )
  );
}

void wnd_type_outline( HWND hwnd, POINT wnd_sz ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_L_GRY );
  RECT r {
    24, 49,
    wnd_sz.x,
    wnd_sz.y - 24
  };
  FillRect( hdc, &r, brush );

  ReleaseDC( hwnd, hdc );
  DeleteObject( brush );
}

void wnd_type_customize( WPARAM wp ) {
  HDC hdc = (HDC)wp;
  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_L_GRY );
}