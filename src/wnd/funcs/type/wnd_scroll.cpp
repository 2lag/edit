#include "wnd_scroll.h"

RECT gtext_sz;
s32 scroll_pos{};

LRESULT CALLBACK vscrollproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  POINT ptext_sz = to_sz_point( gtext_sz );

  switch( msg ) {
  case WM_VSCROLL: {
    switch( LOWORD( wp ) ) { // add sb_pageup/down
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: {
      scroll_pos = HIWORD( wp );
    } break;
    case SB_LINEUP: {
      --scroll_pos;
    } break;
    case SB_LINEDOWN: {
      ++scroll_pos;
    } break;
    }
    scroll_pos = max( 0 , min( ptext_sz.y, scroll_pos ) );
    SetScrollPos( hwnd, SB_CTL, scroll_pos, TRUE );
  } break;
  }

  return CallWindowProc( (WNDPROC)data, hwnd, msg, wp, lp );
}

void wnd_type_scroll( HWND hwnd, RECT text_sz ) {
  gtext_sz = text_sz;
  POINT ptext_sz = to_sz_point( text_sz );

  HWND v_scroll = CreateWindowExW( 0L,
    L"SCROLLBAR", 0,
    WS_CHILD | SBS_VERT | WS_VISIBLE,
    0, 0,
    20, 100,
    txt_box, 0, 0, 0
  );

  /*
  SetScrollRange( v_scroll, SB_CTL, 0, ptext_sz.y, TRUE );
  SetScrollPos( v_scroll, SB_CTL, scroll_pos, TRUE );

  SetWindowSubclass( v_scroll, vscrollproc, 1,
    (DWORD_PTR)GetWindowLongPtr( v_scroll, GWLP_WNDPROC )
  );
  */
}