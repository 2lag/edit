#include "wnd_func.h"
#include "funcs/wnd_includes.h"

LRESULT wnd_proc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp ) {
  RECT wnd_sz;
  GetClientRect( hwnd, &wnd_sz );
  POINT pwnd_sz = to_sz_point( wnd_sz ),
        m_pos{};
  GetCursorPos( &m_pos );
  ScreenToClient( hwnd, &m_pos );

  RECT drag { 0, 6, wnd_sz.right - 75, 25 },
       cls { wnd_sz.right - 25, 05,
             wnd_sz.right - 05, 25
  },   max { wnd_sz.right - 50, 05,
             wnd_sz.right - 25, 25
  },   min { wnd_sz.right - 75, 5,
             wnd_sz.right - 50, 25
  };

  wnd_tps_draw( hwnd, wnd_sz );


#ifdef _DEBUG
  AllocConsole();
  FILE* new_std;
  freopen_s( &new_std, "CONOUT$", "w", stdout );
#endif

  switch( msg ) {
  case WM_CREATE: {
    wnd_type_create( hwnd, pwnd_sz );
    SetFocus( txt_box );
  } break;
  case WM_CTLCOLOREDIT: {
    wnd_type_customize( wp );
    return (LRESULT)GetStockObject( 0 );
  } break;
  case WM_DESTROY: {
    PostQuitMessage( 0 );
    return 0;
  } break;
  case WM_LBUTTONDBLCLK: {
    wnd_resize_title( hwnd, PtInRect( &drag, m_pos ) );
  } break;
  case WM_LBUTTONDOWN: {
    wnd_resize_on( hwnd, m_pos, wnd_sz );
    wnd_drag_on( hwnd, m_pos, PtInRect( &drag, m_pos ) );
    wnd_title_cls( hwnd, PtInRect( &cls, m_pos ) );
    wnd_title_max( hwnd, PtInRect( &max, m_pos ) );
    wnd_title_min( hwnd, PtInRect( &min, m_pos ) );
  } break;
  case WM_LBUTTONUP: {
    if( !user_resizing )
      wnd_drag_resize( hwnd, m_pos );
    wnd_drag_off();
    wnd_resize_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );
    if( is_maxd )
      break;
    wnd_resize_get_cursor( m_pos, wnd_sz );
    wnd_resize( hwnd, m_pos, wnd_sz );
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint( hwnd, &ps );

    wnd_title_draw( hdc, m_pos, wnd_sz );
    wnd_type_outline( hwnd, pwnd_sz );

    EndPaint( hwnd, &ps );
  } break;
  case WM_SIZE: {
    MoveWindow( txt_box,
      25, 50,
      pwnd_sz.x - 25,
      pwnd_sz.y - 75,
      TRUE
    );

    MoveWindow( vscroll,
      pwnd_sz.x - 40, -16,
      15, pwnd_sz.y - 42,
      TRUE
    );
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}