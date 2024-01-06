#include "wnd_mouse_hook.h"

bool user_resizing = false;

LRESULT CALLBACK mouse_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  if( ncode == HC_ACTION ) {
    MSLLHOOKSTRUCT* p_mouse = reinterpret_cast<MSLLHOOKSTRUCT*>( lp );

    switch( wp ) {
    case WM_MOUSEMOVE: {
      vscroll.get_hovered( p_mouse );

      if( vscroll.dragging )
        vscroll.drag( p_mouse, user_resizing );
      else if( vscroll.mdragging )
        vscroll.mdrag_scroll( p_mouse );
    } break;
    case WM_MBUTTONDOWN: {
      vscroll.mdrag_on( p_mouse );
    } break;
    case WM_MBUTTONUP: {
      vscroll.mdrag_off();
    } break;
    case WM_MOUSEWHEEL: {
      vscroll.hover_scroll( p_mouse );
    } break;
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}