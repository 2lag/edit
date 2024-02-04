#include "hooks.h"

#include "../menu/menu.h"
#include "../menu/menu_funcs.h"

#include "../edit/macro.h"

// makes sure it's properly toggled
void check_submenu( ptr vk, bool* check ) {
  if( check && (
      vk != 0x4D &&
      vk != 0x50 &&
      vk != 0x52 )) // M P R
    *check = false;
}

LRESULT CALLBACK key_hook_proc( s32 ncode, WPARAM wp, LPARAM lp ) {
  static std::unordered_map<ptr, bool> key_down;
  static bool m_sub_open = false;

  if( ncode == HC_ACTION ) {
    KBDLLHOOKSTRUCT* p_key = reinterpret_cast<KBDLLHOOKSTRUCT*>( lp );

    // records once per keydown ( blocks multiples )
    if( wp == WM_KEYDOWN && !key_down[ p_key->vkCode ] ) {
      record_macro(
        static_cast<u8>( p_key->vkCode ),
        WM_KEYDOWN
      );
      key_down[ p_key->vkCode ] = true;
    } else if( wp == WM_KEYUP ) {
      key_down[ p_key->vkCode ] = false;
      record_macro(
        static_cast<u8>( p_key->vkCode ),
        WM_KEYUP
      );
    }

    if( wp != WM_KEYDOWN )
      return CallNextHookEx( 0, ncode, wp, lp );

    check_submenu( p_key->vkCode, &m_sub_open );

    if( GetAsyncKeyState( VK_CONTROL ) ) {
      p_key->flags |= LLKHF_INJECTED;

      switch( p_key->vkCode ) {
      case 0x41: { // CTRL + A
        Edit_SetSel( txt_box, 0, -1 );

        return 1;
      } break;
      case 0x45: { // CTRL + E
        if( m_base_open[0] ) {
          unhook();
          PostQuitMessage( 0 );
        }
      } break;
      case 0x46: { // CTRL + F
        if( m_base_open[2] )
          return menu_toggle_style(0);
        
        if( m_base_open[1] )
          return menu_edit_ctrl( m_base_open[1], 2 );
        
        return menu_dropdown_toggle( m_base_open[0], 0 );
      } break;
      case 0x4C: { // CTRL + L
        if( m_base_open[2] )
          return menu_toggle_style(2);
      } break;
      case 0x4D: { // CTRL + M
        return menu_subdropdown_toggle( m_base_open[1], m_sub_open );
      } break;
      case 0x4E: { // CTRL + N
        return menu_new_wnd( m_base_open[0] );
      } break;
      case 0x4F: { // CTRL + O
        return menu_edit_ctrl( m_base_open[0], 0 );
      } break;
      case 0x50: { // CTRL + P      
        if( m_base_open[2] )
          return menu_toggle_style(1);
        if( m_sub_open ) {
          clear_menus();
          return playback_macro();
        }
      } break;
      case 0x52: { // CTRL + R
        if( m_sub_open ) {
          macro_recording = !macro_recording;

          if( !macro_recording ) {
            // trim final 4 here ( ctrl t m r )
            if( macro.size() >= 4 )
              (void)macro.erase( macro.end() - 4, macro.end() );

            for( s32 idx = 0; idx < macro.size(); idx++ ) {
#ifdef _DEBUG
              (void)printf( "0x%x ", macro.at( idx ).first );
#endif
            }
#ifdef _DEBUG
            (void)printf( "\n" );
#endif
          } else {
            for( u8 idx = 0; idx < sizeof( u8 ); idx++ )
              keybd_event( idx, 0, KEYEVENTF_KEYUP, 0 );
            macro.clear();
          }
        }
        clear_menus();
      } break;
      case 0x53: { // CTRL + S
        if( m_base_open[0] )
          return menu_edit_ctrl( m_base_open[0], 1 );

        return menu_dropdown_toggle( m_base_open[2], 2 );
      } break;
      case 0x54: { // CTRL + T
        return menu_dropdown_toggle( m_base_open[1], 1 );
      } break;
      }
    } else
      clear_menus();
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}