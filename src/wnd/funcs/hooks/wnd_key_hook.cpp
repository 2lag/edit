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

      { // open file dropdown
        m_base_open[0] = !m_base_open[0];

        if( m_base_open[0] )
          wnd_menu_draw_dropdown( h_global, 0 );
        else
          wnd_clear_menus( true );

        return 1;
      }
    } break;
    case 0x4D: { // CTRL + M
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      { // open macro subdropdown
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
      }
    } break;
    case 0x4E: { // CTRL + N
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      { // new 
        if( !m_base_open[0] )
          return 1;

        m_base_open[0] = false;
        wnd_clear_menus( true );

        ptr buf_len = GetCurrentDirectoryW( 0, NULL ) + (ptr)wcslen( L"\\edit.exe" ) + 1;
        WCHAR *buf = new WCHAR[ buf_len ];
        GetCurrentDirectoryW( buf_len, buf );

        wcscat_s( buf, buf_len, L"\\edit.exe\0" );

        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory( &si, sizeof( si ) );
        si.cb = sizeof( si );
        ZeroMemory( &pi, sizeof( pi ) );

        CreateProcessW( buf,
          NULL, NULL, NULL,
          NULL, DETACHED_PROCESS,
          NULL, NULL, &si, &pi
        );

        delete[] buf;

        CloseHandle( pi.hProcess );
        CloseHandle( pi.hThread );

        return 1;
      }
    } break;
    case 0x4F: { // CTRL + O
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      { // open
        if( !m_base_open[0] )
          return 1;

        m_base_open[0] = false;
        wnd_clear_menus( true );

        // figure out way to get c:/dir/filename.type input
        //   look to see if pre-defined open/save dialogs exist
        //   if not, just add an edit control into the menu bar on the right side of the menus
        // open/read file in binary mode ( mayb )
        // copy into edit control

        return 1;
      }
    } break;
    case 0x53: { // CTRL + S
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      { // save
        if( m_base_open[0] ) {
          m_base_open[0] = false;
          wnd_clear_menus( true );

          // use same method to get c:/dir/filename.type input as above
          // figure out way to copy entirety of edit control
          // create then write content to file
          //   make sure to force overwrite

          return 1;
        }
      }

      { // open styles dropdown
        m_base_open[2] = !m_base_open[2];

        if( m_base_open[2] )
          wnd_menu_draw_dropdown( h_global, 2 );
        else
          wnd_clear_menus( true );

        return 1;
      }
    } break;
    case 0x54: { // CTRL + T
      if( !GetAsyncKeyState( VK_CONTROL ) )
        break;

      { // open tools dropdown
        m_base_open[1] = !m_base_open[1];

        if( m_base_open[1] )
          wnd_menu_draw_dropdown( h_global, 1 );
        else
          wnd_clear_menus( true );

        return 1;
      }
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