#include "wnd_type.h"

HWND txt_box;
SCROLLBARINFO sbi;

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  RECT text_sz{};
  GetClientRect( hwnd, &text_sz );

  switch( msg ) {
  case WM_CHAR: {
    if (wp == VK_TAB) {
      SendMessageW( hwnd, EM_REPLACESEL, FALSE, (LPARAM)L"  \0" );
      return 0;
    }
  } break;
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wp;
    HBRUSH brush = CreateSolidBrush( COL_D_GRY );

    FillRect( hdc, &text_sz, brush );
    
    DeleteObject( brush );
    return 1;
  } break;
  case WM_NCPAINT:
  case WM_PAINT:
  case WM_VSCROLL: {
    wnd_type_scroll_draw( hwnd );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_type_create( HWND hwnd, POINT pwnd_sz ) {
  txt_box = CreateWindowExW( 0L,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL |
    WS_VSCROLL | ES_AUTOVSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE ), 0
  );

  sbi.cbSize = sizeof( SCROLLBARINFO );

  SendMessageW( txt_box, SBM_ENABLE_ARROWS, ESB_DISABLE_BOTH, 0 );

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

  DeleteObject( brush );
  ReleaseDC( hwnd, hdc );
}

void wnd_type_customize( WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_L_GRY );
}

void wnd_type_scroll_draw( HWND hwnd ) {
  GetScrollBarInfo( hwnd, OBJID_VSCROLL, &sbi );

  ScreenToClient( hwnd, reinterpret_cast<POINT*>( &sbi.rcScrollBar.left ) );
  ScreenToClient( hwnd, reinterpret_cast<POINT*>( &sbi.rcScrollBar.right ) );

  PAINTSTRUCT ps;
  HDC hdc = BeginPaint( hwnd, &ps );

  if( !ps.rcPaint ) {
    EndPaint( hwnd, &ps );
    RedrawWindow( hwnd, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW );
    return;
  }
#ifdef _DEBUG
  std::cout << "Scroll  " << sbi.rcScrollBar.left << " " << sbi.rcScrollBar.top << " " << sbi.rcScrollBar.right << " " << sbi.rcScrollBar.bottom << std::endl;
  std::cout << "rcPaint " << ps.rcPaint.left << " " << ps.rcPaint.top << " " << ps.rcPaint.right << " " << ps.rcPaint.bottom << std::endl;
  std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
#endif
  ps.fErase = TRUE;
  HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
         lbrush = CreateSolidBrush( COL_L_GRY );

  RECT scroll_grab{
    sbi.rcScrollBar.left, sbi.xyThumbTop - 17,
    sbi.rcScrollBar.right, sbi.xyThumbBottom + 18
  };

  FillRect( hdc, &sbi.rcScrollBar, dbrush );
  FillRect( hdc, &scroll_grab, lbrush );

  EndPaint( hwnd, &ps );
  DeleteObject( dbrush );
  DeleteObject( lbrush );
  
  InvalidateRect( hwnd, 0, false ); // swap to immediate draw call
}