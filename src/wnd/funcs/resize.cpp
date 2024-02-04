#include "resize.h"

#define RESIZE_THRESHOLD 5
#define MIN_WINDOW_SIZE 256

POINT ruser_start{};
s32 d_side{};

void resize_get_side( const POINT m_pos ) {
  POINT pwn_sz = get_size( wnd_sz );
  
  bool on_left   = ( m_pos.x <= RESIZE_THRESHOLD ),
       on_top    = ( m_pos.y <= RESIZE_THRESHOLD ),
       on_right  = ( m_pos.x >= pwn_sz.x - RESIZE_THRESHOLD ),
       on_bottom = ( m_pos.y >= pwn_sz.y - RESIZE_THRESHOLD ),
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

void resize_get_cursor( const POINT m_pos ) {
  POINT pwn_sz = get_size( wnd_sz );
  
  bool on_left   = ( m_pos.x <= RESIZE_THRESHOLD ),
       on_top    = ( m_pos.y <= RESIZE_THRESHOLD ),
       on_right  = ( m_pos.x >= pwn_sz.x - RESIZE_THRESHOLD ),
       on_bottom = ( m_pos.y >= pwn_sz.y - RESIZE_THRESHOLD ),
       in_hcenter =  ( !on_right && !on_left ),
       in_vcenter =  ( !on_bottom && !on_top );

  static const HCURSOR cur_nesw = LoadCursorA( 0, IDC_SIZENESW ),
                       cur_nwse = LoadCursorA( 0, IDC_SIZENWSE ),
                       cur_ns   = LoadCursorA( 0, IDC_SIZENS   ),
                       cur_we   = LoadCursorA( 0, IDC_SIZEWE   ),
                       cur_def  = LoadCursorA( 0, IDC_ARROW    );

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

void resize_on( const POINT m_pos ) {
  resize_get_side( m_pos );
  if( !d_side )
    return;

  user_resizing = true;
  ruser_start = m_pos;
  SetCapture( h_global );
}

void resize_off() {
  if( !user_resizing )
    return;

  user_resizing = false;
  ReleaseCapture();
}

void resize_check_bounds( const HWND hwnd,
                             LPPOINT wnd_pos,
                         const POINT m_delta ) {
  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  get_monitor_info( c_mon, i_mon );

  if( wnd_pos->x < i_mon.rcWork.left ) {
    wnd_pos->x = i_mon.rcWork.left;
    wnd_sz.right += m_delta.x;
  }
  if( wnd_pos->y < i_mon.rcWork.top ) {
    wnd_pos->y = i_mon.rcWork.top;
    wnd_sz.bottom += m_delta.y;
  }

  if( wnd_pos->x + wnd_sz.right >= i_mon.rcWork.right )
    wnd_sz.right = i_mon.rcWork.right - wnd_pos->x;
  if( wnd_pos->y + wnd_sz.bottom >= i_mon.rcWork.bottom )
    wnd_sz.bottom = i_mon.rcWork.bottom - wnd_pos->y;

  if( wnd_sz.right < MIN_WINDOW_SIZE ) {
    wnd_sz.right = MIN_WINDOW_SIZE;
    wnd_pos->x -= ( m_delta.x > 0 ) ? m_delta.x : 0;
  }
  if( wnd_sz.bottom < MIN_WINDOW_SIZE ) {
    wnd_sz.bottom = MIN_WINDOW_SIZE;
    wnd_pos->y -= ( m_delta.y > 0 ) ? m_delta.y : 0;
  }
}

void adj_wnd_pos_sz( POINT& wnd_pos,
               const POINT  pos_adj,
               const POINT  sz_adj ) {
  wnd_pos += pos_adj;
  wnd_sz = {
    0, 0,
    wnd_sz.right += sz_adj.x,
    wnd_sz.bottom += sz_adj.y
  };
}

void resize( const POINT m_pos ) {
  if( !d_side || !user_resizing ) {
    ruser_start = m_pos;
    return;
  }

  POINT m_delta = ( m_pos - ruser_start ),
        wnd_pos = ( get_position( wnd_sz ) -= 1 );
  
  ClientToScreen( h_global, &wnd_pos );

  // and finally, sin #3
  std::unordered_map<s32, std::pair<POINT, POINT>> adjustments {
    { EDGE_TOP_LEFT    , { {            m_delta   }, {             -m_delta   } } },
    { EDGE_TOP         , { {         0, m_delta.y }, {          0, -m_delta.y } } },
    { EDGE_TOP_RIGHT   , { {         0, m_delta.y }, {  m_delta.x, -m_delta.y } } },
    { EDGE_RIGHT       , { {         0,         0 }, {  m_delta.x,          0 } } },
    { EDGE_BOTTOM_RIGHT, { {         0,         0 }, {              m_delta   } } },
    { EDGE_BOTTOM      , { {         0,         0 }, {          0,  m_delta.y } } },
    { EDGE_BOTTOM_LEFT , { { m_delta.x,         0 }, { -m_delta.x,  m_delta.y } } },
    { EDGE_LEFT        , { { m_delta.x,         0 }, { -m_delta.x,          0 } } }
  };

  auto adjustment = adjustments.find( d_side );
  if( adjustment != adjustments.end() ) {
    adj_wnd_pos_sz( wnd_pos,
      adjustment->second.first,
      adjustment->second.second
    );
  }

  switch( d_side ) {
  case EDGE_TOP_RIGHT:
  case EDGE_RIGHT:
    ruser_start.x = m_pos.x;
    break;
  case EDGE_BOTTOM_LEFT:
  case EDGE_BOTTOM:
    ruser_start.y = m_pos.y;
    break;
  case EDGE_BOTTOM_RIGHT:
    ruser_start = m_pos;
    break;
  }

  resize_check_bounds(
    h_global,
    &wnd_pos,
    m_delta
  );

  SetWindowPos(
    h_global, 0,
    wnd_pos.x,
    wnd_pos.y,
    wnd_sz.right + 2,
    wnd_sz.bottom + 2,
    SWP_NOZORDER
  );
}

void resize_title( const POINT m_pos ) {
  RECT item_rect {
    WND_BTN_SZ / 5 + 1,
    WND_BTN_SZ / 5 + 1,
    wnd_sz.right - WND_BTN_SZ * 3,
    WND_BTN_SZ
  };

  if( !PtInRect( &item_rect, m_pos ) )
    return;

  HMONITOR c_mon = MonitorFromWindow( h_global, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  get_monitor_info( c_mon, i_mon );

  POINT mon_sz = get_size( i_mon.rcWork ),
       nwnd_ps{},
       nwnd_sz{};

  if( !is_maxd ) {
    GetClientRect( h_global, &max_prev_sz );
    max_prev_pos = get_position( max_prev_sz );
    ClientToScreen( h_global, &max_prev_pos );

    nwnd_ps = get_position( i_mon.rcWork ),
    nwnd_sz = mon_sz;
  } else {
    POINT pwnd_sz = get_size( max_prev_sz );

    if( !max_prev_pos )
      max_prev_pos = ( mon_sz - pwnd_sz ) / 2;

    nwnd_ps = max_prev_pos,
    nwnd_sz = pwnd_sz;
  }
  is_maxd = !is_maxd;

  SetWindowPos( h_global, 0,
    nwnd_ps.x, nwnd_ps.y,
    nwnd_sz.x, nwnd_sz.y,
    SWP_NOZORDER
  );
}