#pragma once
#include "../wincludes.h"

void wnd_title_draw( const HDC hdc, const RECT wnd_sz );
void wnd_title_cls( const RECT wnd_sz, const POINT m_pos );
void wnd_title_max( const RECT wnd_sz, const POINT m_pos );
void wnd_title_min( const RECT wnd_sz, const POINT m_pos );