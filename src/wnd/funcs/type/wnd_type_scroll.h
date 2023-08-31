#pragma once
#include "../../wincludes.h"

class CSCROLL {
public:
  RECT rect;
  bool hovered;

public:
  void cscroll_draw(); // finish
  void cscroll_getinfo(); // finish
  void cscroll_setinfo(); // finish
  bool cscroll_ishovered(); // finish
};

void wnd_type_scroll_create( HWND hwnd, POINT pwnd_sz );