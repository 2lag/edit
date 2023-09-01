#pragma once
#include "../../wincludes.h"

class CSCROLL {
public:
  HWND parent;
  RECT rect;
  bool hovered;
  s32  height;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;
    RECT r = get_wnd_sz( parent );
    s32 sbw = (s32)( r.right * 0.025 );
    rect = {
      r.right - sbw, r.top,
      r.right, r.bottom
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
    // copy resize/drag on/off but use ishovered
  }
  void cscroll_drag_on() {
    // copy resize/drag on/off but use ishovered
  }
  void cscroll_drag(); // finish, use scrollwindowex ?
  bool cscroll_ishovered() {
    // ptinrect for rect and mousepos
    // change cursor to arrow or whatever is good for click
    // return true if so, false if not
  }
  void cscroll_scroll() {
    // get wm_commands en_vscroll message in wnd_func
  }
  void cscroll_setinfo(); // to set info when line count/scroll pos changes
};