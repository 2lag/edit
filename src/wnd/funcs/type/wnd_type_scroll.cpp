#include "wnd_type_scroll.h"

HWND vscroll;

SCROLLINFO si;
SCROLLBARINFO sbi;

LRESULT CALLBACK scrollproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {

  switch( msg ) {
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_type_scroll_create( HWND hwnd , POINT pwnd_sz ) {
  vscroll = CreateWindowExW( 0L,
    L"SCROLLBAR", (PTSTR)0,
    WS_CHILD | WS_VISIBLE | SBS_VERT,
    pwnd_sz.x - 40, -17,
    15, pwnd_sz.y - 40,
    txt_box, (HMENU)0,
    (HINSTANCE)GetWindowLongPtrW( txt_box, GWLP_HINSTANCE ),
    (PVOID)0
  );

  si.cbSize  = sizeof( SCROLLINFO    );
  sbi.cbSize = sizeof( SCROLLBARINFO );

  si.fMask = SIF_ALL;
  si.nMin  = 0;
  si.nMax  = 0;
  si.nPage = 1;
  si.nPos  = 0;
  SetScrollInfo( vscroll, SB_CTL, &si, TRUE );

  int elm[1] { COLOR_3DFACE };
  COLORREF col[1] { COL_L_GRY };

  SetSysColors( 1, elm, col );

  SetWindowSubclass( vscroll, scrollproc, 0, 0 );
}