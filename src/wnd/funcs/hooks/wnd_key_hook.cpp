#include "wnd_key_hook.h"

#include "../wnd_menu.h"
#include "../edit/wnd_edit_line_count.h"
#include "../edit/wnd_edit.h"


LRESULT CALLBACK key_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  if( ncode == HC_ACTION ) {
    KBDLLHOOKSTRUCT* p_key = reinterpret_cast<KBDLLHOOKSTRUCT*>( lp );

    if( wp == WM_KEYDOWN ) {

#ifdef _DEBUG
      printf( "%c\n", p_key->vkCode );
#endif // _DEBUG

      switch( p_key->vkCode ) {
      case 0x41: { // CTRL + A
        if( !GetAsyncKeyState( VK_CONTROL ) )
          break;

        Edit_SetSel( txt_box, 0, -1 );
      } break;
      case 0x46: { // CTRL + F
        if( !GetAsyncKeyState( VK_CONTROL ) )
          break;

        m_base_open[0] = !m_base_open[0];

        if( m_base_open[0] )
          wnd_menu_draw_dropdown( h_global, 0 );
        else {
          vscroll.cscroll_draw( true, true );
          RECT wnd_sz = get_wnd_sz( h_global );
          wnd_type_line_count( h_global, wnd_sz, true );
          wnd_type_outline( h_global, to_sz_point( wnd_sz ) );
        }
      } break;
      case 0x54: { // CTRL + T
        if( !GetAsyncKeyState( VK_CONTROL ) )
          break;

        m_base_open[1] = !m_base_open[1];

        if( m_base_open[1] )
          wnd_menu_draw_dropdown( h_global, 1 );
        else
          vscroll.cscroll_draw( true, true );
      } break;
      case 0x53: { // CTRL + S
        if( !GetAsyncKeyState( VK_CONTROL ) )
          break;

        m_base_open[2] = !m_base_open[2];

        if( m_base_open[2] )
          wnd_menu_draw_dropdown( h_global, 2 );
        else
          vscroll.cscroll_draw( true, true );
      } break;
      default:
        if( GetAsyncKeyState( VK_CONTROL ) )
          break;
        else
          wnd_clear_menus();
      }


#ifdef _DEBUG
      printf( "m1 open : %d\nm2 open : %d\nm3 open : %d\n\n\n\n\n\n\n\n\n\n\n\n\n", m_base_open[0], m_base_open[1], m_base_open[2] );
#endif // _DEBUG
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}