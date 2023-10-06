#include "wnd_type_line_count.h"

s32 prev_line = 0;

void wnd_type_line_count( HWND hwnd, RECT wnd_sz ) {
  if( !txt_box || prev_line == vscroll.line_first )
    return;

  HDC hdc = GetDC( hwnd );

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );
  
  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  
  RECT r {
    0, 50, 24,
    wnd_sz.bottom - 25
  };
  FillRect( hdc, &r, dbrush );

  s32 idx = 0;
  for( s32 curr_line = vscroll.line_first; curr_line < vscroll.line_first + vscroll.lines_vis; curr_line++, idx++ ) {
    SIZE txt_sz;
    wchar_t line[32];
    swprintf_s( line,
      sizeof( line ) / sizeof( wchar_t ),
      L"%d\0", curr_line
    );

    GetTextExtentPoint32W( hdc, line, (s32)wcslen( line ), &txt_sz );

    TextOutW( hdc,
      ( 24 - txt_sz.cx ) / 2,
      51 + ( txt_sz.cy * idx ),
      line, (s32)wcslen( line )
    );
  }

  ReleaseDC( hwnd, hdc );
  DeleteObject( dbrush );

  prev_line = vscroll.line_first;
}