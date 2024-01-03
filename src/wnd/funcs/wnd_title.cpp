#include "wnd_title.h"

#include "./hooks/wnd_hooks.h"

#define OBJ_COUNT 8

void wnd_title_draw( const HDC hdc, const RECT wnd_sz ) {
  WND_OBJ obj[ OBJ_COUNT ] = { // i've got to figure out a better way for this ( vector ? ). sin #2...
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
    
    LPCSTR txt = "";
    switch( idx ) {
    case 3: txt = "X"; break;
    case 5: txt = "O"; break;
    case 7: txt = "_"; break;
    }

    SIZE txt_sz{};
    GetTextExtentPoint32A( hdc, txt, 1, &txt_sz );

    if( idx == 7 )
      txt_sz.cy += 7; // underscore sits too low naturally

    TextOutA( hdc,
      obj[ idx ].r.left + ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 - ( txt_sz.cx / 2 ),
      obj[ idx ].r.top  + ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 - ( txt_sz.cy / 2 ),
      txt, 1
    );
  }

  LPCSTR txt = "Edit";
  TextOutA( hdc, 6, 4, txt, (s32)strlen( txt ) );

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_title_cls( const bool mouse_over ) {
  if( !mouse_over )
    return;

  wnd_unhook();
  ExitProcess( 0 );
}

void wnd_title_max( const bool mouse_over ) {
  if( !mouse_over )
    return;

  HMONITOR c_mon = MonitorFromWindow( h_global, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  get_monitor_info( c_mon, i_mon );

  POINT mon_sz = to_sz_point( i_mon.rcWork ),
       nwnd_ps{},
       nwnd_sz{};
  
  if( !is_maxd ) {
    GetClientRect( h_global, &max_prev_sz );
    max_prev_pos = to_pos_point( max_prev_sz );
    ClientToScreen( h_global, &max_prev_pos );

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

  SetWindowPos( h_global, 0,
    nwnd_ps.x, nwnd_ps.y,
    nwnd_sz.x, nwnd_sz.y,
    SWP_NOZORDER
  );
}

void wnd_title_min( const bool mouse_over ) {
  if( mouse_over ) ShowWindow( h_global, SW_MINIMIZE );
}