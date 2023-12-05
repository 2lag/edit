#include "wnd_key_hook.h"

#include "../menu/wnd_menu.h"
#include "../menu/wnd_menu_funcs.h"

#include "../edit/wnd_edit_line_count.h"
#include "../edit/wnd_edit.h"


LRESULT CALLBACK key_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  static bool m_sub_open = false;

  if( ncode == HC_ACTION ) {
    if( wp != WM_KEYDOWN )
      return CallNextHookEx( 0, ncode, wp, lp );

    KBDLLHOOKSTRUCT* p_key = reinterpret_cast<KBDLLHOOKSTRUCT*>( lp );

    if( p_key->vkCode != 0x4D )
      m_sub_open = false; // ensures submenu is closed

    if( GetAsyncKeyState( VK_CONTROL ) ) {
      p_key->flags |= LLKHF_INJECTED;

      switch( p_key->vkCode ) {
      case 0x41: { // CTRL + A
        Edit_SetSel( txt_box, 0, -1 );

        return 1;
      } break;
      case 0x46: { // CTRL + F
        if( m_base_open[2] )
          return wnd_menu_style_toggle(0);

        return wnd_menu_dropdown_toggle( m_base_open[0], 0 );
      } break;
      case 0x4C: { // CTRL + L
        return wnd_menu_style_toggle(2);
      } break;
      case 0x4D: { // CTRL + M
        return wnd_menu_subdropdown_toggle( m_base_open[1], m_sub_open );
      } break;
      case 0x4E: { // CTRL + N
        return wnd_menu_new_wnd( m_base_open[0] );
      } break;
      case 0x4F: { // CTRL + O
        return wnd_menu_edit_ctrl( m_base_open[0], 0 );
      } break;
      case 0x50: { // CTRL + P
        return wnd_menu_style_toggle(1);
      } break;
      case 0x53: { // CTRL + S
        if( m_base_open[0] ) // save route
          return wnd_menu_edit_ctrl( m_base_open[0], 1 );

        return wnd_menu_dropdown_toggle( m_base_open[2], 2 );
      } break;
      case 0x54: { // CTRL + T
        return wnd_menu_dropdown_toggle( m_base_open[1], 1 );
      } break;
      }
    } else
      wnd_clear_menus();
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}