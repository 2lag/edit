#pragma once

inline RECT get_wnd_sz( const HWND hwnd ) {
  RECT r;
  (void)GetClientRect( hwnd, &r );
  return r;
}

inline POINT get_size( const RECT r ) {
  return { r.right - r.left, r.bottom - r.top };
}

inline POINT get_position( const RECT r ) {
  return { r.left, r.top };
}