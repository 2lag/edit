#pragma once

class CSCROLL {
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
      r.right, -1,
      r.right + 25, r.bottom + 1
    };

    cscroll_draw();
  }
  void cscroll_draw() {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush( COL_L_GRY );

    s32 line_count = Edit_GetLineCount( parent ),
        line_first = Edit_GetFirstVisibleLine( parent ) + 1;
    SIZE line_sz;
    GetTextExtentPoint32W( hdc, L"A", 1, &line_sz );
    s32 visible_lines = to_sz_point( get_wnd_sz( parent ) ).y / line_sz.cy,
        line_last     = line_first + visible_lines - 1,
        current_line  = (s32)SendMessageW( txt_box, EM_LINEFROMCHAR, -1, 0 ) + 1,
        scroll_height = bkrect.bottom / ( ( line_count >= visible_lines ) ? line_count - visible_lines : 1 ) + 1,
        scroll_ytop   = ( bkrect.bottom - scroll_height ) * ( current_line / line_count );

    rect = {
      rect.left, scroll_ytop,
      rect.right, scroll_ytop + scroll_height
    };

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
    // to set info when line count, scroll pos, current line, etc changes
    // update .rect with the appropriate math
  }
};