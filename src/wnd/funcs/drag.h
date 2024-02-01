#pragma once
#include "../wincludes.h"

void drag_on( const HWND hwnd, const POINT m_pos );
void drag_off();
void drag( const HWND hwnd, const POINT m_pos );
void drag_resize( const HWND hwnd );