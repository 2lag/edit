#pragma once
#include "../wincludes.h"

enum WND_RESIZE_CASES {
  EDGE_TOP_LEFT  = 1,
  EDGE_TOP         ,
  EDGE_TOP_RIGHT   ,
  EDGE_RIGHT       ,
  EDGE_BOTTOM_RIGHT,
  EDGE_BOTTOM      ,
  EDGE_BOTTOM_LEFT ,
  EDGE_LEFT
};

void resize_get_cursor( const POINT m_pos );
void resize_on( const POINT m_pos );
void resize_off();
void resize_title( const POINT m_pos );
void resize( const POINT m_pos );