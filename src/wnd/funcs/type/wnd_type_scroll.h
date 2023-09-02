#pragma once

class CSCROLL {
public:
  HWND parent;
  RECT rect;
  bool hovered;
  POINT size;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;
    RECT r = get_wnd_sz( parent );
    rect = {
      r.right, r.top - 1,
      r.right + 25, r.bottom + 1
    };
    cscroll_draw();
  }
  void cscroll_draw() {
    HDC hdc = GetDC( parent );
    HBRUSH lbrush = CreateSolidBrush( COL_L_GRY );
    FillRect( hdc, &rect, lbrush );
    DeleteObject( lbrush );
    ReleaseDC( parent, hdc );
  }
public:
  void cscroll_drag_off() {
    // copy drag on/off but use ishovered
  }
  void cscroll_drag_on() {
    // copy drag on/off but use ishovered
  }
  void cscroll_drag() {
    // finish after drag_on, then use scrollwindowex ?
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