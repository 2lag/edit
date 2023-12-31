#include "wnd_edit_caret.h"

RECT caret_rect;

void wnd_type_caret_pos( const HWND hwnd, const RECT wnd_sz, const bool force_redraw ) {
  static bool caret_covered = false;
  static s64 prev_sel = 0;

  if( ( !menu_style_toggle[ CARET ] && caret_covered ) || !txt_box )
    return;

  s64 sel = Edit_GetSel( txt_box );

  if( sel == prev_sel && !force_redraw )
    return;
    
  prev_sel = sel;
  caret_covered = !menu_style_toggle[ CARET ];

  s32 curr_line_start_idx = Edit_LineIndex( txt_box, -1 ) + 1;
  s32 curr_caret_idx = static_cast<s32>( HIWORD( sel ) ) + 1;
  s32 line_idx_x = curr_caret_idx - curr_line_start_idx;

  char caret_pos[128];
  sprintf_s( caret_pos,
    sizeof( caret_pos ) / sizeof( wchar_t ),
    "X : %d   Y : %d\0",
    line_idx_x, vscroll.curr_line
  );

  HDC hdc = GetDC( hwnd );

  SIZE txt_sz;
  GetTextExtentPoint32A( hdc, caret_pos,
    (s32)strlen( caret_pos ), &txt_sz
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

  if( menu_style_toggle[ CARET ] ) {
    TextOutA( hdc,
      ( wnd_sz.right - txt_sz.cx ) / 2,
      wnd_sz.bottom - txt_sz.cy - 5,
      caret_pos, (s32)strlen( caret_pos )
    );
  } else
    caret_covered = true;

  ReleaseDC( hwnd, hdc );
  DeleteObject( dbrush );
}