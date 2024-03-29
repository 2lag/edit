#include "wnd/wnd_proc.h"
#include "wnd/funcs/fps.h"
#include "wnd/funcs/util/config.h"

RECT wnd_sz;

HWND h_global;

RECT max_prev_sz;
POINT max_prev_pos;

bool is_maxd = false;

#define WND_BASE_SZ 420

s32 WINAPI WinMain( _In_     HINSTANCE inst    ,
                    _In_opt_ HINSTANCE         ,
                    _In_     LPSTR             ,
                    _In_     s32       cmdshow ) {
  WNDCLASSEX wnd_cls {
    sizeof( WNDCLASSEX ),
    CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    wnd_proc, 0, 0, inst, 0,
    LoadCursorA( 0, IDC_ARROW ),
    CreateSolidBrush( COL_D_GRY ),
    "edit", "edit_class", 0
  };

  (void)RegisterClassExA( &wnd_cls );

  HMONITOR c_mon = MonitorFromPoint( {}, MONITOR_DEFAULTTOPRIMARY );
  MONITORINFO i_mon;
  get_monitor_info( c_mon, i_mon );

  POINT wnd_pos = max_prev_pos = {
    ( i_mon.rcWork.right - i_mon.rcWork.left - WND_BASE_SZ ) / 2,
    ( i_mon.rcWork.bottom - i_mon.rcWork.top - WND_BASE_SZ ) / 2
  };
  max_prev_sz = i_mon.rcWork;

  h_global = CreateWindowExA( 0,
    "edit_class", "edit",
    WS_POPUPWINDOW,
    wnd_pos.x  , wnd_pos.y  ,
    WND_BASE_SZ, WND_BASE_SZ,
    0, 0, inst, 0
  );

  (void)ShowWindow( h_global, cmdshow );
  (void)UpdateWindow( h_global );
  
  std::thread fps(
    draw_fps,
    std::ref( h_global )
  );
  fps.detach();

#ifdef _DEBUG
  run_debug_console();
#endif

  init_config();

  MSG msg;
  while( true ) {
    while( PeekMessageA( &msg, nullptr, 0, 0, PM_REMOVE ) ) {
      (void)TranslateMessage( &msg );
      (void)DispatchMessageA( &msg );
    }

    calc_fps();

    if( msg.message == WM_QUIT )
      break;
  }

  return (s32)msg.wParam;
}