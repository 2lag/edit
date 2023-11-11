#include "wnd_key_hook.h"

#include "../wnd_menu.h"
#include "../edit/wnd_edit_line_count.h"
#include "../edit/wnd_edit.h"


LRESULT CALLBACK key_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  static bool m_sub_open = false;

  if( ncode == HC_ACTION ) {
    KBDLLHOOKSTRUCT* p_key = reinterpret_cast<KBDLLHOOKSTRUCT*>( lp );

    if( wp != WM_KEYDOWN )
      return CallNextHookEx( 0, ncode, wp, lp );
    else if( p_key->vkCode != 0x4D )
        m_sub_open = false;


    switch( p_key->vkCode ) {

    p_key->flags |= LLKHF_INJECTED;

    case 0x41: { // CTRL + A
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      Edit_SetSel( txt_box, 0, -1 );

      return 1;
    } break;
    case 0x46: { // CTRL + F
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      m_base_open[0] = !m_base_open[0];

      if( m_base_open[0] )
        wnd_menu_draw_dropdown( h_global, 0 );
      else
        wnd_clear_menus( true );

      return 1;
    } break;
    case 0x4D: { // CTRL + M
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      if( !m_base_open[1] )
        return 1;

      m_sub_open = !m_sub_open;

      if( m_sub_open )
        wnd_menu_draw_sub_dropdown( h_global );
      else {
        m_base_open[1] = false;
        wnd_clear_menus( true );
      }

      return 1;
    } break;
    case 0x54: { // CTRL + T
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      m_base_open[1] = !m_base_open[1];

      if( m_base_open[1] )
        wnd_menu_draw_dropdown( h_global, 1 );
      else
        wnd_clear_menus( true );

      return 1;
    } break;
    case 0x53: { // CTRL + S
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      m_base_open[2] = !m_base_open[2];

      if( m_base_open[2] )
        wnd_menu_draw_dropdown( h_global, 2 );
      else
        wnd_clear_menus( true );

      return 1;
    } break;
    default: {
      if( GetAsyncKeyState( VK_CONTROL ) )
        break;
      else
        wnd_clear_menus();
    } break;
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}