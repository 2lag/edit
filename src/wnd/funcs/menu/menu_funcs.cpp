#include "menu_funcs.h"
#include "menu.h"

#include "../util/config.h"

#include "../edit/line_count.h"

HWND menu_txt;

s32 menu_dropdown_toggle( bool &toggle, const s32 idx ) {
  toggle = !toggle;

  if( toggle )
    draw_menu_dropdown( h_global, (s8)idx );
  else
    clear_menus( true );

  return 1;
}

s32 menu_subdropdown_toggle( bool &check, bool &toggle ) {
  if( !check )
    return 1;

  toggle = !toggle;

  if( toggle )
    draw_menu_sub_dropdown( h_global );
  else {
    check = false;
    clear_menus( true );
  }

  return 1;
}

s32 menu_new_wnd( const bool toggle ) {
  if( !toggle )
    return 1;

  m_base_open[0] = false;
  clear_menus( true );

  ptr buf_len = GetCurrentDirectoryA( 0, 0 ) + (ptr)strlen( "\\edit.exe" ) + 1;
  char *buf = new char[ buf_len ];
  GetCurrentDirectoryA( buf_len, buf );

  strcat_s( buf, buf_len, "\\edit.exe\0" );

  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof( si ) );
  si.cb = sizeof( si );
  ZeroMemory( &pi, sizeof( pi ) );

  CreateProcessA( buf,
    NULL, NULL, NULL,
    NULL, DETACHED_PROCESS,
    NULL, NULL, &si, &pi
  );

  delete[] buf;

  CloseHandle( pi.hProcess );
  CloseHandle( pi.hThread );

  return 1;
}

char *file_path = nullptr;

HANDLE menu_get_file( HWND hwnd, u64 len, bool open ) {
  file_path = new char[ len ];

  if( !file_path ) {
    SetWindowTextA( hwnd, "memory error" );
    return nullptr;
  }

  if( !GetWindowTextA( hwnd, file_path, static_cast<s32>( len ) ) ) {
    SetWindowTextA( hwnd, "error getting path" );
    delete[] file_path;
    return nullptr;
  }

  HANDLE file = CreateFileA( file_path,
    open ?  GENERIC_READ : GENERIC_WRITE,
    open ?          NULL : FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
    NULL,
    open ? OPEN_EXISTING : CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  if( file == INVALID_HANDLE_VALUE ) {
    SetWindowTextA( menu_txt, "error getting file" );
    delete[] file_path;
    return nullptr;
  }

  return file;
}

LRESULT CALLBACK findproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
  static s8 *last_found = nullptr; // To keep track of the last found position

  switch(msg) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    s32 search_len = GetWindowTextLengthA( hwnd ) + 1;
    s8 *search_text = new s8[ search_len ];
    GetWindowTextA( hwnd, search_text, search_len );

    s32 txt_len = GetWindowTextLengthA( txt_box ) + 1;
    s8 *txt = new s8[ txt_len ];
    GetWindowTextA( txt_box, txt, txt_len );

    s8 *found = strstr( last_found ? last_found + 1 : txt, search_text );
    if( found ) {
      s64 start_pos = found - txt;
      s64 end_pos = start_pos + static_cast<s64>( search_len ) - 1;
      Edit_SetSel( txt_box, start_pos, end_pos );
      last_found = found;
    } else {
      // dumb hack-y shit to avoid text invis... again
      SetFocus( txt_box );
      Edit_SetSel( txt_box, 0, -1 );
#pragma warning( push )
#pragma warning( disable : 4245 )
      Edit_SetSel( txt_box, -1, 0 );
#pragma warning( pop )

      last_found = nullptr;
      DestroyWindow( hwnd );
    }

    delete[] txt;
    delete[] search_text;
  } break;
  case WM_DESTROY: {
    last_found = nullptr;
  } break;
  }
  return DefSubclassProc( hwnd, msg, wp, lp );
}

LRESULT CALLBACK openproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    s32 path_len = GetWindowTextLengthA( hwnd ) + 1;
    
    HANDLE file = menu_get_file( hwnd,
      static_cast<u64>( path_len ), true
    );

    if( file == nullptr ) {
      delete[] file_path;
      break;
    }

    LARGE_INTEGER file_sz;
    GetFileSizeEx( file, &file_sz );
    ptr buf_sz = static_cast<ptr>( file_sz.QuadPart );
    LPVOID buf = new BYTE[ buf_sz ];

    ptr bytes_read;
    if( !ReadFile( file, buf, buf_sz, &bytes_read, nullptr ) || bytes_read != buf_sz )
      SetWindowTextA( hwnd, "file read error" );

    SetWindowTextA( txt_box, static_cast<LPCSTR>( buf ) );
    
    delete[] static_cast<BYTE*>( buf );
    delete[] file_path;
    CloseHandle( file );
    DestroyWindow( hwnd );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

LRESULT CALLBACK saveproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    s32 path_len = GetWindowTextLengthA( hwnd ) + 1;
    s32 txt_box_len = GetWindowTextLengthA( txt_box ) + 1;
    u64 total_len = static_cast<u64>( path_len ) + static_cast<u64>( txt_box_len ) + 4ull; // +4 for "\r\n\r\n"
    
    HANDLE file = menu_get_file( hwnd,
      total_len, false
    );

    if( file == nullptr ) {
      delete[] file_path;
      break;
    }

    s8 *txt_box_txt = new s8[ txt_box_len ];

    if( !txt_box_txt ) {
      SetWindowTextA( hwnd, "memory error");
      delete[] file_path;
      CloseHandle( file );
      break;
    }

    if( !GetWindowTextA( txt_box, txt_box_txt, txt_box_len ) ) {
      SetWindowTextA( hwnd, "error getting text");
      delete[] file_path;
      delete[] txt_box_txt;
      CloseHandle( file );
      break;
    }

    ptr bytes_wrote;
    if( !WriteFile( file, txt_box_txt, txt_box_len - 1, &bytes_wrote, NULL ) )
      SetWindowTextA( hwnd, "failed to write to file" );
    else {
      strcat_s( file_path, total_len, "\r\n\r\n");
      strcat_s( file_path, total_len, txt_box_txt );

      SetWindowTextA( txt_box, file_path ); // in case saved in wrong dir bc user ( me... OR YOU...... ) is dumb.
      DestroyWindow( hwnd );
    }

    delete[] file_path;
    delete[] txt_box_txt;
    CloseHandle( file );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

s32 menu_edit_ctrl( bool &toggle, s32 idx ) {
  if( !toggle )
    return 1;

  if( menu_txt )
    DestroyWindow( menu_txt );

  toggle = false;
  
  menu_txt = CreateWindowExA( WS_EX_TRANSPARENT, "EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL,
    ( WND_BTN_SZ * 6 ) + 6,
    WND_BTN_SZ + 4,
    get_size( wnd_sz ).x - ( WND_BTN_SZ * 6 ) - 10,
    WND_BTN_SZ - 9,
    h_global, NULL,
    (HINSTANCE)GetWindowLongPtrA( h_global, GWLP_HINSTANCE ), NULL
  );

  switch( idx ) {
  case 0: {
    SetWindowSubclass( menu_txt, openproc, 0, 0 );
  } break;
  case 1: {
    SetWindowSubclass( menu_txt, saveproc, 0, 0 );
  } break;
  case 2: {
    SetWindowSubclass( menu_txt, findproc, 0, 0 );
  } break;
  default:
    return 1;
  }

  // dumb hack-y shit to avoid text invis
  SetFocus( txt_box );
  Edit_SetSel( txt_box, 0, -1 );
#pragma warning( push )
#pragma warning( disable : 4245 )
  Edit_SetSel( txt_box, -1, 0 );
#pragma warning( pop )

  SetFocus( menu_txt );

  draw_line_count( h_global, true );

  MSG msg;
  while( GetMessageA( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage( &msg );
    DispatchMessageA( &msg );
  }

  return 1;
}

s32 menu_toggle_style( s32 idx ) {
  clear_menus( true );

  toggle_config_idx( idx );

  return 1;
}