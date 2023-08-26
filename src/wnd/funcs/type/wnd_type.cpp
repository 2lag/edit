#include "wnd_type.h"

HWND vscroll;
HWND txt_box;

SCROLLINFO si;

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  RECT text_sz{};
  GetClientRect( hwnd, &text_sz );

  SIZE txt_sz{};
  HDC hdc = GetDC( hwnd );
  
  GetTextExtentPoint32W( hdc, L"A", 1, &txt_sz );

  ReleaseDC( hwnd, hdc );

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
  case WM_VSCROLL: {
    s32 n_delta{};
    si.nPage = text_sz.bottom / txt_sz.cy;
    switch( LOWORD( wp ) ) {
    case SB_LINEUP: {
      n_delta -= 1;
    } break;
    case SB_LINEDOWN: {
      n_delta = 1;
    } break;
    case SB_PAGEUP: {
      n_delta = -(s32)si.nPage;
    } break;
    case SB_PAGEDOWN: {
      n_delta = si.nPage;
    } break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: {
      si.nPos = (s32)HIWORD( wp );
    } break;

    si.nPos += n_delta;
    si.nPos = max( 0, min( si.nMax, si.nPos ) );

    // set max and page ? make sure
    // maybe invalidate rect after too? no clue i hate winapi

    SetScrollInfo( vscroll, SB_CTL, &si, TRUE );
    ScrollWindowEx( hwnd, 0, -n_delta * txt_sz.cy, 0, 0, 0, 0, SW_ERASE | SW_INVALIDATE );
    }
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

  vscroll = CreateWindowExW( 0L,
    L"ScrollBar", 0,
    WS_CHILD | WS_VISIBLE | SBS_VERT,
    0, 0, 0, 0, txt_box, 0, 0, 0
  );

  si.cbSize = sizeof( SCROLLINFO );
  si.fMask = SIF_ALL;
  si.nMin = 0;
  si.nMax = 100;
  si.nPage = 10;
  si.nPos = 0;
  si.nTrackPos = 0;
  SetScrollInfo( vscroll, SB_CTL, &si, TRUE );

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

  ReleaseDC( hwnd, hdc );
  DeleteObject( brush );
}

void wnd_type_customize( WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_L_GRY );
}