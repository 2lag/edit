#include "wnd_type_caret.h"

void wnd_type_caret_pos( HWND hwnd, RECT wnd_sz ) {
  if( !txt_box )
    return;

  s32 curr_line_start_idx = (s32)SendMessageW( txt_box, EM_LINEINDEX, -1, 0 ) + 1;
  s32 curr_caret_idx = (s32)HIWORD( SendMessageW( txt_box, EM_GETSEL, 0, 0 ) ) + 1;
  s32 line_idx_x = curr_caret_idx - curr_line_start_idx;

  wchar_t caret_pos[32];
  swprintf_s( caret_pos,
    sizeof( caret_pos ) / sizeof( wchar_t ),
    L"X : %d   Y : %d\0",
    line_idx_x, vscroll.curr_line
  );

  HDC hdc = GetDC( hwnd );

  SIZE txt_sz;
  GetTextExtentPoint32W( hdc, caret_pos,
    (s32)wcslen( caret_pos ), &txt_sz
  );

  RECT r {
    wnd_sz.left + 5, wnd_sz.bottom - txt_sz.cy - 5,
    wnd_sz.right - 5, wnd_sz.bottom - 5
  };

  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  FillRect( hdc, &r, dbrush );

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );

  TextOutW( hdc,
    ( wnd_sz.right - txt_sz.cx ) / 2,
    wnd_sz.bottom - txt_sz.cy - 5,
    caret_pos, (s32)wcslen( caret_pos )
  );

  ReleaseDC( hwnd, hdc );
  DeleteObject( dbrush );
}