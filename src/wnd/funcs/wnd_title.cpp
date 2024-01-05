#include "wnd_title.h"

#include "./hooks/wnd_hooks.h"

// never. again.
using namespace std;

// sin #2 absolved 1.5.24
void wnd_title_draw( const HDC hdc, const RECT wnd_sz ) {
  HBRUSH bg_brush = CreateSolidBrush( COL_M_GRY );
  HBRUSH brush = CreateSolidBrush( COL_D_GRY );
  const RECT title_rect = {
    -1, -1, wnd_sz.right + 1, WND_BTN_SZ
  };

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );

  vector<const char*> txt{ "Edit", "X", "O", "_" };
  vector<SIZE> txt_sz( txt.size() );
  for( s8 idx = 0; idx < txt.size(); ++idx ) {

    RECT item_rect {
      wnd_sz.right - ( idx * WND_BTN_SZ ),
      -1,
      wnd_sz.right - ( ( idx - 1 ) * WND_BTN_SZ ) + 1,
      WND_BTN_SZ
    };

    if( !idx ) item_rect = title_rect;

    RECT inner_item_rect = item_rect - 1;

    FillRect( hdc, &item_rect, bg_brush );
    FillRect( hdc, &inner_item_rect, brush );


    GetTextExtentPoint32A( hdc,
      txt[ idx ],
      static_cast<s32>( strlen( txt[ idx ] ) ),
      &txt_sz[ idx ]
    );

    TextOutA( hdc,
      ( !idx ) ? 6 : hcenter_text( inner_item_rect, txt_sz[ idx ] ),
      ( idx  == 3 ) ? 0 : vcenter_text( inner_item_rect, txt_sz[ idx ] ),
      txt[ idx ], static_cast<s32>( strlen( txt[ idx ] ) )
    );
  }

  DeleteObject( brush );
  DeleteObject( bg_brush );
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

  POINT nwnd_ps{},
        nwnd_sz{};
  
  if( !is_maxd ) {
    GetClientRect( h_global, &max_prev_sz );
    max_prev_pos = get_position( max_prev_sz );
    ClientToScreen( h_global, &max_prev_pos );

    nwnd_ps = get_position( i_mon.rcWork ),
    nwnd_sz = get_size( i_mon.rcWork );
  } else {
    if( !max_prev_pos )
      max_prev_pos = ( get_size( i_mon.rcWork ) - get_size( max_prev_sz ) ) / 2;

    nwnd_ps = max_prev_pos,
    nwnd_sz = get_size( max_prev_sz );
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