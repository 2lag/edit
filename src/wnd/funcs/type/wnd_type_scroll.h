#pragma once

struct SCROLL_PROP {
  s32 line_count;
  s32 line_first;
  SIZE line_sz;
  s32 visible_lines;
  s32 line_last;
  s32 curr_line;
  s32 scroll_height;
  s32 scroll_y_top;
};

class CSCROLL {
public:
  HWND parent;
  RECT bkrect;
  RECT rect;
  bool hovered;
  bool dragging;
  POINT drag_pos;
  SCROLL_PROP pscroll;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;

    RECT r = get_wnd_sz( parent );
    bkrect = rect = {
      r.right, -1,
      r.right + 25, r.bottom + 1
    };

  }
  void cscroll_draw() {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush( COL_L_GRY );

    GetTextExtentPoint32W( hdc, L"A", 1, &pscroll.line_sz );
    cscroll_setinfo();

    FillRect( hdc, &bkrect, dbrush );
    FillRect( hdc, &rect, lbrush );
    
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
    m_pos.x -= 25; m_pos.y -= 50; // check it's right & that func returns as expected
    if( PtInRect( &rect, m_pos ) ) {
      hovered = true;
      return true;
    }

    hovered = false;
    return false;
  }
  void cscroll_scroll() {
    // get wm_commands en_vscroll message in wnd_func
  }
public:
  void cscroll_setinfo() {
    pscroll.line_count = Edit_GetLineCount( parent );
    pscroll.line_first = Edit_GetFirstVisibleLine( parent ) + 1;
    pscroll.visible_lines = to_sz_point( get_wnd_sz( parent ) ).y / pscroll.line_sz.cy,
    pscroll.line_last     = pscroll.line_first + pscroll.visible_lines - 1,
    pscroll.curr_line  = (s32)SendMessageW( txt_box, EM_LINEFROMCHAR, -1, 0 ) + 1,
    pscroll.scroll_height = bkrect.bottom / ( ( pscroll.line_count >= pscroll.visible_lines ) ? pscroll.line_count - pscroll.visible_lines : 1 ) + 1,
    pscroll.scroll_y_top   = ( bkrect.bottom - pscroll.scroll_height ) * ( pscroll.curr_line / pscroll.line_count );

    std::cout << "line count    : " << pscroll.line_count    << std::endl;
    std::cout << "line first    : " << pscroll.line_first    << std::endl;
    std::cout << "line size     : " << pscroll.line_sz.cy    << std::endl;
    std::cout << "lines visible : " << pscroll.visible_lines << std::endl;
    std::cout << "line last     : " << pscroll.line_last     << std::endl;
    std::cout << "current line  : " << pscroll.curr_line << std::endl;
    std::cout << "scroll height : " << pscroll.scroll_height << std::endl;
    std::cout << "scroll y top  : " << pscroll.scroll_y_top << std::endl;
    std::cout << "\n\n\n\n\n\n\n\n" << std::endl;

    rect = {
      rect.left,
      pscroll.scroll_y_top,
      rect.right,
      pscroll.scroll_y_top + pscroll.scroll_height
    };
  }
};