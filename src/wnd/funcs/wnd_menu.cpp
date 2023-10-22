#include "wnd_menu.h"

#define OBJ_COUNT 6

const RECT o_menu_file  {  -1, 24,  50, 50 },
           i_menu_file  {   0, 25,  49, 49 },
           o_menu_tools {  50, 24, 102, 50 },
           i_menu_tools {  50, 25, 101, 49 },
           o_menu_style { 102, 24, 152, 50 },
           i_menu_style { 102, 25, 151, 49 };

bool m_hovered[ OBJ_COUNT ] = { false };

void wnd_menu_draw( HDC hdc, RECT wnd_sz ) {
  WND_MENU obj[ OBJ_COUNT ] = {
    wnd_menu_create( o_menu_file , COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_file , COL_D_GRY, false, L"File" , { 12,  4 } ),
    wnd_menu_create( o_menu_tools, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_tools, COL_D_GRY, false, L"Tools", {  8,  4 } ),
    wnd_menu_create( o_menu_style, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( i_menu_style, COL_D_GRY, false, L"Style", {  8,  4 } )
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    if( m_hovered[ idx ] )
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

  static bool was_hovered[ OBJ_COUNT ] = { false };

  m_hovered[ 1 ] = (bool)PtInRect( &o_menu_file , cm_pos );
  m_hovered[ 3 ] = (bool)PtInRect( &o_menu_tools, cm_pos );
  m_hovered[ 5 ] = (bool)PtInRect( &o_menu_style, cm_pos );

  for( s32 idx = 0; idx < OBJ_COUNT; ++idx ) {
    if( m_hovered[ idx ] == was_hovered[ idx ] )
      continue;
    
    was_hovered[ idx ] = m_hovered[ idx ];

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

void wnd_menu_clicked( POINT m_pos ) {
  // use hovered to see which one is clicked to activate a dropdown
}