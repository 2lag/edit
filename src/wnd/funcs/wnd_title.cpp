#include "wnd_title.h"

#define OBJ_COUNT 8

void wnd_title_draw( const HDC hdc, const RECT wnd_sz ) {
  WND_OBJ obj[ OBJ_COUNT ] = {
    wnd_obj_create( {                                     0, 0,
                      wnd_sz.right                         , WND_BTN_SZ * 1 }, COL_M_GRY, false ),
    wnd_obj_create( {                                     0, 0,
                      wnd_sz.right                         , WND_BTN_SZ - 1 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 1 )    , 0,
                      wnd_sz.right                         , WND_BTN_SZ * 1 }, COL_M_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 1 ) + 1, 0,
                      wnd_sz.right                         , WND_BTN_SZ - 1 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 2 )    , 0,
                      wnd_sz.right - ( WND_BTN_SZ * 1 )    , WND_BTN_SZ * 1 }, COL_M_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 2 ) + 1, 0,
                      wnd_sz.right - ( WND_BTN_SZ * 1 )    , WND_BTN_SZ - 1 }, COL_D_GRY, false ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 3 )    , 0,
                      wnd_sz.right - ( WND_BTN_SZ * 2 )    , WND_BTN_SZ * 1 }, COL_M_GRY, true  ),
    wnd_obj_create( { wnd_sz.right - ( WND_BTN_SZ * 3 ) + 1, 0,
                      wnd_sz.right - ( WND_BTN_SZ * 2 )    , WND_BTN_SZ - 1 }, COL_D_GRY, false )};

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );

  for( s8 idx = 0; idx < OBJ_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }
    
    LPCWSTR txt = L"";
    switch( idx ) {
    case 3:
      txt = L"X";
      break;
    case 5:
      txt = L"O";
      break;
    case 7:
      txt = L"_";
      break;
    }

    SIZE txt_sz{};
    GetTextExtentPoint32W( hdc, txt, 1, &txt_sz );

    if( idx == 7 )
      txt_sz.cy += 7; // offset for looks

    TextOutW( hdc,
      obj[ idx ].r.left + ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 - ( txt_sz.cx / 2 ),
      obj[ idx ].r.top  + ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 - ( txt_sz.cy / 2 ),
      txt, 1
    );
  }

  LPCWSTR txt = L"Edit";
  TextOutW( hdc, 6, 4, txt, lstrlenW( txt ) );

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_title_cls( const bool mouse_over ) {
  if( mouse_over )
    ExitProcess( 0 );
}

RECT max_prev_sz;
POINT max_prev_pos;
bool is_maxd = false;
void wnd_title_max( const HWND hwnd, const bool mouse_over ) {
  if( !mouse_over )
    return;

  HMONITOR c_mon = MonitorFromWindow( hwnd,
    is_maxd ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST
  );
  get_monitor_info( c_mon );

  POINT mon_sz = to_sz_point( i_mon.rcWork ),
       nwnd_ps{},
       nwnd_sz{};

  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = to_pos_point( max_prev_sz );
    ClientToScreen( hwnd, &max_prev_pos );

    nwnd_ps = to_pos_point( i_mon.rcWork ),
    nwnd_sz = mon_sz;
  } else {
    POINT wnd_sz = to_sz_point( max_prev_sz );

    if( !max_prev_pos )
      max_prev_pos = ( mon_sz - wnd_sz ) / 2;

    nwnd_ps = max_prev_pos,
    nwnd_sz = wnd_sz;
  }
  is_maxd = !is_maxd;

  SetWindowPos( hwnd, 0,
    nwnd_ps.x, nwnd_ps.y,
    nwnd_sz.x, nwnd_sz.y,
    SWP_NOZORDER
  );
}

void wnd_title_min( const HWND hwnd, const bool mouse_over ) {
  if( mouse_over )
    ShowWindow( hwnd, SW_MINIMIZE );
}