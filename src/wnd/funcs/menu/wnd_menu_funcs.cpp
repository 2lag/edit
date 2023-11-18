#include "wnd_menu_funcs.h"
#include "wnd_menu.h"

HWND open_txt;

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

WCHAR *file_path = nullptr;

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

    s32 len = GetWindowTextLengthW( hwnd ) + 1;
    file_path = new WCHAR[ len ];
    GetWindowTextW( hwnd, file_path, len );
    KillTimer( GetParent( hwnd ), 1 );

    HANDLE file = CreateFileW( file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if( file == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
      wprintf( L"invalid file handle\n" );
#endif
      break;
    }

    LARGE_INTEGER file_sz;
    GetFileSizeEx( file, &file_sz );
    ptr buf_sz = static_cast<ptr>( file_sz.QuadPart );
    LPVOID buf = new BYTE[ buf_sz ];

    ptr bytes_read;
    if( ReadFile( file, buf, buf_sz, &bytes_read, nullptr ) && bytes_read == buf_sz ) // fix this
      SetWindowTextW( txt_box, static_cast<LPCWSTR>( buf ) ); // or this

    delete[] static_cast<BYTE*>( buf );
    CloseHandle( file );
    DestroyWindow( hwnd );
  } break;
  }

  return DefSubclassProc( hwnd, msg, wp, lp );
}

s32 wnd_menu_open_ctrl( bool &toggle ) {
  if( !toggle )
    return 1;

  toggle = false;
  wnd_clear_menus( true );

  RECT open_sz = get_wnd_sz( h_global );

  open_txt = CreateWindowExW( WS_EX_TRANSPARENT,
    L"EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL,
    ( WND_BTN_SZ * 6 ) + 6,
    WND_BTN_SZ + 4,
    ( open_sz.right - open_sz.left ) - ( ( WND_BTN_SZ * 6 ) + 10 ),
    WND_BTN_SZ - 9,
    h_global, NULL,
    (HINSTANCE)GetWindowLongPtrW( h_global, GWLP_HINSTANCE ), NULL
  );

  SetWindowSubclass( open_txt, openproc, 0, 0 );

  SetTimer( h_global, 1, 100, nullptr );

  MSG msg;
  while( GetMessageW( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage( &msg );
    DispatchMessageW( &msg );
  }

  KillTimer( h_global, 1 );

  return 1;
}
