#include "wnd_fps.h"

u64 prev_time = GetTickCount64(), frames{}, fps{};
bool hide_fps = false;

void wnd_fps_calc() {
  if( hide_fps )
    return;

  u64 cur_time = GetTickCount64();
  u64 fin_time = cur_time - prev_time;

  frames++;

  if( fin_time < 64 )
    return;

  fps = ( frames / fin_time );
  frames = 0;
  prev_time = cur_time;
}

void wnd_fps_draw( const HWND hwnd, const RECT wnd_sz ) {
  wchar_t fps_txt[32];
  swprintf_s( fps_txt,
    sizeof( fps_txt ) / sizeof( wchar_t ),
    L"FPS : %llu\0", fps
  );

  HDC hdc = GetDC( hwnd );
  
  SIZE txt_sz;
  GetTextExtentPoint32W( hdc, fps_txt,
    (s32)wcslen( fps_txt ), &txt_sz
  );

  const RECT fps_rect {
    wnd_sz.right  - txt_sz.cx - 20,
    wnd_sz.bottom - txt_sz.cy -  5,
    wnd_sz.right - 5, wnd_sz.bottom - 5
  };

  if( caret_rect.right > fps_rect.left + 15 ) {
    hide_fps = true;
    ReleaseDC( hwnd, hdc );
    return;
  } else if( hide_fps )
    hide_fps = false;

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_M_GRY );
  TextOutW( hdc,
    wnd_sz.right  - txt_sz.cx - 5,
    wnd_sz.bottom - txt_sz.cy - 5,
    fps_txt, (s32)wcslen( fps_txt )
  );

  ReleaseDC( hwnd, hdc );
}