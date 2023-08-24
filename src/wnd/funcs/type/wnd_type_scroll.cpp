#include "wnd_type_scroll.h"

LRESULT CALLBACK vscrollproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  switch( msg ) {
  case WM_PAINT: {
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_scroll_create( HWND hwnd, POINT pwnd_sz ) {
  vscroll = CreateWindowExW( 0L,
    L"ScrollBar", 0,
    WS_CHILD | WS_VISIBLE | SBS_VERT,
    0, 0, 0, 0, txt_box, 0, 0, 0
  );
  // setup scroll with bounds and start position
  // if scrollbar control lets you in create, do that
  

  // also figure out how to custom draw rest of scrollbar
  // definitely use getscrollinfo or whatever for pos/sz
  SetWindowSubclass( vscroll, vscrollproc, 0,
    (DWORD_PTR)GetWindowLongPtr( txt_box, GWLP_WNDPROC )
  );
}