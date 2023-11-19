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

    s32 len = GetWindowTextLengthA( hwnd ) + 1;
    file_path = new char[ len ];
    GetWindowTextA( hwnd, file_path, len );
    KillTimer( GetParent( hwnd ), 1 );

    HANDLE file = CreateFileA( file_path,
      GENERIC_READ, 0, NULL, OPEN_EXISTING,
      FILE_ATTRIBUTE_NORMAL, NULL
    );

    if( file == INVALID_HANDLE_VALUE ) {
#ifdef _DEBUG
      printf( "invalid file handle\n" );
#endif
      break;
    }

    LARGE_INTEGER file_sz;
    GetFileSizeEx( file, &file_sz );
    ptr buf_sz = static_cast<ptr>( file_sz.QuadPart );
    LPVOID buf = new BYTE[ buf_sz ];

    // reverse notepad and see how they do it
    // or look at npp

    ptr bytes_read;
    if( ReadFile( file, buf, buf_sz, &bytes_read, nullptr ) && bytes_read == buf_sz )
      SetWindowTextA( txt_box, static_cast<LPCSTR>( buf ) );

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

  open_txt = CreateWindowExA( WS_EX_TRANSPARENT,
    "EDIT", 0,
    WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL,
    ( WND_BTN_SZ * 6 ) + 6,
    WND_BTN_SZ + 4,
    ( open_sz.right - open_sz.left ) - ( ( WND_BTN_SZ * 6 ) + 10 ),
    WND_BTN_SZ - 9,
    h_global, NULL,
    (HINSTANCE)GetWindowLongPtrA( h_global, GWLP_HINSTANCE ), NULL
  );

  SetWindowSubclass( open_txt, openproc, 0, 0 );

  SetTimer( h_global, 1, 100, nullptr );

  MSG msg;
  while( GetMessageA( &msg, nullptr, 0, 0 ) ) {
    TranslateMessage( &msg );
    DispatchMessageA( &msg );
  }

  KillTimer( h_global, 1 );

  return 1;
}
