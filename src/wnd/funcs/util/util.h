#pragma once

inline RECT get_wnd_sz( const HWND hwnd ) {
  RECT r;
  GetClientRect( hwnd, &r );
  return r;
}

inline POINT to_sz_point( const RECT r ) {
  return { r.right - r.left, r.bottom - r.top };
}

inline POINT to_pos_point( const RECT r ) {
  return { r.left, r.top };
}