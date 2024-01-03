#include "wnd_mouse_hook.h"

bool user_resizing = false;

LRESULT CALLBACK mouse_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  if( ncode == HC_ACTION ) {
    MSLLHOOKSTRUCT* p_mouse = reinterpret_cast<MSLLHOOKSTRUCT*>( lp );

    switch( wp ) {
    case WM_MOUSEMOVE: {
      vscroll.cscroll_ishovered( p_mouse );

      if( vscroll.dragging )
        vscroll.cscroll_drag( p_mouse, user_resizing );
      else if( vscroll.mdragging )
        vscroll.cscroll_mbutton_scroll( p_mouse );
    } break;
    case WM_MBUTTONDOWN: {
      vscroll.cscroll_mbutton_on( p_mouse );
    } break;
    case WM_MBUTTONUP: {
      vscroll.cscroll_mbutton_off();
    } break;
    case WM_MOUSEWHEEL: {
      vscroll.cscroll_hover_scroll( p_mouse );
      vscroll.cscroll_update();
    } break;
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}