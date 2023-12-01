#include "wnd_menu_funcs.h"
#include "wnd_menu.h"

HWND menu_txt;

s32 wnd_menu_dropdown_toggle( bool &toggle, s32 idx ) {
  toggle = !toggle;

  if( toggle )
    wnd_menu_draw_dropdown( h_global, idx );
  else
    wnd_clear_menus( true );

  return 1;
}

s32 wnd_menu_subdropdown_toggle( bool &check, bool &toggle ) {
  if( !check )
    return 1;

  toggle = !toggle;

  if( toggle )
    wnd_menu_draw_sub_dropdown( h_global );
  else {
    check = false;
    wnd_clear_menus( true );
  }

  return 1;
}

s32 wnd_menu_new_wnd( bool &toggle ) {
  if( !toggle )
    return 1;

  m_base_open[0] = false;
  wnd_clear_menus( true );

  ptr buf_len = GetCurrentDirectoryA( 0, NULL ) + (ptr)strlen( "\\edit.exe" ) + 1;
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

LRESULT CALLBACK openproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  static bool once = false;
  
  if( !once ) {
    SetFocus( hwnd );
    once = true;
  }

  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    s32 path_len = GetWindowTextLengthA( hwnd ) + 1;
    file_path = new char[ path_len ];
    GetWindowTextA( hwnd, file_path, path_len );

    HANDLE file = CreateFileA( file_path,
      GENERIC_READ, 0, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, NULL
    );

    if( file == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
      printf( "invalid path : %s\n", file_path );
#else
      char *err = new char[ 16 + path_len ];
      sprintf_s( err, 16 + path_len, "invalid path : %s", file_path );
      SetWindowTextA( txt_box, err );
      delete[] err;
#endif

      delete[] file_path;
      break;
    }

    LARGE_INTEGER file_sz;
    GetFileSizeEx( file, &file_sz );
    ptr buf_sz = static_cast<ptr>( file_sz.QuadPart );
    LPVOID buf = new BYTE[ buf_sz ];

    ptr bytes_read;
    if( ReadFile( file, buf, buf_sz, &bytes_read, nullptr ) && bytes_read == buf_sz )
      SetWindowTextA( txt_box, static_cast<LPCSTR>( buf ) );
    
    delete[] file_path;
    delete[] static_cast<BYTE*>( buf );
    CloseHandle( file );
    DestroyWindow( hwnd );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

LRESULT CALLBACK saveproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  static bool once = false;
  
  if( !once ) {
    SetFocus( hwnd );
    once = true;
  }

  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    s32 path_len = GetWindowTextLengthA( hwnd ) + 1;
    file_path = new char[ path_len ];
    GetWindowTextA( hwnd, file_path, path_len );

    HANDLE file = CreateFileA( file_path,
      GENERIC_WRITE, FILE_SHARE_READ |
      FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if( file == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
      printf( "error opening file handle : %lu\npath : %s\n", GetLastError(), file_path );
#else
      SetWindowTextA( menu_txt, "invalid path" );
#endif

      delete[] file_path;
      break;
    } else {
      s32 txt_len = GetWindowTextLengthA( txt_box ) + 1;
      char *txt_box_txt = new char[ txt_len ];
      GetWindowTextA( txt_box, txt_box_txt, txt_len );

      ptr bytes_wrote;
      if( WriteFile( file, txt_box_txt, txt_len - 1, &bytes_wrote, NULL ) ) {
        SetWindowTextA( txt_box, file_path ); // in case saved in wrong dir bc user ( me... OR YOU...... ) is dumb.
        DestroyWindow( menu_txt );
      } else {
#ifdef _DEBUG
        printf( "error writing to file : %lu\n", GetLastError() );
#else
        SetWindowTextA( menu_txt, "failed to write to file" );
#endif
      }
      
      delete[] file_path;
      delete[] txt_box_txt;
      CloseHandle( file );
    }
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

s32 wnd_menu_edit_ctrl( bool &toggle, s32 idx ) {
  if( !toggle )
    return 1;

  if( menu_txt )
    DestroyWindow( menu_txt );

  toggle = false;
  wnd_clear_menus( true );

  RECT menu_sz = get_wnd_sz( h_global );

  menu_txt = CreateWindowExA( WS_EX_TRANSPARENT,
    "EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL,
    ( WND_BTN_SZ * 6 ) + 6,
    WND_BTN_SZ + 4,
    ( menu_sz.right - menu_sz.left ) - ( ( WND_BTN_SZ * 6 ) + 10 ),
    WND_BTN_SZ - 9,
    h_global, NULL,
    (HINSTANCE)GetWindowLongPtrA( h_global, GWLP_HINSTANCE ), NULL
  );

  SetWindowSubclass( menu_txt,
    ( idx == 0 ) ? openproc : saveproc,
    0, 0
  );

  MSG msg;
  while( GetMessageA( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage( &msg );
    DispatchMessageA( &msg );
  }

  return 1;
}