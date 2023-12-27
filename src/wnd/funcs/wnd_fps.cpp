#include "wnd_fps.h"

#define UPDATE_INTERVAL 64
u64 prev_time = GetTickCount64(), frames{}, fps{};
bool hide_fps = false,
     hide_fps_prev = true;

void wnd_fps_calc() {
  if( hide_fps || !menu_style_toggle[ FPS ] )
    return;

  u64 cur_time = GetTickCount64();
  u64 fin_time = cur_time - prev_time;

  frames++;

  if( fin_time < UPDATE_INTERVAL )
    return;

  fps = ( frames / fin_time );
  frames = 0;
  prev_time = cur_time;
}

void wnd_fps_draw( HWND hwnd ) {
  while( !hwnd ) {
    Sleep( 100 );
  }

  while( true && !hide_fps ) {
    if( !menu_style_toggle[ FPS ] ) {
      if( !hide_fps_prev ) {
        char fps_txt[32];
        sprintf_s( fps_txt,
          sizeof( fps_txt ) / sizeof( wchar_t ),
          "FPS : %llu\0", fps
        );

        RECT wnd_sz = get_wnd_sz( hwnd );

        HDC hdc = GetDC( hwnd );

        SIZE txt_sz;
        GetTextExtentPoint32A( hdc, fps_txt,
          (s32)strlen( fps_txt ), &txt_sz
        );

        const RECT fps_rect {
          wnd_sz.right  - txt_sz.cx - 20,
          wnd_sz.bottom - txt_sz.cy -  5,
          wnd_sz.right - 5,
          wnd_sz.bottom - 5
        };

        HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
        FillRect( hdc, &fps_rect, dbrush );

        hide_fps_prev = true;
      }

      Sleep( UPDATE_INTERVAL );
      continue;
    } else
      hide_fps_prev = false;

    char fps_txt[32];
    sprintf_s( fps_txt,
      sizeof( fps_txt ) / sizeof( wchar_t ),
      "FPS : %llu\0", fps
    );

    RECT wnd_sz = get_wnd_sz( hwnd );

    HDC hdc = GetDC( hwnd );

    SIZE txt_sz;
    GetTextExtentPoint32A( hdc, fps_txt,
      (s32)strlen( fps_txt ), &txt_sz
    );

    const RECT fps_rect {
      wnd_sz.right  - txt_sz.cx - 20,
      wnd_sz.bottom - txt_sz.cy -  5,
      wnd_sz.right - 5,
      wnd_sz.bottom - 5
    };

    if( caret_rect.right > fps_rect.left + 15 && menu_style_toggle[ CARET ] ) {
      hide_fps = true;
      Sleep( 1000 );
      continue;
    } else {
      hide_fps = false;

      HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
      FillRect( hdc, &fps_rect, dbrush );

      SetBkColor( hdc, COL_D_GRY );
      SetTextColor( hdc, COL_M_GRY );
      TextOutA( hdc,
        wnd_sz.right  - txt_sz.cx - 5,
        wnd_sz.bottom - txt_sz.cy - 5,
        fps_txt, (s32)strlen( fps_txt )
      );

      DeleteObject( dbrush );
    }

    ReleaseDC( hwnd, hdc );

    Sleep( UPDATE_INTERVAL );
  }
}