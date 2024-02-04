#pragma once

// this file disgusts me at this point

#define WIN32_LEAN_AND_MEAN
#include  <windows.h>
#include <windowsx.h>
#include   <dwmapi.h>
#pragma comment( lib, "dwmapi" )
#include <commctrl.h>
#pragma comment( lib, "comctl32" )

#include <unordered_map>
#include <functional>

#include <thread>

#include <vector>

#ifdef _DEBUG
#include <stdio.h>
#endif

#include "../typedef.h"

#define WND_BTN_SZ 25

enum WND_COL {
  COL_D_GRY = RGB(  32,  32,  32 ),
  COL_M_GRY = RGB( 120, 120, 120 ),
  COL_L_GRY = RGB( 150, 150, 150 ),
  COL_D_RED = RGB(  64,  32,  32 ),
};

struct VEC3 {
  s32 x, y, w;
};

#include "funcs/util/util.h"
#include "funcs/util/overloads.h"

extern HWND h_global;
extern HWND txt_box;
extern HWND menu_txt;

#define OBJ_BASE_COUNT 6
extern bool m_base_open[ OBJ_BASE_COUNT / 2 ];
extern bool menu_style_toggle[ OBJ_BASE_COUNT / 2 ];

extern bool macro_recording;
extern std::vector< std::pair< u8, u16 > > macro; // ew

enum MENU_STYLE {
  FPS = 0,
  CARET = 1,
  LINE_COUNT = 2
};

#include "funcs/edit/scroll.h"
extern CSCROLL vscroll;

extern HHOOK mouse_hook;
extern HHOOK key_hook;

extern RECT caret_rect;

extern bool  user_resizing;
extern POINT max_prev_pos;
extern RECT  max_prev_sz;
extern bool  is_maxd;

extern RECT wnd_sz;

inline void run_debug_console() {
  (void)AllocConsole();
  FILE* new_std;
  (void)freopen_s( &new_std, "CONOUT$", "w", stdout );
  (void)SetConsoleTitleA( "edit debug" );
  void *std_handle = GetStdHandle( STD_OUTPUT_HANDLE );
  const SMALL_RECT wnd_debug{ 00, 00, 42, 16 };
  (void)SetConsoleWindowInfo( std_handle, true, &wnd_debug );
  (void)SetConsoleScreenBufferSize( std_handle, { 43, 17 } );
  (void)SetConsoleWindowInfo( std_handle, true, &wnd_debug );
}

inline void get_monitor_info( HMONITOR c_mon, MONITORINFO &i_mon ) {
  i_mon.cbSize = sizeof( i_mon );
  (void)GetMonitorInfoA( c_mon, &i_mon );
}

#define hcenter_text( r_container, text_sz ) r_container.left + ( ( r_container.right - r_container.left ) / 2 ) - ( text_sz.cx / 2 )
#define vcenter_text( r_container, text_sz ) r_container.top  + ( ( r_container.bottom - r_container.top ) / 2 ) - ( text_sz.cy / 2 )