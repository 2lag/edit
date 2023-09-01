#include "wnd_type.h"

#include "wnd_type_scroll.h"

HWND txt_box;
CSCROLL vscroll;

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  RECT text_sz{};
  GetClientRect( hwnd, &text_sz );

  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_TAB )
      break;

    SendMessageW( hwnd, EM_REPLACESEL, FALSE, (LPARAM)L"  \0" );
    return 0;
  } break;
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wp;
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
    FillRect( hdc, &text_sz, dbrush );
    DeleteObject( dbrush );

    vscroll.cscroll_create( txt_box );
    return 1;
    
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_type_create( HWND hwnd, POINT pwnd_sz ) {
  txt_box = CreateWindowExW( WS_EX_TRANSPARENT,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL | ES_AUTOVSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE ), 0
  );

  SetWindowSubclass( txt_box, editproc, 0, 0 );
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

  DeleteObject( brush );
  ReleaseDC( hwnd, hdc );
}

void wnd_type_customize( WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_L_GRY );
}