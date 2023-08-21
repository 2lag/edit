#include "wnd_tps.h"

void wnd_tps_calc() {
  static u64 frames{};
  u64 fin_time = GetTickCount64() - prev_time;

  frames++;

  if( fin_time >= 1000 ) {
    tps = ( frames / fin_time );
    frames = 0;
    prev_time = GetTickCount64();
  }
}

u64 prev_time = GetTickCount64(), tps{};
void wnd_tps_draw( HWND hwnd, RECT wnd_sz ) {
  wchar_t tps_txt[ 32 ];
  swprintf_s( tps_txt,
    sizeof( tps_txt ) / sizeof(tps_txt[0]),
    L"TPS : %llu\0", tps
  );

  HDC hdc = GetDC( hwnd );
  
  SIZE txt_sz;
  GetTextExtentPoint32W( hdc,
    tps_txt, (s32)wcslen( tps_txt ),
    &txt_sz
  );

  SetBkColor( hdc, COL_D_GRY );
  SetTextColor( hdc, COL_L_GRY );
  TextOutW( hdc,
    wnd_sz.right  - txt_sz.cx - 5,
    wnd_sz.bottom - txt_sz.cy - 5,
    tps_txt, (s32)wcslen( tps_txt )
  );

  ReleaseDC( hwnd, hdc );
}