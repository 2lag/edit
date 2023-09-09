#pragma once
#define WIN32_LEAN_AND_MEAN
#include  <windows.h>
#include <windowsx.h>
#include   <dwmapi.h>
#pragma comment( lib, "dwmapi" )
#include <unordered_map>
#include <functional>

#include <commctrl.h>
#pragma comment( lib, "comctl32" )

#ifdef _DEBUG
#include <iostream>
#endif

#include "../typedef.h"

enum WND_COL {
  COL_D_GRY = RGB(  32,  32,  32 ),
  COL_M_GRY = RGB( 120, 120, 120 ),
  COL_L_GRY = RGB( 150, 150, 150 ),
  COL_D_RED = RGB(  64,  32,  32 ),
};

#include "funcs/util/util.h"

extern HWND txt_box;

#include "funcs/type/wnd_type_scroll.h"
extern CSCROLL vscroll;

extern u64 tps;
extern u64 prev_time;
extern bool  user_resizing;
extern POINT max_prev_pos;
extern RECT  max_prev_sz;
extern bool  is_maxd;

#include "funcs/util/overloads.h"

#define get_monitor_info( c_mon ) \
MONITORINFO i_mon; \
i_mon.cbSize = sizeof( i_mon ); \
GetMonitorInfoW( c_mon, &i_mon )