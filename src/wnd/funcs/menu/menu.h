#pragma once
#include "../../wincludes.h"

void clear_menus( const s32 ignore = -1, const bool overryde = false );

void draw_menu( const HDC hdc );
void draw_menu_dropdown( const HWND hwnd, const s8 idx );
void draw_menu_sub_dropdown( const HWND hwnd );