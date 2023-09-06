#pragma once

struct SCROLL_PROP {
  s32 line_count;
  s32 line_first;
  SIZE line_sz;
  s32 lines_vis;
  s32 line_last;
  s32 curr_line;
  s32 scroll_h;
  s32 scroll_y;
};

struct SCROLL_DRAW {
  HWND parent;
  RECT bkrect;
  RECT rect;
  bool hovered;
  bool dragging;
  POINT drag_pos;
};

class CSCROLL {
private:
  SCROLL_PROP pscroll;
  SCROLL_DRAW dscroll;

public:
  void cscroll_create( HWND hwnd ) {
    dscroll.parent = hwnd;

    RECT r = get_wnd_sz( dscroll.parent );
    dscroll.bkrect = dscroll.rect = {
      r.right + 1,
      -1,
      r.right + 25,
      r.bottom + 1
    };

  }
  void cscroll_draw( bool redraw = false ) {
    HDC hdc = GetDC( dscroll.parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
    HBRUSH lbrush;
    if( dscroll.hovered )
      lbrush = CreateSolidBrush( COL_L_GRY );
    else
      lbrush = CreateSolidBrush( COL_M_GRY );

    GetTextExtentPoint32W( hdc, L"A", 1, &pscroll.line_sz );
    cscroll_setinfo( redraw );

    FillRect( hdc, &dscroll.bkrect, dbrush );
    FillRect( hdc, &dscroll.rect  , lbrush );
    
    DeleteObject( dbrush );
    DeleteObject( lbrush );
    ReleaseDC( dscroll.parent, hdc );
  }
public:
  void cscroll_drag_off() {
    if( !dscroll.dragging )
      return;

    dscroll.dragging = false;
  }
  void cscroll_drag_on( POINT m_pos ) {
    if( !cscroll_ishovered( m_pos ) )
      return;

    dscroll.dragging = true;
    dscroll.drag_pos = m_pos;
  }
  void cscroll_drag( POINT m_pos ) {
    if( !dscroll.dragging )
      return;
    // use scrollwindowex or em_linescroll/em|en_scroll ? ( copy wnd_drag )
    // make sure to invalidate/repaint txt_box & limit position
  }
  bool cscroll_ishovered( POINT m_pos ) {
    m_pos.x -= 25; m_pos.y -= 50; // check it's right & that func returns as expected
    if( PtInRect( &dscroll.rect, m_pos ) ) {
      dscroll.hovered = true;
      cscroll_draw();
      return true;
    }
    cscroll_draw();
    dscroll.hovered = false;
    return false;
  }
  void cscroll_scroll() {
    // get wm_commands en_vscroll message in wnd_func
  }
public:
  void cscroll_setinfo( bool redraw ) {
    pscroll.line_count = (s32)SendMessageW( dscroll.parent, EM_GETLINECOUNT, 0, 0 );
    pscroll.line_first = (s32)SendMessageW( dscroll.parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;
    pscroll.lines_vis = to_sz_point( get_wnd_sz( dscroll.parent ) ).y / pscroll.line_sz.cy;
    pscroll.line_last     = pscroll.line_first + pscroll.lines_vis - 1;
    pscroll.curr_line  = (s32)SendMessageW( txt_box, EM_LINEFROMCHAR, -1, 0 ) + 1;
    if( pscroll.line_count > pscroll.lines_vis )
      pscroll.scroll_h = dscroll.bkrect.bottom / ( pscroll.line_count - pscroll.lines_vis );
    else
      pscroll.scroll_h = dscroll.bkrect.bottom + 1;
    pscroll.scroll_y   = ( dscroll.bkrect.bottom - pscroll.scroll_h ) * ( pscroll.curr_line / pscroll.line_count );
    // ^^ ** fix top most scroll when small scroll is in action ** ^^
    // also fix the fucking math because its clearly not working right

    std::cout << "line count    : " << pscroll.line_count    << std::endl;
    std::cout << "line first    : " << pscroll.line_first    << std::endl;
    std::cout << "line size     : " << pscroll.line_sz.cy    << std::endl;
    std::cout << "lines visible : " << pscroll.lines_vis << std::endl;
    std::cout << "line last     : " << pscroll.line_last     << std::endl;
    std::cout << "current line  : " << pscroll.curr_line << std::endl;
    std::cout << "scroll height : " << pscroll.scroll_h << std::endl;
    std::cout << "scroll y top  : " << pscroll.scroll_y << std::endl;
    std::cout << "scroll hover  : " << dscroll.hovered << std::endl;
    std::cout << "\n\n\n\n\n\n\n" << std::endl;

    dscroll.rect = {
      dscroll.rect.left,
      pscroll.scroll_y,
      dscroll.rect.right,
      pscroll.scroll_y + pscroll.scroll_h
    };

    if( redraw ) {
      InvalidateRect( dscroll.parent, 0, 1 );
      cscroll_draw();
    }
  }
};