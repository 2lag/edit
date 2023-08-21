#include "wnd_drag.h"

bool user_dragging = false;
POINT duser_start{};

void wnd_drag_on( HWND hwnd, POINT m_pos, bool mouse_over ) {
  if( !mouse_over )
    return;

  if( !is_maxd ) {
    GetClientRect( hwnd, &max_prev_sz );
    max_prev_pos = to_pos_point( max_prev_sz );
    ClientToScreen( hwnd, &max_prev_pos );
  }

  user_dragging = true;
  duser_start = m_pos;
  SetCapture( hwnd );
} 

void wnd_drag_off() {
  if( !user_dragging )
    return;
  
  user_dragging = false;
  ReleaseCapture();
}

void wnd_drag( HWND hwnd, POINT m_pos ) {
  if( !user_dragging ) {
    duser_start = m_pos;
    return;
  }

  RECT r_wnd;
  GetWindowRect( hwnd, &r_wnd );

  HMONITOR c_mon = MonitorFromWindow( hwnd, MONITOR_DEFAULTTONEAREST );
  get_monitor_info( c_mon );


  POINT m_delta = m_pos - duser_start,
         wnd_sz = to_sz_point( r_wnd ),
           m_sz = to_sz_point( i_mon.rcWork ),
        wnd_pos = to_pos_point( r_wnd ) + m_delta;

  u32 swp_flags = SWP_NOSIZE | SWP_NOZORDER;

  bool is_quarthalfmax = ( wnd_sz   == m_sz     ) ||
                         ( wnd_sz.x == m_sz.x / 2 &&
                         ( wnd_sz.y == m_sz.y     ||
                           wnd_sz.y == m_sz.y / 2 ));
  if( is_quarthalfmax ) {
    swp_flags = SWP_NOZORDER;
    f32 wnd_xper = (f32)m_pos.x / (f32)wnd_sz.x;
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

void wnd_drag_resize( HWND hwnd, POINT m_pos ) {
  POINT sm_pos;
  GetCursorPos( &sm_pos );

  HMONITOR c_mon = MonitorFromPoint( sm_pos, MONITOR_DEFAULTTONEAREST );
  static HMONITOR pc_mon = nullptr;
  static RECT pm_rect{};
  static POINT monitor_offset{};

  if( c_mon != pc_mon ) {
    get_monitor_info( c_mon );
    if( !EqualRect( &i_mon.rcWork, &pm_rect ) ) {
      pm_rect = i_mon.rcWork;
      pc_mon = c_mon;
      monitor_offset = to_pos_point( i_mon.rcWork );
    }
  }

  get_monitor_info( pc_mon );
  POINT nwnd_sz{},
       nwnd_pos{},
         mon_sz = to_sz_point( i_mon.rcWork ),
        pmon_sz = to_sz_point( pm_rect ),
     sm_pos_adj {
       sm_pos.x < 0 ? sm_pos.x + pmon_sz.x : sm_pos.x,
       sm_pos.y - monitor_offset.y
  };

  if( sm_pos.x > pmon_sz.x )
    sm_pos_adj.x = sm_pos.x - ( pm_rect.right - mon_sz.x );

  // lord forgive me
  bool m_in_lxr = sm_pos_adj.x < mon_sz.x * 0.1f,
       m_in_rxr = sm_pos_adj.x > mon_sz.x * 0.9f,
       m_in_tyr = sm_pos_adj.y < mon_sz.y * 0.1f,
       m_in_byr = sm_pos_adj.y > mon_sz.y * 0.9f,
       within_max_range = sm_pos_adj.y <= i_mon.rcWork.top && !m_in_lxr && !m_in_rxr,
     within_lhalf_range = sm_pos_adj.x <= mon_sz.x * 0.01f && !m_in_tyr && !m_in_byr,
     within_rhalf_range = sm_pos_adj.x >= mon_sz.x * 0.99f && !m_in_tyr && !m_in_byr,
   within_lcorner_range = ( m_in_tyr || m_in_byr ) && m_in_lxr,
   within_rcorner_range = ( m_in_tyr || m_in_byr ) && m_in_rxr,
   within_tcorner_range = ( m_in_lxr || m_in_rxr ) && m_in_tyr,
   within_bcorner_range = ( m_in_lxr || m_in_rxr ) && m_in_byr,
           within_range =  within_max_range     ||
                           within_lhalf_range   ||
                           within_rhalf_range   ||
                           within_lcorner_range ||
                           within_tcorner_range ||
                           within_rcorner_range ||
                           within_bcorner_range;

  if( !within_range )
    return;

  is_maxd = true;
  GetClientRect( hwnd, &max_prev_sz );

  if( within_max_range ) {
    nwnd_pos = to_pos_point( i_mon.rcWork ),
    nwnd_sz  = mon_sz;
  } else if ( within_lhalf_range || within_rhalf_range ) {
    nwnd_pos = {
      within_lhalf_range ?
        i_mon.rcWork.left : i_mon.rcWork.right - mon_sz.x / 2,
      i_mon.rcWork.top
    },
    nwnd_sz = mon_sz;
    nwnd_sz.x /= 2;
  } else if( within_tcorner_range ) {
    nwnd_pos = {
      within_lcorner_range ?
        i_mon.rcWork.left : i_mon.rcWork.right - mon_sz.x / 2,
      i_mon.rcWork.top
    };
    nwnd_sz = mon_sz / 2;
  } else if( within_bcorner_range ) {
    nwnd_pos = {
      within_lcorner_range ?
        i_mon.rcWork.left : i_mon.rcWork.right - mon_sz.x / 2,
      mon_sz.y / 2
    };
    nwnd_sz = mon_sz / 2;
  }

  SetWindowPos( hwnd, 0,
    nwnd_pos.x, nwnd_pos.y,
    nwnd_sz.x , nwnd_sz.y ,
    SWP_NOZORDER
  );
}