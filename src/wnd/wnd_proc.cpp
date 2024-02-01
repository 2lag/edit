#include "wnd_proc.h"
#include "funcs/includes.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  POINT m_pos{};
  GetCursorPos( &m_pos );
  ScreenToClient( hwnd, &m_pos );

  vscroll.update();
  draw_line_count( hwnd );
  draw_caret_pos( hwnd );

  switch( msg ) {
  case WM_CREATE: {
    wnd_sz = get_wnd_sz( hwnd );
    wnd_type_create( hwnd );
  } break;
  case WM_CTLCOLOREDIT: {
    wnd_customize( wp );
    return (LRESULT)GetStockObject( 0 );
  } break;
  case WM_DESTROY: {
    return 0;
  } break;
  case WM_LBUTTONDBLCLK: {
    wnd_resize_title( hwnd, m_pos );
  } break;
  case WM_LBUTTONDOWN: {
    wnd_resize_on( hwnd, m_pos );
    drag_on( hwnd, m_pos );
    
    vscroll.drag_on( m_pos );

    wnd_title_cls( m_pos );
    wnd_title_max( m_pos );
    wnd_title_min( m_pos );
  } break;
  case WM_LBUTTONUP: {
    drag_resize( hwnd );

    drag_off();
    wnd_resize_off();

    vscroll.drag_off();
  } break;
  case WM_MOUSEMOVE: {
    drag( hwnd, m_pos );

    if( is_maxd ) break;

    wnd_resize_get_cursor( m_pos );
    wnd_resize( hwnd, m_pos );
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( hwnd, &ps );

    wnd_title_draw( hdc );
    wnd_type_outline( hwnd );
    wnd_menu_draw( hdc );

    EndPaint( hwnd, &ps );
  } break;
  case WM_SIZE: {
    wnd_sz = get_wnd_sz( hwnd );

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

    draw_line_count( hwnd, true );
    draw_caret_pos( hwnd, true );

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