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

LRESULT CALLBACK openproc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR class_uid, DWORD_PTR data ) {
  static bool once = false;
  
  switch( msg ) {
  case WM_CHAR: {
    if( wp != VK_RETURN )
      break;

    // set the path/file to a var that we can use to import
  } break;
  case WM_ERASEBKGND: {
    
    HDC hdc = (HDC)wp;
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY );
    RECT open_sz = get_wnd_sz( hwnd );
    FillRect( hdc, &open_sz, dbrush );
    DeleteObject( dbrush );

    if( !once ) {
      SetFocus( hwnd );
      once = true;
    }

    return 1;
  }
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
  // open/read file in binary? mode once it's set
  // copy contents into txt_box

  return 1;
}