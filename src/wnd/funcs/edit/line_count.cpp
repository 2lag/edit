#include "line_count.h"

HDC wnd_edit_line_count_draw_bg( const HWND hwnd,
                                 const bool ret_hdc = false ) {
  HDC hdc = GetDC( hwnd );

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );

  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  
  RECT r { 0, 50, 24, wnd_sz.bottom - 25 };
  FillRect( hdc, &r, dbrush );

  DeleteObject( dbrush );

  if( ret_hdc )
    return hdc;

  ReleaseDC( hwnd, hdc );
  return nullptr;
}

void wnd_type_line_count( const HWND hwnd,
                          const bool force_redraw ) {
  static bool hide_line_count_prev = true;

  if( !menu_style_toggle[ LINE_COUNT ] || !txt_box ) {
    if( !hide_line_count_prev || force_redraw ) {
      wnd_edit_line_count_draw_bg( hwnd );
      hide_line_count_prev = true;
    }
    return;
  } else
    hide_line_count_prev = false;

  static s32 prev_line = 0;
  if( prev_line == vscroll.curr_line && !force_redraw )
    return;

  if( vscroll.curr_line  == vscroll.line_first + vscroll.lines_vis &&
      vscroll.line_first == vscroll.line_last  - vscroll.lines_vis + 1 )
    vscroll.line_first++;

  HDC hdc = wnd_edit_line_count_draw_bg( hwnd, true );

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

  prev_line = vscroll.curr_line;
}