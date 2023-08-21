#pragma warning( disable : 26812 )
#include "wnd_resize.h"

bool user_resizing = false;
POINT ruser_start{};
s32 d_side{};

void wnd_resize_get_side( POINT m_pos, RECT wnd_sz ) {
  POINT pwn_sz = to_sz_point( wnd_sz );
  
  bool on_left   = ( m_pos.x <= pwn_sz.x * 0.01f ),
       on_top    = ( m_pos.y <= pwn_sz.y * 0.01f ),
       on_right  = ( m_pos.x >= pwn_sz.x * 0.99f ),
       on_bottom = ( m_pos.y >= pwn_sz.y * 0.99f ),
       in_hcenter =  ( !on_right && !on_left ),
       in_vcenter =  ( !on_bottom && !on_top );

  if( on_top )
    d_side = in_hcenter ? EDGE_TOP : ( on_left ? EDGE_TOP_LEFT : EDGE_TOP_RIGHT );
  else if( on_bottom )
    d_side = in_hcenter ? EDGE_BOTTOM : ( on_left ? EDGE_BOTTOM_LEFT : EDGE_BOTTOM_RIGHT );
  else if( on_left )
    d_side = in_vcenter ? EDGE_LEFT : EDGE_TOP_LEFT;
  else if( on_right )
    d_side = in_vcenter ? EDGE_RIGHT : EDGE_BOTTOM_RIGHT;
  else
    d_side = 0;
}

void wnd_resize_get_cursor( POINT m_pos, RECT wnd_sz ) {
  POINT pwn_sz = to_sz_point( wnd_sz );

  bool on_left   = ( m_pos.x <= pwn_sz.x * 0.01f ),
       on_top    = ( m_pos.y <= pwn_sz.y * 0.01f ),
       on_right  = ( m_pos.x >= pwn_sz.x * 0.99f ),
       on_bottom = ( m_pos.y >= pwn_sz.y * 0.99f ),
       in_hcenter =  ( !on_right && !on_left ),
       in_vcenter =  ( !on_bottom && !on_top );

  HCURSOR cur_nesw = LoadCursorW( 0, IDC_SIZENESW ),
          cur_nwse = LoadCursorW( 0, IDC_SIZENWSE ),
          cur_ns   = LoadCursorW( 0, IDC_SIZENS   ),
          cur_we   = LoadCursorW( 0, IDC_SIZEWE   ),
          cur_def  = LoadCursorW( 0, IDC_ARROW    );

  if( on_top )
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nwse : cur_nesw ) );
  else if( on_bottom )
    SetCursor( in_hcenter ? cur_ns : ( on_left ? cur_nesw : cur_nwse ) );
  else if( on_left )
    SetCursor( in_vcenter ? cur_we : cur_nesw );
  else if( on_right )
    SetCursor( in_vcenter ? cur_we : cur_nwse );
  else
    SetCursor( cur_def );
}

void wnd_resize_on( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  wnd_resize_get_side( m_pos, wnd_sz );
  if( d_side == 0 )
    return;

  user_resizing = true;
  ruser_start = m_pos;
  SetCapture( hwnd );
}

void wnd_resize_off() {
  if( !user_resizing )
    return;
  
  user_resizing = false;
  ReleaseCapture();
}

void wnd_resize_check_bounds( HWND hwnd, LPPOINT wnd_pos, LPPOINT wnd_sz, POINT m_delta ) {
  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  get_monitor_info( c_mon );

  if( wnd_pos->x < i_mon.rcWork.left ) {
    wnd_pos->x = i_mon.rcWork.left;
    wnd_sz->x += m_delta.x;
  }
  if( wnd_pos->y < i_mon.rcWork.top ) {
    wnd_pos->y = i_mon.rcWork.top;
    wnd_sz->y += m_delta.y;
  }
  if( wnd_pos->x + wnd_sz->x >= i_mon.rcWork.right )
    wnd_sz->x = i_mon.rcWork.right - wnd_pos->x;
  if( wnd_pos->y + wnd_sz->y >= i_mon.rcWork.bottom )
    wnd_sz->y = i_mon.rcWork.bottom - wnd_pos->y;

  if( wnd_sz->x < 200 ) {
    wnd_sz->x = 200;
    wnd_pos->x -= ( m_delta.x > 0 ) ? m_delta.x : 0;
  }
  if( wnd_sz->y < 200 ) {
    wnd_sz->y = 200;
    wnd_pos->y -= ( m_delta.y > 0 ) ? m_delta.y : 0;
  }
}

void wnd_resize( HWND hwnd, POINT m_pos, RECT wnd_sz ) {
  if( !d_side || !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  POINT m_delta { m_pos - ruser_start },
        wnd_pos = ( to_pos_point( wnd_sz ) -= 1 ),
        _wnd_sz = ( to_sz_point ( wnd_sz ) += 2 );

  ClientToScreen( hwnd, &wnd_pos );

  auto wnd_adj = [&]( POINT pos, POINT size ) {
    wnd_pos += pos;
    _wnd_sz += size;
  };  

  std::unordered_map<s32, std::function<void()>> edge_actions {
    { EDGE_TOP_LEFT    ,
      { [&](){ wnd_adj( m_delta, -m_delta ); } } },
    { EDGE_TOP         ,
      { [&](){ wnd_adj( { 0, m_delta.y }, { 0, -m_delta.y } ); } } },
    { EDGE_TOP_RIGHT   ,
      { [&](){ wnd_adj( { 0, m_delta.y }, { m_delta.x, -m_delta.y } ); ruser_start.x = m_pos.x; } } },
    { EDGE_RIGHT       ,
      { [&](){ wnd_adj( {},{ m_delta.x, 0 } ); ruser_start.x = m_pos.x; } } },
    { EDGE_BOTTOM_RIGHT,
      { [&](){ wnd_adj( {}, m_delta ); ruser_start = m_pos; } } },
    { EDGE_BOTTOM      ,
      { [&](){ wnd_adj( {}, { 0, m_delta.y } ); ruser_start.y = m_pos.y; } } },
    { EDGE_BOTTOM_LEFT ,
      { [&](){ wnd_adj( { m_delta.x, 0 }, { -m_delta.x, m_delta.y } ); ruser_start.y = m_pos.y; } } },
    { EDGE_LEFT        ,
      { [&](){ wnd_adj( { m_delta.x, 0 }, { -m_delta.x, 0 } ); } } }
  };

  auto it = edge_actions.find( d_side );
  if( it != edge_actions.end() )
    it->second();

  wnd_resize_check_bounds( hwnd, &wnd_pos, &_wnd_sz, m_delta );

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    _wnd_sz.x, _wnd_sz.y,
    SWP_NOZORDER
  );
}

void wnd_resize_title( HWND hwnd, bool mouse_over ) {
  if( !mouse_over )
    return;

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
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