#pragma once
#include "../wincludes.h"

void wnd_title_draw( const HDC hdc, const RECT wnd_sz );
void wnd_title_cls( const bool mouse_over );
void wnd_title_max( const bool mouse_over );
void wnd_title_min( const bool mouse_over );