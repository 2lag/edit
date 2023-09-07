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
  POINT drag_pos;

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
  void cscroll_draw( bool redraw = false ) {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
    HBRUSH lbrush;
    if( hovered )
      lbrush = CreateSolidBrush( COL_L_GRY );
    else
      lbrush = CreateSolidBrush( COL_M_GRY );

    GetTextExtentPoint32W( hdc, L"A", 1, &line_sz );
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
    drag_pos = m_pos;
  }
  void cscroll_drag( POINT m_pos ) {
    if( !dragging )
      return;
    // use scrollwindowex or em_linescroll/em|en_scroll ? ( copy wnd_drag )
    // make sure to invalidate/repaint txt_box & limit position
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
  void cscroll_scroll() {

  }
public:
  void cscroll_setinfo( bool redraw ) {
    line_count = (s32)SendMessageW( parent, EM_GETLINECOUNT, 0, 0 );
    line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;
    lines_vis = to_sz_point( get_wnd_sz( parent ) ).y / line_sz.cy;
    line_last     = line_first + lines_vis - 1;
    curr_line  = (s32)SendMessageW( txt_box, EM_LINEFROMCHAR, -1, 0 ) + 1;
    if( line_count > lines_vis )
      scroll_h = bkrect.bottom / ( line_count - lines_vis );
    else
      scroll_h = bkrect.bottom + 1;
    scroll_y   = ( bkrect.bottom - scroll_h ) * ( curr_line / line_count );
    // ^^ ** fix top most scroll when small scroll is in action ** ^^
    // also fix the fucking math because its clearly not working right
#ifdef _DEBUG
    std::cout << "current line   : " << curr_line << std::endl;
    std::cout << "line count     : " << line_count << std::endl;
    std::cout << "line first vis : " << line_first << std::endl;
    std::cout << "line last vis  : " << line_last << std::endl;
    std::cout << "line total vis : " << lines_vis << std::endl;
    std::cout << "line height    : " << line_sz.cy << std::endl;
    std::cout << "scroll height  : " << scroll_h << std::endl;
    std::cout << "scroll y top   : " << scroll_y << std::endl;
    std::cout << "scroll hover   : " << hovered << std::endl;
    std::cout << "\n\n\n\n\n\n" << std::endl;
#endif
    rect = {
      rect.left,
      scroll_y,
      rect.right,
      scroll_y + scroll_h
    };

    if( redraw ) {
      RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW );
      cscroll_draw();
    }
  }
};