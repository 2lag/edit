#pragma once

class CSCROLL {
public:
  s32 curr_line;
  s32 line_count;
  s32 line_first;
  s32 line_last;
  s32 lines_vis;
  SIZE line_sz;
  s32 scroll_h;
  s32 scroll_y;
public:
  HWND parent;
  RECT bkrect;
  RECT rect;
  bool hovered;
  bool dragging;
  POINT duser_start;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;

    RECT r = get_wnd_sz( parent );
    bkrect = rect = {
      r.right + 1,
      -1,
      r.right + 25,
      r.bottom + 1
    };
  }
  void cscroll_draw( bool update_info = true, bool redraw = false ) {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush(
             ( hovered ) ? COL_L_GRY : COL_M_GRY
    );

    GetTextExtentPoint32W( hdc, L"A", 1, &line_sz );
    if( update_info )
      cscroll_setinfo( redraw );

    FillRect( hdc, &bkrect, dbrush );
    FillRect( hdc, &rect  , lbrush );
    
    DeleteObject( dbrush );
    DeleteObject( lbrush );
    ReleaseDC( parent, hdc );
  }
public:
  void cscroll_drag_off() {
    if( !dragging )
      return;

    dragging = false;
  }
  void cscroll_drag_on( POINT m_pos ) {
    if( !cscroll_ishovered( m_pos ) )
      return;

    dragging = true;
    duser_start = m_pos;
  }
  void cscroll_drag( POINT m_pos ) {
    if( !GetAsyncKeyState( MK_LBUTTON ) )
      dragging = false;

    if( !dragging ) {
      duser_start = m_pos;
      return;
    }

    // fix this math by debugging with _DEBUG couts
    s32 m_delta = m_pos.y - duser_start.y;
    s32 delta_lines = m_delta / line_sz.cy;

    // verify this
    if( curr_line + delta_lines > line_count )
      delta_lines = 0;
    else if( curr_line + delta_lines < 1 )
      delta_lines = 0;
    
    RECT r_parent = get_wnd_sz( parent );

    // swap for em_scroll?
    ScrollWindowEx( parent,
      0, -delta_lines,
      0, &r_parent, 0, 0,
      SW_ERASE | SW_INVALIDATE
    );

    scroll_y += m_delta;

#ifdef _DEBUG
    std::cout << "current line   : " << curr_line << std::endl;
    std::cout << "line count     : " << line_count << std::endl;
    std::cout << "line first vis : " << line_first << std::endl;
    std::cout << "line last vis  : " << line_last << std::endl;
    std::cout << "line total vis : " << lines_vis << std::endl;
    std::cout << "scroll height  : " << scroll_h << std::endl;
    std::cout << "scroll y top   : " << scroll_y << std::endl;
    std::cout << "scroll hover   : " << hovered << std::endl;
    std::cout << "scroll drag    : " << dragging << "\n" << std::endl;
    std::cout << "delta lines    : " << delta_lines << std::endl;
    std::cout << "m_delta        : " << m_delta << std::endl;
    std::cout << "\n\n" << std::endl;
#endif


    SendMessageW( parent, EM_SCROLLCARET, 0, 0 );
    cscroll_draw( false );
  }
  bool cscroll_ishovered( POINT m_pos ) {
    m_pos.x -= 25; m_pos.y -= 50;
    if( PtInRect( &rect, m_pos ) ) {
      hovered = true;
      cscroll_draw();
      return true;
    }
    hovered = false;
    cscroll_draw();
    return false;
  }
public:
  void cscroll_setinfo( bool redraw ) {
    curr_line  = (s32)SendMessageW( parent, EM_LINEFROMCHAR, -1, 0 ) + 1;
    line_count = (s32)SendMessageW( parent, EM_GETLINECOUNT, 0, 0 );
    line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;
    lines_vis = to_sz_point( get_wnd_sz( parent ) ).y / line_sz.cy;
    line_last = line_first + lines_vis - 1;
    if( line_count > lines_vis )
      scroll_h = bkrect.bottom / ( line_count - lines_vis );
    else
      scroll_h = bkrect.bottom + 1;
    if( curr_line == 1 )
      scroll_y = -1;
    else
      scroll_y = (s32)( ( bkrect.bottom - scroll_h ) * ( (f32)curr_line / (f32)line_count ) );

    rect = {
      rect.left,
      scroll_y,
      rect.right,
      scroll_y + scroll_h
    };

    if( redraw ) {
      RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE );
      cscroll_draw();
    }
  }
};