#include "wnd_edit.h"

#include "../wnd_menu.h"

HWND txt_box;
HHOOK mouse_hook;
CSCROLL vscroll;

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  static bool once = false;
  vscroll.txt_rect = get_wnd_sz( hwnd );

  switch( msg ) {
  case WM_CHAR: {
    if( wp == VK_TAB ) {
      Edit_ReplaceSel( hwnd,(LPARAM)L"  \0" );
      return 0; 
    }
  } break;
  case WM_ERASEBKGND: {
    HDC hdc = (HDC)wp;
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
    FillRect( hdc, &vscroll.txt_rect, dbrush );
    DeleteObject( dbrush );

    if( !once ) {
      vscroll.cscroll_create( txt_box );
      once = true;
    }
    
    return 1;
  } break;
  case WM_KEYDOWN: {
    if( wp == 0x41 ) {
      if( GetAsyncKeyState( VK_CONTROL ) )
        Edit_SetSel( txt_box, 0, -1 );
    }
    vscroll.cscroll_draw( true, true );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

LRESULT CALLBACK mouse_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  if( ncode == HC_ACTION ) {
    MSLLHOOKSTRUCT* p_mouse = reinterpret_cast<MSLLHOOKSTRUCT*>( lp );

    switch( wp ) {
    case WM_MOUSEMOVE: {
      vscroll.cscroll_ishovered( p_mouse );

      if( vscroll.dragging )
        vscroll.cscroll_drag( p_mouse );
      else if( vscroll.mdragging )
        vscroll.cscroll_mbutton_scroll( p_mouse );

      wnd_menu_hovered( p_mouse );
    } break;
    case WM_MBUTTONDOWN: {
      vscroll.cscroll_mbutton_on( p_mouse );
    } break;
    case WM_MBUTTONUP: {
      vscroll.cscroll_mbutton_off();
    } break;
    case WM_MOUSEWHEEL: {
      vscroll.cscroll_hover_scroll( p_mouse );
      vscroll.cscroll_update();
    } break;
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}

void wnd_type_create( const HWND hwnd ) {
  txt_box = CreateWindowExW( WS_EX_TRANSPARENT,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL |
    ES_AUTOVSCROLL | ES_AUTOHSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrW( hwnd, GWLP_HINSTANCE ), 0
  );

  SetWindowSubclass( txt_box, editproc, 0, 0 );

  mouse_hook = SetWindowsHookExW(
    WH_MOUSE_LL, mouse_hook_proc,
    GetModuleHandleW( NULL ), 0
  );
}

void wnd_type_outline( const HWND hwnd, const POINT wnd_sz ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_M_GRY );
  RECT outline_sz {
    24, 49,
    wnd_sz.x - 24,
    wnd_sz.y - 24
  };
  FillRect( hdc, &outline_sz, brush );

  DeleteObject( brush );
  ReleaseDC( hwnd, hdc );
}

void wnd_type_customize( const WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );
}