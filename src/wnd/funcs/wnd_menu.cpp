#include "wnd_menu.h"

void wnd_menu_draw( HDC hdc, RECT wnd_sz ) {
  SIZE txt_sz[ OBJ_BASE_COUNT ] = {};

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( {  -1, ( WND_BTN_SZ - 1 ),  50, 50 }, COL_M_GRY, true , L""      ),
    wnd_menu_create( {   0,   WND_BTN_SZ      ,  49, 49 }, COL_D_GRY, false, L"File"  ),
    wnd_menu_create( {  50, ( WND_BTN_SZ - 1 ), 102, 50 }, COL_M_GRY, true , L""      ),
    wnd_menu_create( {  50,   WND_BTN_SZ      , 101, 49 }, COL_D_GRY, false, L"Tools" ),
    wnd_menu_create( { 102, ( WND_BTN_SZ - 1 ), 152, 50 }, COL_M_GRY, true , L""      ),
    wnd_menu_create( { 102,   WND_BTN_SZ      , 151, 49 }, COL_D_GRY, false, L"Style" )
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    if( idx % 2 != 0 )
      GetTextExtentPoint32W( hdc, obj[ idx ].txt, lstrlenW( obj[ idx ].txt ), &txt_sz[ idx ] );

    TextOutW( hdc,
      obj[ idx ].r.left + ( ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 ) - ( txt_sz[ idx ].cx / 2 ),
      obj[ idx ].r.top  + ( ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 ) - ( txt_sz[ idx ].cy / 2 ),
      obj[ idx ].txt,
      lstrlenW( obj[ idx ].txt )
    );

    s32 underline_pos = 0;

    switch( idx ) {
    case 1: {
      underline_pos =  11;
    } break;
    case 3: {
      underline_pos =  58;
    } break;
    case 5: {
      underline_pos = 110;
    } break;
    default:
      break;
    }

    for( s32 u_idx = 0; u_idx < 8; u_idx++ )
      SetPixel( hdc, underline_pos + u_idx, 44, COL_M_GRY );
  }

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_menu_draw_dropdown( HWND hwnd, s8 idx ) {
  wnd_clear_menus( idx, false );

  RECT menu_rect;

  switch( idx ) {
  case 0: {
    menu_rect = {   0, 49,  50, 126 };
  } break;
  case 1: {
    menu_rect = {  49, 49, 152, 126 };
  } break;
  case 2: {
    menu_rect = { 101, 49, 216, 126 };
  } break;
  default:
    return;
  }

  ValidateRect( h_global, &menu_rect );

  HDC hdc = GetDC( hwnd );

  WND_MENU obj[ OBJ_BASE_COUNT ] = {};
  VEC3 underline_dim[ OBJ_BASE_COUNT / 2 ] = {};
  SIZE txt_sz[ OBJ_BASE_COUNT ] = {};

  switch( idx ) {
  case 0: {
    obj[0] = wnd_menu_create( {   0,  49,  50,  76 }, COL_M_GRY, true , L""               );
    obj[1] = wnd_menu_create( {   0,  50,  49,  75 }, COL_D_GRY, false, L"New"            );
    underline_dim[0] = { 10, 69, 9 };

    obj[2] = wnd_menu_create( {   0,  75,  50, 101 }, COL_M_GRY, true , L""               );
    obj[3] = wnd_menu_create( {   0,  76,  49, 100 }, COL_D_GRY, false, L"Open"           );
    underline_dim[1] = { 8, 95, 9 };

    obj[4] = wnd_menu_create( {   0, 100,  50, 126 }, COL_M_GRY, true , L""               );
    obj[5] = wnd_menu_create( {   0, 101,  49, 125 }, COL_D_GRY, false, L"Exit"           );
    underline_dim[2] = { 11, 120, 9 };
  } break;
  case 1: {
    obj[0] = wnd_menu_create( {  49,  49, 152,  76 }, COL_M_GRY, true , L""               );
    obj[1] = wnd_menu_create( {  50,  50, 151,  75 }, COL_D_GRY, false, L"Macro"    );
    underline_dim[0] = { 78, 69, 11 };

    obj[2] = wnd_menu_create( {  49,  75, 152, 101 }, COL_M_GRY, true , L""               );
    obj[3] = wnd_menu_create( {  50,  76, 151, 100 }, COL_D_GRY, false, L"Multi-Cursor"   );
    underline_dim[1] = { 96, 95, 9 };

    obj[4] = wnd_menu_create( {  49, 100, 152, 126 }, COL_M_GRY, true , L""               );
    obj[5] = wnd_menu_create( {  50, 101, 151, 125 }, COL_D_GRY, false, L"Plugins(?)"     );
    underline_dim[2] = { 65, 120, 9 };
  } break;
  case 2: {
    obj[0] = wnd_menu_create( { 101,  49, 216,  76 }, COL_M_GRY, true , L""               );
    obj[1] = wnd_menu_create( { 102,  50, 215,  75 }, COL_D_GRY, false, L"Hide FPS"       );
    underline_dim[0] = { 161, 69, 9 };

    obj[2] = wnd_menu_create( { 101,  75, 216, 101 }, COL_M_GRY, true , L""               );
    obj[3] = wnd_menu_create( { 102,  76, 215, 100 }, COL_D_GRY, false, L"Hide Caret Pos" );
    underline_dim[1] = { 110, 95, 9 };

    obj[4] = wnd_menu_create( { 101, 100, 216, 126 }, COL_M_GRY, true , L""                );
    obj[5] = wnd_menu_create( { 102, 101, 215, 125 }, COL_D_GRY, false, L"Hide Line Count" );
    underline_dim[2] = { 107, 120, 9 };
  } break;
  default:
    return;
  }

  SetBkMode( hdc, TRANSPARENT );

  for( s8 m_idx = 0; m_idx < OBJ_BASE_COUNT; ++m_idx ) {
    FillRect( hdc, &obj[ m_idx ].r, obj[ m_idx ].col );
    if( obj[ m_idx ].next ) {
      ++m_idx;
      FillRect( hdc, &obj[ m_idx ].r, obj[ m_idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    if( m_idx % 2 != 0 )
      GetTextExtentPoint32W( hdc, obj[ m_idx ].txt, lstrlenW( obj[ m_idx ].txt ), &txt_sz[ m_idx ] );

    TextOutW( hdc,
      obj[ m_idx ].r.left + ( ( obj[ m_idx ].r.right - obj[ m_idx ].r.left ) / 2 ) - ( txt_sz[ m_idx ].cx / 2 ),
      obj[ m_idx ].r.top  + ( ( obj[ m_idx ].r.bottom - obj[ m_idx ].r.top ) / 2 ) - ( txt_sz[ m_idx ].cy / 2 ),
      obj[ m_idx ].txt,
      lstrlenW( obj[ m_idx ].txt )
    );

    if( idx == 1 && m_idx == 1 ) {
      SIZE lt_sz{};
      GetTextExtentPoint32W( hdc, L">", 1, &lt_sz );

      TextOutW( hdc,
        obj[ m_idx ].r.right - 15,
        obj[ m_idx ].r.top  + ( ( obj[ m_idx ].r.bottom - obj[ m_idx ].r.top ) / 2 ) - ( lt_sz.cy / 2 ),
        L">", 1
      );
    }
  }

  for( s32 u_idx = 0; u_idx < OBJ_BASE_COUNT / 2; u_idx++ ) {
    for( s32 uw_idx = 0; uw_idx < underline_dim[ u_idx ].w; uw_idx++ ) {
      SetPixel( hdc,
        underline_dim[ u_idx ].x + uw_idx,
        underline_dim[ u_idx ].y,
        COL_M_GRY
      );
    }  
  }

  ReleaseDC( hwnd, hdc );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx )
    DeleteObject( obj[ idx ].col );
}

void wnd_menu_draw_sub_dropdown( HWND hwnd ) {
  RECT menu_rect = { 151, 49, 233, 130 };
  ValidateRect( hwnd, &menu_rect );

  HDC hdc = GetDC( hwnd );

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( { 151,  49, 233,  76 }, COL_M_GRY, true , L""         ),
    wnd_menu_create( { 152,  50, 232,  75 }, COL_D_GRY, false, L"Clear"    ),
    wnd_menu_create( { 151,  76, 233, 103 }, COL_M_GRY, true , L""         ),
    wnd_menu_create( { 152,  77, 232, 102 }, COL_D_GRY, false, L"Record"   ),
    wnd_menu_create( { 151, 103, 233, 130 }, COL_M_GRY, true , L""         ),
    wnd_menu_create( { 152, 104, 232, 129 }, COL_D_GRY, false, L"Playback" )
  };
  SIZE txt_sz[ OBJ_BASE_COUNT ];
  VEC3 underline_dim[ OBJ_BASE_COUNT / 2 ] = {
    { 175,  69,  9 },
    { 169,  96, 10 },
    { 162, 123,  8 }
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    GetTextExtentPoint32W( hdc, obj[ idx ].txt, lstrlenW( obj[ idx ].txt ), &txt_sz[ idx ] );

    TextOutW( hdc,
      obj[ idx ].r.left + ( ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 ) - ( txt_sz[ idx ].cx / 2 ),
      obj[ idx ].r.top  + ( ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 ) - ( txt_sz[ idx ].cy / 2 ),
      obj[ idx ].txt,
      lstrlenW( obj[ idx ].txt )
    );
  }

  for( s32 u_idx = 0; u_idx < OBJ_BASE_COUNT / 2; u_idx++ ) {
    for( s32 uw_idx = 0; uw_idx < underline_dim[ u_idx ].w; uw_idx++ ) {
      SetPixel( hdc,
        underline_dim[ u_idx ].x + uw_idx,
        underline_dim[ u_idx ].y,
        COL_M_GRY
      );
    }  
  }

  ReleaseDC( hwnd, hdc );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx )
    DeleteObject( obj[ idx ].col );
}