#include "wnd_edit.h"

#include "../hooks/wnd_key_hook.h"
#include "../hooks/wnd_mouse_hook.h"

#include "../menu/wnd_menu.h"

#include "wnd_edit_line_count.h"

HWND txt_box;
HHOOK mouse_hook;
HHOOK key_hook;
CSCROLL vscroll;

bool m_base_open[ OBJ_BASE_COUNT / 2 ] = { false };

void wnd_clear_menus( s32 exclude, bool overryde ) {
  bool should_run = false;
  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; ++idx ) {
    if( m_base_open[ idx ] )
      should_run = true;
      break;
  }

  if( !should_run )
    return;

  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; idx++ ) {
    if( !m_base_open[ idx ] || ( exclude == idx && !overryde ) )
      continue;

    m_base_open[ idx ] = false;
  }
  vscroll.cscroll_draw( true, true );

  RECT wnd_sz = get_wnd_sz( h_global );
  wnd_type_line_count( h_global, wnd_sz, true );
  wnd_type_outline( h_global, to_sz_point( wnd_sz ) );
}

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  static bool once = false;
  vscroll.txt_rect = get_wnd_sz( hwnd );

  switch( msg ) {
  case WM_CHAR: {
    if( wp == VK_TAB ) {
      Edit_ReplaceSel( hwnd,(LPARAM)"  \0" );
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
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

void wnd_type_create( const HWND hwnd ) {
  txt_box = CreateWindowExA( WS_EX_TRANSPARENT,
    "EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL |
    ES_AUTOVSCROLL | ES_AUTOHSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrA( hwnd, GWLP_HINSTANCE ), 0
  );

  SetWindowSubclass( txt_box, editproc, 0, 0 );

  mouse_hook = SetWindowsHookExA(
    WH_MOUSE_LL, mouse_hook_proc,
    GetModuleHandleA( NULL ), 0
  );

  key_hook = SetWindowsHookExA(
    WH_KEYBOARD_LL, key_hook_proc,
    GetModuleHandleA( NULL ), 0
  );
}

void wnd_type_outline( const HWND hwnd, const POINT wnd_sz ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_M_GRY );
  RECT outline_sz {
    WND_BTN_SZ - 1,
    ( WND_BTN_SZ * 2 ) - 1,
    wnd_sz.x - ( WND_BTN_SZ - 1 ),
    wnd_sz.y - ( WND_BTN_SZ - 1 )
  };
  FillRect( hdc, &outline_sz, brush );

  DeleteObject( brush );
  ReleaseDC( hwnd, hdc );
}

void wnd_customize( const WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_M_GRY );
}