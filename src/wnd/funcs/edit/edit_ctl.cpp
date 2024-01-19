#include "edit_ctl.h"
#include "line_count.h"

#include "../menu/menu.h"
#include "../hooks/hooks.h"


HWND txt_box;
HHOOK key_hook;
CSCROLL vscroll;
HHOOK mouse_hook;
bool m_base_open[ OBJ_BASE_COUNT / 2 ] = { false };

bool wnd_should_clear_menus() {
  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; ++idx ) {
    if( m_base_open[ idx ] )
      return true;
  }
  return false;
};

void wnd_clear_menus( const s32 exclude, const bool overryde ) {
  if( !wnd_should_clear_menus() )
    return;

  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; idx++ ) {
    if( !m_base_open[ idx ] || ( exclude == idx && !overryde ) )
      continue;

    m_base_open[ idx ] = false;
  }

  vscroll.draw( true, true );
  wnd_type_line_count( h_global, true );
  wnd_type_outline( h_global );
}

LRESULT CALLBACK editproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
  static bool once = false;

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
      // only spawn once when txt_box exists
      vscroll.create( txt_box );
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

  SetFocus( txt_box );

  wnd_init_hooks();
}

void wnd_type_outline( const HWND hwnd ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_M_GRY );
  RECT outline_sz {
    WND_BTN_SZ - 1,
    ( WND_BTN_SZ * 2 ) - 1,
    get_size( wnd_sz ).x - ( WND_BTN_SZ - 1 ),
    get_size( wnd_sz ).y - ( WND_BTN_SZ - 1 )
  };
  FillRect( hdc, &outline_sz, brush );

  // maybe swap this with validaterect inside_sz ?
  HBRUSH bg_brush = CreateSolidBrush( COL_D_GRY );
  RECT inside_sz = outline_sz - 1;
  FillRect( hdc, &inside_sz, bg_brush );
  
  DeleteObject( bg_brush );
  DeleteObject( brush );

  ReleaseDC( hwnd, hdc );
}

void wnd_customize( const WPARAM wp ) {
  HDC hdc = (HDC)wp;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_M_GRY );
}