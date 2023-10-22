#include "wnd/wnd_func.h"
#include "wnd/funcs/wnd_fps.h"

HWND h_global;

s32 WINAPI WinMain( _In_     HINSTANCE inst    ,
                    _In_opt_ HINSTANCE         ,
                    _In_     LPSTR             ,
                    _In_     s32       cmdshow ) {
  WNDCLASSEX wnd_cls {
    sizeof( WNDCLASSEX ),
    CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    wnd_proc, 0, 0, inst, 0,
    LoadCursorW( 0, IDC_ARROW ),
    CreateSolidBrush( RGB( 32, 32, 32 ) ),
    L"edit", L"edit_class", 0
  };

  RegisterClassExW( &wnd_cls );

  HWND hwnd = h_global = CreateWindowExW( 0,
    L"edit_class", L"edit",
    WS_POPUPWINDOW,
    100, 100,
    420, 420,
    0, 0, inst, 0
  );

  ShowWindow( hwnd, cmdshow );
  UpdateWindow( hwnd );

  std::thread draw_fps( wnd_fps_draw, std::ref( hwnd ) );
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