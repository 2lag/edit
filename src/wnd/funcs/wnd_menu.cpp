#include "wnd_menu.h"

#define OBJ_BASE_COUNT 6

const RECT o_menu_file  {  -1, 24,  50, 50 },
           o_menu_tools {  50, 24, 102, 50 },
           o_menu_style { 102, 24, 152, 50 };

bool m_base_hovered[ OBJ_BASE_COUNT ] = { false };

void wnd_menu_draw( HDC hdc, RECT wnd_sz ) {
  const static RECT i_menu_file  {   0, 25,  49, 49 },
                    i_menu_tools {  50, 25, 101, 49 },
                    i_menu_style { 102, 25, 151, 49 };

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( o_menu_file , COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_file , COL_D_GRY, false, L"File" , { 12,  4 } ),
    wnd_menu_create( o_menu_tools, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_tools, COL_D_GRY, false, L"Tools", {  8,  4 } ),
    wnd_menu_create( o_menu_style, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_style, COL_D_GRY, false, L"Style", {  8,  4 } )
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    if( m_base_hovered[ idx ] )
      SetTextColor( hdc, COL_L_GRY );
    else
      SetTextColor( hdc, COL_M_GRY );

    TextOutW( hdc,
      obj[ idx ].r.left + obj[ idx ].offset.x,
      obj[ idx ].r.top  + obj[ idx ].offset.y,
      obj[ idx ].txt,
      lstrlenW( obj[ idx ].txt )
    );
  }

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_menu_hovered( MSLLHOOKSTRUCT* p_mouse ) {
  POINT cm_pos = p_mouse->pt;
  ScreenToClient( h_global, &cm_pos );

  static bool was_hovered[ OBJ_BASE_COUNT ] = { false };

  m_base_hovered[ 1 ] = (bool)PtInRect( &o_menu_file , cm_pos );
  m_base_hovered[ 3 ] = (bool)PtInRect( &o_menu_tools, cm_pos );
  m_base_hovered[ 5 ] = (bool)PtInRect( &o_menu_style, cm_pos );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    if( m_base_hovered[ idx ] == was_hovered[ idx ] )
      continue;
    
    was_hovered[ idx ] = m_base_hovered[ idx ];

    RECT r_invalid{};

    switch( idx ) {
    case 1: {
      r_invalid = o_menu_file;
    } break;
    case 3: {
      r_invalid = o_menu_tools;
    } break;
    case 5: {
      r_invalid = o_menu_style;
    } break;
    }

    InvalidateRect( h_global, &r_invalid, TRUE );
    InvalidateRect( txt_box, 0, TRUE );
  }
}

void wnd_menu_draw_dropdown( HWND hwnd, s8 idx ) {
  HDC hdc = GetDC( hwnd );

  WND_MENU obj[ 6 ];

  switch( idx ) {
  case 1: {
    obj[0] = wnd_menu_create( {   0,  49,  50,  76 }, COL_L_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( {   0,  50,  49,  75 }, COL_D_GRY, false, L"New"            , { 10,  4 } );
    obj[2] = wnd_menu_create( {   0,  75,  50, 101 }, COL_L_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( {   0,  76,  49, 100 }, COL_D_GRY, false, L"Open"           , {  8,  4 } );
    obj[4] = wnd_menu_create( {   0, 100,  50, 126 }, COL_L_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( {   0, 101,  49, 125 }, COL_D_GRY, false, L"Exit"           , { 11,  4 } );
  } break;
  case 3: {
    obj[0] = wnd_menu_create( {  49,  49, 150,  76 }, COL_L_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( {  50,  50, 149,  75 }, COL_D_GRY, false, L"Macro     >"    , { 28,  4 } );
    obj[2] = wnd_menu_create( {  49,  75, 150, 101 }, COL_L_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( {  50,  76, 149, 100 }, COL_D_GRY, false, L"Multi-Cursor"   , { 10,  4 } );
    obj[4] = wnd_menu_create( {  49, 100, 150, 126 }, COL_L_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( {  50, 101, 149, 125 }, COL_D_GRY, false, L"Plugins(?)"     , { 16,  4 } );
  } break;
  case 5: {
    obj[0] = wnd_menu_create( { 101,  49, 216,  76 }, COL_L_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( { 102,  50, 215,  75 }, COL_D_GRY, false, L"Hide FPS"       , { 26,  4 } );
    obj[2] = wnd_menu_create( { 101,  75, 216, 101 }, COL_L_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( { 102,  76, 215, 100 }, COL_D_GRY, false, L"Hide Caret Pos" , {  8,  4 } );
    obj[4] = wnd_menu_create( { 101, 100, 216, 126 }, COL_L_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( { 102, 101, 215, 125 }, COL_D_GRY, false, L"Hide Line Count", {  5,  4 } );
  } break;
  default:
    return;
  }

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < 6; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );  // add hovering

    TextOutW( hdc,
      obj[ idx ].r.left + obj[ idx ].offset.x,
      obj[ idx ].r.top  + obj[ idx ].offset.y,
      obj[ idx ].txt,
      lstrlenW( obj[ idx ].txt )
    );

  }

  ReleaseDC( hwnd, hdc );

  for( auto& it : obj )
    DeleteObject( obj[ idx ].col );
}

void wnd_menu_clicked( HWND hwnd, POINT m_pos ) {
  s8 menu_clicked = -1;
  for( s8 idx = 1; idx < OBJ_BASE_COUNT; idx+=2 ) {
    if( !m_base_hovered[ idx ] )
      continue;

    menu_clicked = idx;
    break;
  }

  if( menu_clicked == -1 )
    return;

  wnd_menu_draw_dropdown( hwnd, menu_clicked );
}