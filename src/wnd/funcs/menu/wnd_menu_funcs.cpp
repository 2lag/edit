#include "wnd_menu_funcs.h"
#include "wnd_menu.h"

#include "../util/config.h"

HWND menu_txt;

s32 wnd_menu_dropdown_toggle( bool &toggle, s32 idx ) {
  toggle = !toggle;

  if( toggle )
    wnd_menu_draw_dropdown( h_global, (s8)idx );
  else {
    HDC hdc = GetDC( h_global );

    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
  
    RECT wnd_sz = get_wnd_sz( h_global );

    RECT r { 0, 50, 24, wnd_sz.bottom - 25 };
    FillRect( hdc, &r, dbrush );

    ReleaseDC( h_global, hdc );
    DeleteObject( dbrush );

    wnd_clear_menus( true );
  }

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

LRESULT CALLBACK openproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
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

LRESULT CALLBACK saveproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR ) {
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
    s32 txt_box_len = GetWindowTextLengthA( txt_box ) + 1;
    u64 total_len = static_cast<u64>( path_len ) + static_cast<u64>( txt_box_len ) + 4ull; // +4 for "\r\n\r\n"
    file_path = new char[ total_len ];

    if( !file_path ) {
#ifdef _DEBUG
      printf("failed to allocate file_path memory\n");
#else
      SetWindowTextA( menu_txt, "memory error" );
#endif
      break;
    }

    if( !GetWindowTextA( hwnd, file_path, path_len ) ) {
#ifdef _DEBUG
      printf("failed to get text: %lu\n", GetLastError() );
#else
      SetWindowTextA( menu_txt, "error getting path" );
#endif
      delete[] file_path;
      break;
    }

    HANDLE file = CreateFileA( file_path,
      GENERIC_WRITE, FILE_SHARE_READ |
      FILE_SHARE_WRITE | FILE_SHARE_DELETE,
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if( file == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
      printf("error opening file handle : %lu\npath : %s\n", GetLastError(), file_path );
#else
      SetWindowTextA( menu_txt, "invalid path");
#endif

      delete[] file_path;
      break;
    }

    char *txt_box_txt = new char[ txt_box_len ];

    if( !txt_box_txt ) {
#ifdef _DEBUG
      printf("failed mem allocation for text box text\n" );
#else
      SetWindowTextA( menu_txt, "memory error");
#endif
      delete[] file_path;
      CloseHandle( file );
      break;
    }

    if( !GetWindowTextA( txt_box, txt_box_txt, txt_box_len ) ) {
#ifdef _DEBUG
      printf("failed to get text box text\n" );
#else
      SetWindowTextA( menu_txt, "error getting text");
#endif
      delete[] file_path;
      delete[] txt_box_txt;
      CloseHandle( file );
      break;
    }

    ptr bytes_wrote;
    if( !WriteFile( file, txt_box_txt, txt_box_len - 1, &bytes_wrote, NULL ) ) {
#ifdef _DEBUG
      printf( "error writing to file : %lu\n", GetLastError() );
#else
      SetWindowTextA( menu_txt, "failed to write to file" );
#endif
    } else {
      strcat_s( file_path, total_len, "\r\n\r\n");
      strcat_s( file_path, total_len, txt_box_txt );

      SetWindowTextA( txt_box, file_path ); // in case saved in wrong dir bc user ( me... OR YOU...... ) is dumb.
      DestroyWindow( menu_txt );
    }

    delete[] file_path;
    delete[] txt_box_txt;
    CloseHandle( file );
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

s32 wnd_menu_style_toggle( s32 idx ) {
  wnd_clear_menus( true );

  toggle_config_idx( idx );

  return 1;
}