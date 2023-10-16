#include "wnd_edit_caret.h"

RECT caret_rect;

void wnd_type_caret_pos( const HWND hwnd, const RECT wnd_sz, bool force_redraw ) {
  if( !txt_box )
    return;

  static s64 prev_sel = 0;
  s64 sel = Edit_GetSel( txt_box );

  if( sel != prev_sel )
    prev_sel = sel;
  else if( !force_redraw )
    return;

  s32 curr_line_start_idx = Edit_LineIndex( txt_box, -1 ) + 1;
  s32 curr_caret_idx = (s32)HIWORD( Edit_GetSel( txt_box ) ) + 1;
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

  caret_rect = {
    ( wnd_sz.right - txt_sz.cx ) / 2 - 20,
    wnd_sz.bottom - txt_sz.cy - 5,
    ( wnd_sz.right + txt_sz.cx ) / 2 + 20,
    wnd_sz.bottom - 5
  };

  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  FillRect( hdc, &caret_rect, dbrush );

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