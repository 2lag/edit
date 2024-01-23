#include "key_hook.h"

#include "hooks.h"

#include "../menu/menu.h"
#include "../menu/menu_funcs.h"

#include "../edit/line_count.h"
#include "../edit/edit_ctl.h"

#include "../util/keylist.h"

LRESULT CALLBACK key_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  static std::unordered_map<u8, bool> key_down;
  static bool m_sub_open = false;

  if( ncode == HC_ACTION ) {
    KBDLLHOOKSTRUCT* p_key = reinterpret_cast<KBDLLHOOKSTRUCT*>( lp );

    // records once per keydown ( blocks multiples )
    if( wp == WM_KEYDOWN && !key_down[ p_key->vkCode ] ) {
      record_macro( p_key->vkCode );
      key_down[ p_key->vkCode ] = true;
    } else if( wp == WM_KEYUP )
      key_down[ p_key->vkCode ] = false;

    if( wp != WM_KEYDOWN )
      return CallNextHookEx( 0, ncode, wp, lp );

    // make sure this is right
    if( m_sub_open && (
          p_key->vkCode != 0x4D  &&
          p_key->vkCode != 0x50  &&
          p_key->vkCode != 0x52  )) // M P R
      m_sub_open = false; // ensures submenu is closed

    if( GetAsyncKeyState( VK_CONTROL ) ) {
      p_key->flags |= LLKHF_INJECTED;

      switch( p_key->vkCode ) {
      case 0x41: { // CTRL + A
        Edit_SetSel( txt_box, 0, -1 );

        return 1;
      } break;
      case 0x45: { // CTRL + E
        wnd_unhook();
        PostQuitMessage( 0 );
      } break;
      case 0x46: { // CTRL + F
        if( m_base_open[2] )
          return wnd_menu_style_toggle(0);
        
        if( m_base_open[1] )
          return wnd_menu_edit_ctrl( m_base_open[1], 2 );
        
        return wnd_menu_dropdown_toggle( m_base_open[0], 0 );
      } break;
      case 0x4C: { // CTRL + L
        if( m_base_open[2] )
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
        if( m_base_open[2] )
          return wnd_menu_style_toggle(1);
        if( m_sub_open ) {
          wnd_clear_menus();
          return playback_macro();
        }
      } break;
      case 0x52: { // CTRL + R
        if( m_sub_open ) {
          macro_recording = !macro_recording;

          if( !macro_recording ) {
            // trim final 4 here ( ctrl t m r )
            if( macro.size() >= 4 )
              macro.erase( macro.end() - 4, macro.end() );

            for( s32 idx = 0; idx < macro.size(); idx++ ) {
#ifdef _DEBUG
              printf( "%s\n", get_key( macro.at( idx ) ) );
#endif
            }
          } else
            macro.clear();
        }
        wnd_clear_menus();
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