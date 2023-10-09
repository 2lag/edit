#include "wnd_fps.h"

u64 fps{};

void wnd_fps_calc() {
  static u64 frames{}, prev_time = GetTickCount64();
  u64 fin_time = GetTickCount64() - prev_time;

  frames++;

  if( fin_time < 10 )
    return;

  fps = ( frames / fin_time );
  frames = 0;
  prev_time = GetTickCount64();
}

void wnd_fps_draw( const HWND hwnd ) {
  const RECT wnd_sz = get_wnd_sz( hwnd );

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

  if( caret_info_pos.right > wnd_sz.right - txt_sz.cx - 5 ) {
    ReleaseDC( hwnd, hdc );
    return;
  }

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_M_GRY );
  TextOutW( hdc,
    wnd_sz.right  - txt_sz.cx - 5,
    wnd_sz.bottom - txt_sz.cy - 5,
    fps_txt, (s32)wcslen( fps_txt )
  );

  ReleaseDC( hwnd, hdc );
}