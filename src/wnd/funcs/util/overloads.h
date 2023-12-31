#pragma once

inline POINT operator+=( POINT &a, const POINT b ) {
  return { a.x += b.x, a.y += b.y }; // only god knows why it doesnt work without the =
}

inline POINT operator+=( const POINT a, const s32 b ) {
  return { a.x + b, a.y + b };
}

inline POINT operator+( const POINT a, const POINT b ) {
  return { a.x + b.x, a.y + b.y };
}

inline POINT operator-=( const POINT a, const s32 b ) {
  return { a.x - b, a.y - b };
}

inline POINT operator-( const POINT a ) {
  return { -a.x, -a.y };
}

inline POINT operator-( const POINT a, const POINT b ) {
  return { a.x - b.x, a.y - b.y };
}

inline bool operator==( const POINT a, const POINT b ) {
  return( a.x == b.x && a.y == b.y );
}

inline POINT operator/( const POINT a, const s32 b ) {
  return { a.x / b, a.y / b };
}

inline bool operator!( const POINT &a ) {
  return ( a.x == 0 || a.y == 0 );
}

inline bool operator!( const RECT& a ) {
  return ( a.left == 0 && a.right == 0 && a.top == 0 && a.bottom == 0 );
}