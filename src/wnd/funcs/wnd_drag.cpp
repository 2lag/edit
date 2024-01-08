#include "wnd_drag.h"

bool user_dragging = false;
POINT duser_start{};

void wnd_drag_on( const HWND hwnd, const POINT m_pos ) {
  RECT item_rect {
    WND_BTN_SZ / 5 + 1,
    WND_BTN_SZ / 5 + 1,
    wnd_sz.right - WND_BTN_SZ * 3,
    WND_BTN_SZ
  };

  if( !PtInRect( &item_rect, m_pos ) )
    return;

  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = get_position( max_prev_sz );
    ClientToScreen( hwnd, &max_prev_pos );
  }

  user_dragging = true;
  duser_start = m_pos;
  SetCapture( hwnd );
} 

void wnd_drag_off() {
  if( user_dragging ) {
    user_dragging = false;
    ReleaseCapture();
  }
}

void wnd_drag( const HWND hwnd, const POINT m_pos ) {
  if( !user_dragging ) {
    duser_start = m_pos;
    return;
  }
  
  RECT r_wnd;
  GetWindowRect( hwnd, &r_wnd );

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  get_monitor_info( c_mon, i_mon );


  POINT m_delta = m_pos - duser_start,
        pwnd_sz = get_size( r_wnd ),
           m_sz = get_size( i_mon.rcWork ),
        wnd_pos = get_position( r_wnd ) + m_delta;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;

  bool is_quarthalfmax = pwnd_sz   == m_sz ||
                         pwnd_sz.x == m_sz.x / 2 ||
                         pwnd_sz.y == m_sz.y / 2;

  if( is_quarthalfmax ) {
    swp_flags = SWP_NOZORDER;
    f32 wnd_xper = (f32)m_pos.x / (f32)get_size( wnd_sz ).x;
    salt nwnd_szx = max_prev_sz.right - max_prev_sz.left;
    wnd_pos.x = duser_start.x = (salt)( (f32)nwnd_szx * wnd_xper );
    is_maxd = false;
  }

  SetWindowPos( hwnd, 0,
    wnd_pos.x, wnd_pos.y,
    max_prev_sz.right - max_prev_sz.left,
    max_prev_sz.bottom - max_prev_sz.top,
    swp_flags
  );
}

void wnd_drag_resize( const HWND hwnd ) {
  if( user_resizing )
    return;

  POINT sm_pos;
  GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  MONITORINFO i_mon;
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};
  static POINT monitor_offset{};

  if( c_mon != pc_mon ) {
    get_monitor_info( c_mon, i_mon );
    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;
      monitor_offset = get_position( i_mon.rcWork );
    }
  }

  get_monitor_info( pc_mon, i_mon );
  POINT nwnd_sz{},
       nwnd_pos{},
         mon_sz = get_size( i_mon.rcWork ),
        pmon_sz = get_size( pm_rect ),
     sm_pos_adj {
       sm_pos.x < 0 ? sm_pos.x + pmon_sz.x : sm_pos.x,
       sm_pos.y - monitor_offset.y
  };

  if( sm_pos.x > pmon_sz.x )
    sm_pos_adj.x = sm_pos.x - ( pm_rect.right - mon_sz.x );

  // lord forgive me ( partly forgiven as of 1.1.24 )
  bool m_in_lxr = sm_pos_adj.x <= mon_sz.x * 0.1f,
       m_in_rxr = sm_pos_adj.x >= mon_sz.x * 0.9f,
       m_in_tyr = sm_pos_adj.y <= mon_sz.y * 0.1f,
       m_in_byr = sm_pos_adj.y >= mon_sz.y * 0.9f,
       within_max_range = m_in_tyr && !m_in_lxr && !m_in_rxr,
     within_lhalf_range = m_in_lxr && !m_in_tyr && !m_in_byr,
     within_rhalf_range = m_in_rxr && !m_in_tyr && !m_in_byr,
   within_lcorner_range = ( m_in_tyr || m_in_byr ) && m_in_lxr,
   within_tcorner_range = ( m_in_lxr || m_in_rxr ) && m_in_tyr,
   within_bcorner_range = ( m_in_lxr || m_in_rxr ) && m_in_byr;

  if( within_max_range ) { // fullscreen
    nwnd_pos = get_position( i_mon.rcWork ),
    nwnd_sz  = mon_sz;
  } else if( within_lhalf_range || within_rhalf_range ) { // half screen
    nwnd_pos = {
      within_lhalf_range ?
        i_mon.rcWork.left : i_mon.rcWork.right - mon_sz.x / 2,
      i_mon.rcWork.top
    },
    nwnd_sz = mon_sz;
    nwnd_sz.x /= 2;
  } else if( within_tcorner_range || within_bcorner_range ) {
    nwnd_pos = {
      within_lcorner_range ?
        i_mon.rcWork.left : i_mon.rcWork.right - mon_sz.x / 2,
      within_tcorner_range ?
        i_mon.rcWork.top : mon_sz.y / 2 + monitor_offset.y
    };
    nwnd_sz = mon_sz / 2;
  } else
    return;

  is_maxd = true;

  SetWindowPos( hwnd, 0,
    nwnd_pos.x, nwnd_pos.y,
    nwnd_sz.x , nwnd_sz.y ,
    SWP_NOZORDER
  );
}