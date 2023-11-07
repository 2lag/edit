#include "wnd_menu.h"

void wnd_menu_draw( HDC hdc, RECT wnd_sz ) {
  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( {  -1, 24,  50, 50 }, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( {   0, 25,  49, 49 }, COL_D_GRY, false, L"File" , { 12,  4 } ),
    wnd_menu_create( {  50, 24, 102, 50 }, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( {  50, 25, 101, 49 }, COL_D_GRY, false, L"Tools", {  8,  4 } ),
    wnd_menu_create( { 102, 24, 152, 50 }, COL_M_GRY, true , L""     , {        } ),
    wnd_menu_create( { 102, 25, 151, 49 }, COL_D_GRY, false, L"Style", {  8,  4 } )
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    TextOutW( hdc,
      obj[ idx ].r.left + obj[ idx ].offset.x,
      obj[ idx ].r.top  + obj[ idx ].offset.y,
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

  WND_MENU obj[ OBJ_BASE_COUNT ];
  VEC3 underline_dim[ OBJ_BASE_COUNT / 2 ];

  switch( idx ) {
  case 0: {
    obj[0] = wnd_menu_create( {   0,  49,  50,  76 }, COL_M_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( {   0,  50,  49,  75 }, COL_D_GRY, false, L"New"            , { 10,  4 } );
    underline_dim[0] = { 10, 69, 9 };

    obj[2] = wnd_menu_create( {   0,  75,  50, 101 }, COL_M_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( {   0,  76,  49, 100 }, COL_D_GRY, false, L"Open"           , {  8,  4 } );
    underline_dim[1] = { 8, 95, 9 };

    obj[4] = wnd_menu_create( {   0, 100,  50, 126 }, COL_M_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( {   0, 101,  49, 125 }, COL_D_GRY, false, L"Exit"           , { 11,  4 } );
    underline_dim[2] = { 11, 120, 9 };
  } break;
  case 1: {
    obj[0] = wnd_menu_create( {  49,  49, 152,  76 }, COL_M_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( {  50,  50, 151,  75 }, COL_D_GRY, false, L"Macro     >"    , { 28,  4 } );
    underline_dim[0] = { 78, 69, 11 };

    obj[2] = wnd_menu_create( {  49,  75, 152, 101 }, COL_M_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( {  50,  76, 151, 100 }, COL_D_GRY, false, L"Multi-Cursor"   , { 10,  4 } );
    underline_dim[1] = { 96, 95, 9 };

    obj[4] = wnd_menu_create( {  49, 100, 152, 126 }, COL_M_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( {  50, 101, 151, 125 }, COL_D_GRY, false, L"Plugins(?)"     , { 16,  4 } );
    underline_dim[2] = { 65, 120, 9 };
  } break;
  case 2: {
    obj[0] = wnd_menu_create( { 101,  49, 216,  76 }, COL_M_GRY, true , L""               , {        } );
    obj[1] = wnd_menu_create( { 102,  50, 215,  75 }, COL_D_GRY, false, L"Hide FPS"       , { 26,  4 } );
    underline_dim[0] = { 161, 69, 9 };

    obj[2] = wnd_menu_create( { 101,  75, 216, 101 }, COL_M_GRY, true , L""               , {        } );
    obj[3] = wnd_menu_create( { 102,  76, 215, 100 }, COL_D_GRY, false, L"Hide Caret Pos" , {  8,  4 } );
    underline_dim[1] = { 110, 95, 9 };

    obj[4] = wnd_menu_create( { 101, 100, 216, 126 }, COL_M_GRY, true , L""               , {        } );
    obj[5] = wnd_menu_create( { 102, 101, 215, 125 }, COL_D_GRY, false, L"Hide Line Count", {  5,  4 } );
    underline_dim[2] = { 107, 120, 9 };
  } break;
  default:
    return;
  }

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    TextOutW( hdc,
      obj[ idx ].r.left + obj[ idx ].offset.x,
      obj[ idx ].r.top  + obj[ idx ].offset.y,
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

void wnd_menu_draw_sub_dropdown( HWND hwnd ) {
  RECT menu_rect = { 151, 49, 233, 130 };
  ValidateRect( hwnd, &menu_rect );

  HDC hdc = GetDC( hwnd );

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( { 151,  49, 233,  76 }, COL_M_GRY, true , L""        , {       } ),
    wnd_menu_create( { 152,  50, 232,  75 }, COL_D_GRY, false, L"Clear"   , { 23, 4 } ),
    wnd_menu_create( { 151,  76, 233, 103 }, COL_M_GRY, true , L""        , {       } ),
    wnd_menu_create( { 152,  77, 232, 102 }, COL_D_GRY, false, L"Record"  , { 17, 4 } ),
    wnd_menu_create( { 151, 103, 233, 130 }, COL_M_GRY, true , L""        , {       } ),
    wnd_menu_create( { 152, 104, 232, 129 }, COL_D_GRY, false, L"Playback", { 10, 4 } )
  };

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

    TextOutW( hdc,
      obj[ idx ].r.left + obj[ idx ].offset.x,
      obj[ idx ].r.top  + obj[ idx ].offset.y,
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