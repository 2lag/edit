#pragma once
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

struct WND_OBJ {
  RECT r;
  HBRUSH col;
  bool next;
};

struct VEC3 {
  s32 x, y, w;
};

inline WND_OBJ wnd_obj_create( RECT r, COLORREF col, bool next ) {
  return { r, CreateSolidBrush( col ), next };
}

#include "funcs/util/util.h"
#include "funcs/util/overloads.h"

extern HWND h_global;
extern HWND txt_box;
extern HWND menu_txt;

#define OBJ_BASE_COUNT 6
extern bool m_base_open[ OBJ_BASE_COUNT / 2 ];

#include "funcs/edit/wnd_edit_scroll.h"
extern CSCROLL vscroll;

extern HHOOK mouse_hook;
extern HHOOK key_hook;

extern RECT caret_rect;

extern bool  user_resizing;
extern POINT max_prev_pos;
extern RECT  max_prev_sz;
extern bool  is_maxd;

inline void run_debug_console() {
#ifdef _DEBUG
  AllocConsole();
  FILE* new_std;
  freopen_s( &new_std, "CONOUT$", "w", stdout );
  SetConsoleTitleA( "edit debug" );
  void *std_handle = GetStdHandle( STD_OUTPUT_HANDLE );
  const SMALL_RECT wnd_debug{ 00, 00, 42, 16 };
  SetConsoleWindowInfo( std_handle, true, &wnd_debug );
  SetConsoleScreenBufferSize( std_handle, { 43, 17 } );
  SetConsoleWindowInfo( std_handle, true, &wnd_debug );
#endif
}

#define get_monitor_info( c_mon ) \
MONITORINFO i_mon; \
i_mon.cbSize = sizeof( i_mon ); \
GetMonitorInfoA( c_mon, &i_mon )