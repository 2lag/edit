#pragma once
#include "../../wincludes.h"

void wnd_type_create( HWND hwnd, POINT pwnd_sz );
void wnd_type_outline( HWND hwnd, POINT wnd_sz );
void wnd_type_customize( WPARAM wp );
void wnd_type_caret_pos( HWND hwnd, RECT wnd_sz );