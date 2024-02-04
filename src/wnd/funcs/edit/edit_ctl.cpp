#include "edit_ctl.h"
#include "line_count.h"

#include "../menu/menu.h"
#include "../hooks/hooks.h"


HWND txt_box;
HHOOK key_hook;
CSCROLL vscroll;
HHOOK mouse_hook;
bool m_base_open[ OBJ_BASE_COUNT / 2 ] = { false };

bool should_clear_menus() {
  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; ++idx ) {
    if( m_base_open[ idx ] )
      return true;
  }
  return false;
};

void clear_menus( const s32 exclude, const bool overryde ) {
  if( !should_clear_menus() )
    return;

  for( s8 idx = 0; idx < OBJ_BASE_COUNT / 2; idx++ ) {
    if( !m_base_open[ idx ] || ( exclude == idx && !overryde ) )
      continue;

    m_base_open[ idx ] = false;
  }

  vscroll.draw( true, true );
  draw_line_count( h_global, true );
  draw_outline( h_global );
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
    (void)FillRect( hdc, &vscroll.txt_rect, dbrush );
    (void)DeleteObject( dbrush );

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

void create_edit_ctl( const HWND hwnd ) {
  txt_box = CreateWindowExA( WS_EX_TRANSPARENT,
    "EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_MULTILINE |
    ES_WANTRETURN | ES_NOHIDESEL |
    ES_AUTOVSCROLL | ES_AUTOHSCROLL,
    0, 0, 0, 0, hwnd, 0,
    (HINSTANCE)GetWindowLongPtrA( hwnd, GWLP_HINSTANCE ), 0
  );

  (void)SetWindowSubclass( txt_box, editproc, 0, 0 );

  (void)SetFocus( txt_box );

  init_hooks();
}

void draw_outline( const HWND hwnd ) {
  HDC hdc = GetDC( hwnd );
  HBRUSH brush = CreateSolidBrush( COL_M_GRY );
  RECT outline_sz {
    WND_BTN_SZ - 1,
    ( WND_BTN_SZ * 2 ) - 1,
    get_size( wnd_sz ).x - ( WND_BTN_SZ - 1 ),
    get_size( wnd_sz ).y - ( WND_BTN_SZ - 1 )
  };
  (void)FillRect( hdc, &outline_sz, brush );

  HBRUSH bg_brush = CreateSolidBrush( COL_D_GRY );
  RECT inside_sz = outline_sz - 1;
  (void)FillRect( hdc, &inside_sz, bg_brush );
  
  (void)DeleteObject( bg_brush );
  (void)DeleteObject( brush );

  (void)ReleaseDC( hwnd, hdc );
}

void set_colors( const WPARAM wp ) {
  HDC hdc = (HDC)wp;

  (void)SetBkColor( hdc, COL_D_GRY );
  (void)SetTextColor( hdc, COL_M_GRY );
}