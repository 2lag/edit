#include "wnd_type_line_count.h"

void wnd_type_line_count( const HWND hwnd, const RECT wnd_sz ) {
  static s32 prev_line = 0;
  if( !txt_box || prev_line == vscroll.line_first )
    return;

  HDC hdc = GetDC( hwnd );

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );
  
  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  
  RECT r { 0, 50, 24, wnd_sz.bottom - 25 };
  FillRect( hdc, &r, dbrush );

  for( s32 curr_line = vscroll.line_first; curr_line < vscroll.line_first + vscroll.lines_vis; curr_line++ ) {
    SIZE txt_sz;
    wchar_t line[32];
    swprintf_s( line,
      sizeof( line ) / sizeof( wchar_t ),
      L"%d\0", curr_line
    );

    GetTextExtentPoint32W( hdc, line,
      (s32)wcslen( line ), &txt_sz
    );

    TextOutW( hdc,
      ( 24 - txt_sz.cx ) / 2,
      51 + ( txt_sz.cy * ( curr_line - vscroll.line_first ) ),
      line, (s32)wcslen( line )
    );
  }

  ReleaseDC( hwnd, hdc );
  DeleteObject( dbrush );

  prev_line = vscroll.line_first;
}