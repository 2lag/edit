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
  SetConsoleTitleW( L"type debug" );
  void *std_handle = GetStdHandle( STD_OUTPUT_HANDLE );
  const SMALL_RECT wnd_debug{ 00, 00, 42, 16 };
  SetConsoleWindowInfo( std_handle, true, &wnd_debug );
  SetConsoleScreenBufferSize( std_handle, { 43, 17 } );
  SetConsoleWindowInfo( std_handle, true, &wnd_debug );
#endif

  switch( msg ) {
  case WM_COMMAND: {
    switch( HIWORD( wp ) ) {
    case EN_ERRSPACE: {
      MessageBoxW( (HWND)lp,
        L"error", L"EN_ERRSPACE triggered",
        MB_OK | MB_ICONERROR
      );
    } break;
    case EN_VSCROLL: {
      vscroll.cscroll_setinfo( true, true );
    } break;
    }
  } break;
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
    
    vscroll.cscroll_drag_on( m_pos );

    wnd_title_cls( hwnd, PtInRect( &cls, m_pos ) );
    wnd_title_max( hwnd, PtInRect( &max, m_pos ) );
    wnd_title_min( hwnd, PtInRect( &min, m_pos ) );
  } break;
  case WM_LBUTTONUP: {
    if( !user_resizing )
      wnd_drag_resize( hwnd, m_pos );

    wnd_drag_off();
    wnd_resize_off();

    vscroll.cscroll_drag_off();
  } break;
  case WM_MOUSEMOVE: {
    wnd_drag( hwnd, m_pos );

    vscroll.cscroll_drag( m_pos );
    if( !vscroll.dragging )
      vscroll.cscroll_ishovered( m_pos );

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
      pwnd_sz.x - 50,
      pwnd_sz.y - 75,
      TRUE
    );
  } break;
  }
  return DefWindowProcW( hwnd, msg, wp, lp );
}