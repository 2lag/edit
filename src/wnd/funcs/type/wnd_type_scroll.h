#pragma once

class CSCROLL {
public:
  HWND parent;
  RECT bkrect;
  RECT rect;
  POINT size;
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
    POINT size = to_sz_point( r );

    cscroll_draw();
  }
  void cscroll_draw() {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush( COL_L_GRY );

    // calculate rect/size
    s32 line_count = (s32)SendMessageW( parent, EM_GETLINECOUNT, 0, 0 );
    // get line height
    // get total amount of visible lines
    // get first visible line

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

    // use scrollwindowex or em_linescroll/em_scroll ? ( copy wnd_drag )
    // make sure to invalidate/repaint txt_box & limit position
  }
  bool cscroll_ishovered( POINT m_pos ) {
    m_pos.x -= 25; m_pos.y -= 50;
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
  void cscroll_setinfo() {
    // to set info when line count, scroll pos, current line, etc changes
    // update .rect with the appropriate math
  }
};