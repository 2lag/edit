#include "wnd_type.h"
#include "wnd_type_scroll.h"

HWND vscroll;
HWND txt_box;

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  RECT text_sz{};
  GetClientRect( hwnd, &text_sz );

  switch( msg ) {
  case WM_CTLCOLORSCROLLBAR: {
    HDC hdc = (HDC)wp;

    return (LRESULT)CreateSolidBrush( COL_D_GRY );
  } break;
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wp;
    HBRUSH brush = CreateSolidBrush( COL_D_GRY );

    FillRect( hdc, &text_sz, brush );
    DeleteObject( brush );

    return 1;
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_type_create( HWND hwnd, POINT pwnd_sz ) {
  txt_box = CreateWindowExW( 0L,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOVSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE ), 0
  );

  wnd_scroll_create( hwnd, pwnd_sz );

  SetWindowSubclass( txt_box, editproc, 0,
    (DWORD_PTR)GetWindowLongPtr( txt_box, GWLP_WNDPROC )
  );
}

void wnd_type_outline( HWND hwnd, POINT wnd_sz ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_L_GRY );
  RECT outline_sz {
    24, 49,
    wnd_sz.x,
    wnd_sz.y - 24
  };
  FillRect( hdc, &outline_sz, brush );

  ReleaseDC( hwnd, hdc );
  DeleteObject( brush );
}

void wnd_type_customize( WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_L_GRY );
}