#include "wnd/wnd_func.h"
#include "wnd/funcs/wnd_fps.h"

HWND h_global;

#define WND_BASE_SZ 420

s32 WINAPI WinMain( _In_     HINSTANCE inst    ,
                    _In_opt_ HINSTANCE         ,
                    _In_     LPSTR             ,
                    _In_     s32       cmdshow ) {
  WNDCLASSEX wnd_cls {
    sizeof( WNDCLASSEX ),
    CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    wnd_proc, 0, 0, inst, 0,
    LoadCursorW( 0, IDC_ARROW ),
    CreateSolidBrush( COL_D_GRY ),
    L"edit", L"edit_class", 0
  };

  RegisterClassExW( &wnd_cls );

  HMONITOR c_mon = MonitorFromPoint( {}, MONITOR_DEFAULTTOPRIMARY );
  get_monitor_info( c_mon );

  POINT wnd_pos = { ( i_mon.rcWork.right - i_mon.rcWork.left ) / 2 - ( WND_BASE_SZ / 2 ),
                    ( i_mon.rcWork.bottom - i_mon.rcWork.top ) / 2 - ( WND_BASE_SZ / 2 )
  };

  h_global = CreateWindowExW( 0,
    L"edit_class", L"edit",
    WS_POPUPWINDOW,
    wnd_pos.x,
    wnd_pos.y,
    WND_BASE_SZ,
    WND_BASE_SZ,
    0, 0, inst, 0
  );

  ShowWindow( h_global, cmdshow );
  UpdateWindow( h_global );

  std::thread draw_fps( wnd_fps_draw, std::ref( h_global ) );
  draw_fps.detach();

  run_debug_console();

  MSG msg;
  while( true ) {
    while( PeekMessageW( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
      TranslateMessage( &msg );
      DispatchMessageW( &msg );
    }

    wnd_fps_calc();

    if( msg.message == WM_QUIT )
      break;
  }

  UnhookWindowsHookEx( mouse_hook );

  return (s32)msg.wParam;
}