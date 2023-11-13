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

    p_key->flags |= LLKHF_INJECTED;


    switch( p_key->vkCode ) {
    case 0x41: { // CTRL + A
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      Edit_SetSel( txt_box, 0, -1 );

      return 1;
    } break;
    case 0x46: { // CTRL + F
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      return wnd_menu_dropdown_toggle( m_base_open[0], 0 );
    } break;
    case 0x4D: { // CTRL + M
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      return wnd_menu_subdropdown_toggle( m_base_open[1], m_sub_open );
    } break;
    case 0x4E: { // CTRL + N
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      return wnd_menu_new_wnd( m_base_open[0] );
    } break;
    case 0x4F: { // CTRL + O
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      if( m_base_open[0] ) { // do open

        m_base_open[0] = false;
        wnd_clear_menus( true );

        // add an edit control into the menu bar on the right side of the menus
        // open/read file in binary mode ( mayb )
        // copy into edit control

        return 1;
      }
    } break;
    case 0x53: { // CTRL + S
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      if( m_base_open[0] ) { // do save instead
        m_base_open[0] = false;
        wnd_clear_menus( true );

        // use same method to get c:/dir/filename.type input as above
        // figure out way to copy entirety of edit control
        // create then write content to file
        //   make sure to force overwrite

        return 1;
      }

      return wnd_menu_dropdown_toggle( m_base_open[2], 2 );
    } break;
    case 0x54: { // CTRL + T
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      return wnd_menu_dropdown_toggle( m_base_open[1], 1 );
    } break;
    default:
      if( GetAsyncKeyState( VK_CONTROL ) )
        break;
      wnd_clear_menus();
    }
  }

  return CallNextHookEx( 0, ncode, wp, lp );
}