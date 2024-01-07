#include "wnd_func.h"
#include "funcs/wnd_includes.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz = get_wnd_sz( hwnd );
  POINT m_pos{};
  GetCursorPos( &m_pos );
  ScreenToClient( hwnd, &m_pos );

  const RECT drag {                WND_BTN_SZ * 0,              6, wnd_sz.right - WND_BTN_SZ * 3, WND_BTN_SZ },
              cls { wnd_sz.right - WND_BTN_SZ * 1, WND_BTN_SZ / 5, wnd_sz.right -              5, WND_BTN_SZ },
              max { wnd_sz.right - WND_BTN_SZ * 2, WND_BTN_SZ / 5, wnd_sz.right - WND_BTN_SZ * 1, WND_BTN_SZ },
              min { wnd_sz.right - WND_BTN_SZ * 3, WND_BTN_SZ / 5, wnd_sz.right - WND_BTN_SZ * 2, WND_BTN_SZ };

  vscroll.update();
  wnd_type_line_count( hwnd, wnd_sz );
  wnd_type_caret_pos( hwnd, wnd_sz );

  switch( msg ) {
  case WM_CREATE: {
    wnd_type_create( hwnd );
  } break;
  case WM_CTLCOLOREDIT: {
    wnd_customize( wp );
    return (LRESULT)GetStockObject( 0 );
  } break;
  case WM_DESTROY: {
    wnd_title_cls( true );
    return 0;
  } break;
  case WM_LBUTTONDBLCLK: {
    wnd_resize_title( hwnd, PtInRect( &drag, m_pos ) );
  } break;
  case WM_LBUTTONDOWN: {
    wnd_resize_on( hwnd, m_pos, wnd_sz );
    wnd_drag_on( hwnd, m_pos, PtInRect( &drag, m_pos ) );
    
    vscroll.drag_on( m_pos );

    wnd_title_cls( PtInRect( &cls, m_pos ) );
    wnd_title_max( PtInRect( &max, m_pos ) );
    wnd_title_min( PtInRect( &min, m_pos ) );
  } break;
  case WM_LBUTTONUP: {
    wnd_drag_resize( hwnd );

    wnd_drag_off();
    wnd_resize_off();

    vscroll.drag_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );

    if( is_maxd ) break;

    wnd_resize_get_cursor( m_pos, wnd_sz );
    wnd_resize( hwnd, m_pos, wnd_sz );
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( hwnd, &ps );

    wnd_title_draw( hdc, wnd_sz );
    wnd_type_outline( hwnd, get_size( wnd_sz ) );
    wnd_menu_draw( hdc );

    EndPaint( hwnd, &ps );
  } break;
  case WM_SIZE: {
    MoveWindow( txt_box,
      WND_BTN_SZ,
      WND_BTN_SZ * 2,
      wnd_sz.right - WND_BTN_SZ * 2,
      wnd_sz.bottom - WND_BTN_SZ * 3,
      TRUE
    );

    if( menu_txt ) {
      MoveWindow( menu_txt,
        ( WND_BTN_SZ * 6 ) + 6,
        WND_BTN_SZ + 4,
        ( wnd_sz.right - wnd_sz.left ) - ( ( WND_BTN_SZ * 6 ) + 10 ),
        WND_BTN_SZ - 9,
        TRUE
      );
    }

    wnd_type_line_count( hwnd, wnd_sz, true );
    wnd_type_caret_pos( hwnd, wnd_sz, true );

    if( !txt_box )
      break;

    vscroll.txt_rect = get_wnd_sz( txt_box );

    vscroll.bkrect = {
      vscroll.txt_rect.right + 1, vscroll.txt_rect.top - 1,
      vscroll.txt_rect.right + WND_BTN_SZ, vscroll.txt_rect.bottom + 1
    };
  
    vscroll.draw( true, true );
  } break;
  }
  return DefWindowProcA( hwnd, msg, wp, lp );
}