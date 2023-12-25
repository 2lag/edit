#include "wnd_edit_line_count.h"

void wnd_type_line_count( const HWND hwnd, const RECT wnd_sz, const bool force_redraw ) {
  if( !menu_style_toggle[ LINE_COUNT ] || !txt_box )
    return;

  static s32 prev_line = 0;
  if( prev_line == vscroll.curr_line && !force_redraw )
    return;

  HDC hdc = GetDC( hwnd );

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );
  
  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  
  RECT r { 0, 50, 24, wnd_sz.bottom - 25 };
  FillRect( hdc, &r, dbrush );

  if( vscroll.curr_line  == vscroll.line_first + vscroll.lines_vis &&
      vscroll.line_first == vscroll.line_last  - vscroll.lines_vis + 1 )
    vscroll.line_first++;

  for( s32 curr_line = vscroll.line_first;
           curr_line < vscroll.line_first + vscroll.lines_vis &&
           curr_line <= vscroll.line_count;
           curr_line++ ) {
    SIZE txt_sz;
    char line[32];
    sprintf_s( line,
      sizeof( line ) / sizeof( wchar_t ),
      "%d\0", curr_line
    );

    GetTextExtentPoint32A( hdc, line, (s32)strlen( line ), &txt_sz );

    TextOutA( hdc,
      ( 24 - txt_sz.cx ) / 2,
      50 + ( txt_sz.cy * ( curr_line - vscroll.line_first ) ),
      line, (s32)strlen( line )
    );
  }

  ReleaseDC( hwnd, hdc );
  DeleteObject( dbrush );

  prev_line = vscroll.curr_line;
}