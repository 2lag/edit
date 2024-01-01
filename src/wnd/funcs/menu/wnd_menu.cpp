#include "wnd_menu.h"

void wnd_menu_draw( const HDC hdc ) {
  SIZE txt_sz[ OBJ_BASE_COUNT ] = {};

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( {                     -1, ( WND_BTN_SZ       - 1 ),
                       ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 2 )     }, COL_M_GRY, true , ""      ),
    wnd_menu_create( {                      0,   WND_BTN_SZ            ,
                       ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 2 ) - 1 }, COL_D_GRY, false, "File"  ),
    wnd_menu_create( { ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ       - 1 ),
                       ( WND_BTN_SZ * 4 ) + 2, ( WND_BTN_SZ * 2 )     }, COL_M_GRY, true , ""      ),
    wnd_menu_create( { ( WND_BTN_SZ * 2 )    ,   WND_BTN_SZ            ,
                       ( WND_BTN_SZ * 4 ) + 1, ( WND_BTN_SZ * 2 ) - 1 }, COL_D_GRY, false, "Tools" ),
    wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 2, ( WND_BTN_SZ       - 1 ),
                       ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 2 )     }, COL_M_GRY, true , ""      ),
    wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 2,   WND_BTN_SZ            ,
                       ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 2 ) - 1 }, COL_D_GRY, false, "Style" )
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
      GetTextExtentPoint32A( hdc, obj[ idx ].txt, strlen( obj[ idx ].txt ), &txt_sz[ idx ] );

    TextOutA( hdc,
      obj[ idx ].r.left + ( ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 ) - ( txt_sz[ idx ].cx / 2 ),
      obj[ idx ].r.top  + ( ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 ) - ( txt_sz[ idx ].cy / 2 ),
      obj[ idx ].txt, strlen( obj[ idx ].txt )
    );

    s32 underline_pos = 0;

    switch( idx ) {
    case 1: {
      underline_pos =  10;
    } break;
    case 3: {
      underline_pos =  56;
    } break;
    case 5: {
      underline_pos = 109;
    } break;
    default:
      break;
    }

    for( s32 u_idx = 0; u_idx < 10; u_idx++ )
      SetPixel( hdc, underline_pos + u_idx, ( WND_BTN_SZ * 2 ) - 6, COL_M_GRY );
  }

  for( auto& it : obj )
    DeleteObject( it.col );
}

void wnd_menu_draw_dropdown( const HWND hwnd, const s8 idx ) {
  wnd_clear_menus( idx, false );

  RECT menu_rect;

  switch( idx ) {
  case 0: {
    menu_rect = {
      0,
      ( WND_BTN_SZ * 2 ) - 1,
      ( WND_BTN_SZ * 2 ),
      ( WND_BTN_SZ * 5 ) + 1
    };
  } break;
  case 1: {
    menu_rect = {
      ( WND_BTN_SZ * 2 ) - 1,
      ( WND_BTN_SZ * 2 ) - 1,
      ( WND_BTN_SZ * 6 ) + 2,
      ( WND_BTN_SZ * 5 ) + 1
    };
  } break;
  case 2: {
    menu_rect = {
      ( WND_BTN_SZ * 4 ) +  1,
      ( WND_BTN_SZ * 2 ) -  1,
      ( WND_BTN_SZ * 9 ),
      ( WND_BTN_SZ * 5 ) +  1
    };
  } break;
  default:
    return;
  }

  ValidateRect( h_global, &menu_rect );

  HDC hdc = GetDC( hwnd );
  WND_MENU obj[ OBJ_BASE_COUNT ] = {};
  VEC3 underline_dim[ OBJ_BASE_COUNT / 2 ] = {
    { 0, ( WND_BTN_SZ * 3 ) - 5, 9 },
    { 0, ( WND_BTN_SZ * 4 ) - 5, 9 },
    { 0, ( WND_BTN_SZ * 5 ) - 5, 9 }
  };
  SIZE txt_sz[ OBJ_BASE_COUNT ] = {};

  switch( idx ) {
  case 0: {
    obj[0] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 2 ) - 1  ,
                                ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 3 ) + 1 }, COL_M_GRY, true , ""               );
    obj[1] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 2 )      ,
                                ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 3 )     }, COL_D_GRY, false, "New"            );
    obj[2] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 3 )      ,
                                ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 4 ) + 1 }, COL_M_GRY, true , ""               );
    obj[3] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 3 ) + 1  ,
                                ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 4 )     }, COL_D_GRY, false, "Open"           );
    obj[4] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 4 )      ,
                                ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 5 ) + 1 }, COL_M_GRY, true , ""               );
    obj[5] = wnd_menu_create( {                      0, ( WND_BTN_SZ * 4 ) + 1  ,
                                ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 5 )     }, COL_D_GRY, false, "Save"           );
    
    underline_dim[0].x = 10;
    underline_dim[1].x = 7;
    underline_dim[2].x = 7,
    underline_dim[2].w = 10;
  } break;
  case 1: {
    obj[0] = wnd_menu_create( { ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 2 ) - 1  ,
                                ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 3 ) + 1 }, COL_M_GRY, true , ""               );
    obj[1] = wnd_menu_create( { ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 2 )      ,
                                ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 3 )     }, COL_D_GRY, false, "Macro"          );
    obj[2] = wnd_menu_create( { ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 3 )      ,
                                ( WND_BTN_SZ * 2 ) + 2, ( WND_BTN_SZ * 4 ) + 1 }, COL_M_GRY, true , ""               );
    obj[3] = wnd_menu_create( { ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 3 ) + 1  ,
                                ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 4 )     }, COL_D_GRY, false, "Multi-Cursor"   );
    obj[4] = wnd_menu_create( { ( WND_BTN_SZ * 2 ) - 1, ( WND_BTN_SZ * 4 )      ,
                                ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 5 ) + 1 }, COL_M_GRY, true , ""               );
    obj[5] = wnd_menu_create( { ( WND_BTN_SZ * 2 )    , ( WND_BTN_SZ * 4 ) + 1  ,
                                ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 5 )     }, COL_D_GRY, false, "Plugins(?)"     );

    underline_dim[0].x = 80,
    underline_dim[0].w = 11;
    underline_dim[1].x = 97;
    underline_dim[2].x = 67;
  } break;
  case 2: {
    obj[0] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 1, ( WND_BTN_SZ * 2 ) - 1  ,
                                ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 3 ) + 1 }, COL_M_GRY, true , ""               );
    obj[1] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 2, ( WND_BTN_SZ * 2 )      ,
                                ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 3 )     }, COL_D_GRY, false, menu_style_toggle[ FPS ]        ? "[x] FPS"        : "[  ] FPS"         );
    obj[2] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 1, ( WND_BTN_SZ * 3 )      ,
                                ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 4 ) + 1 }, COL_M_GRY, true , ""               );
    obj[3] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 2, ( WND_BTN_SZ * 3 ) + 1  ,
                                ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 4 )     }, COL_D_GRY, false, menu_style_toggle[ CARET ]      ? "[x] Caret Pos"  : "[  ] Caret Pos"   );
    obj[4] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 1, ( WND_BTN_SZ * 4 )      ,
                                ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 5 ) + 1 }, COL_M_GRY, true , ""                );
    obj[5] = wnd_menu_create( { ( WND_BTN_SZ * 4 ) + 2, ( WND_BTN_SZ * 4 ) + 1  ,
                                ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 5 )     }, COL_D_GRY, false, menu_style_toggle[ LINE_COUNT ] ? "[x] Line Count" : "[  ] Line Count"  );

    underline_dim[0].x = 160;
    underline_dim[1].x = 180;
    underline_dim[2].x = 138,
    underline_dim[2].w = 10;
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
      GetTextExtentPoint32A( hdc, obj[ m_idx ].txt, strlen( obj[ m_idx ].txt ), &txt_sz[ m_idx ] );

    TextOutA( hdc,
      obj[ m_idx ].r.left + ( ( obj[ m_idx ].r.right - obj[ m_idx ].r.left ) / 2 ) - ( txt_sz[ m_idx ].cx / 2 ),
      obj[ m_idx ].r.top  + ( ( obj[ m_idx ].r.bottom - obj[ m_idx ].r.top ) / 2 ) - ( txt_sz[ m_idx ].cy / 2 ),
      obj[ m_idx ].txt, strlen( obj[ m_idx ].txt )
    );

    if( idx == 1 && m_idx == 1 ) {
      SIZE lt_sz{};
      GetTextExtentPoint32A( hdc, ">", 1, &lt_sz );

      TextOutA( hdc,
        obj[ m_idx ].r.right - 15,
        obj[ m_idx ].r.top  + ( ( obj[ m_idx ].r.bottom - obj[ m_idx ].r.top ) / 2 ) - ( lt_sz.cy / 2 ),
        ">", 1
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

  for( s8 o_idx = 0; o_idx < OBJ_BASE_COUNT; ++o_idx )
    DeleteObject( obj[ o_idx ].col );
}

void wnd_menu_draw_sub_dropdown( const HWND hwnd ) {
  RECT menu_rect = {
    ( WND_BTN_SZ * 6 ) + 1,
    ( WND_BTN_SZ * 2 ) - 1,
    ( WND_BTN_SZ * 9 )    ,
    ( WND_BTN_SZ * 5 ) + 1
  };
  ValidateRect( hwnd, &menu_rect );

  HDC hdc = GetDC( hwnd );

  WND_MENU obj[ OBJ_BASE_COUNT ] = {
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 2 ) - 1  ,
                       ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 3 ) + 1 }, COL_M_GRY, true , ""         ),
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 2 )      ,
                       ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 3 )     }, COL_D_GRY, false, "Clear"    ),
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 3 )      ,
                       ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 4 ) + 1 }, COL_M_GRY, true , ""         ),
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 3 ) + 1  ,
                       ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 4 )     }, COL_D_GRY, false, "Record"   ),
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 1, ( WND_BTN_SZ * 4 )      ,
                       ( WND_BTN_SZ * 9 )    , ( WND_BTN_SZ * 5 ) + 1 }, COL_M_GRY, true , ""         ),
    wnd_menu_create( { ( WND_BTN_SZ * 6 ) + 2, ( WND_BTN_SZ * 4 ) + 1  ,
                       ( WND_BTN_SZ * 9 ) - 1, ( WND_BTN_SZ * 5 )     }, COL_D_GRY, false, "Playback" )
  };
  SIZE txt_sz[ OBJ_BASE_COUNT ];
  VEC3 underline_dim[ OBJ_BASE_COUNT / 2 ] = {
    { 171, ( WND_BTN_SZ * 3 ) - 5,  9 },
    { 165, ( WND_BTN_SZ * 4 ) - 5, 10 },
    { 159, ( WND_BTN_SZ * 5 ) - 5,  8 }
  };

  SetBkMode( hdc, TRANSPARENT );

  for( s8 idx = 0; idx < OBJ_BASE_COUNT; ++idx ) {
    FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    if( obj[ idx ].next ) {
      ++idx;
      FillRect( hdc, &obj[ idx ].r, obj[ idx ].col );
    }

    SetTextColor( hdc, COL_M_GRY );

    GetTextExtentPoint32A( hdc, obj[ idx ].txt, strlen( obj[ idx ].txt ), &txt_sz[ idx ] );

    TextOutA( hdc,
      obj[ idx ].r.left + ( ( obj[ idx ].r.right - obj[ idx ].r.left ) / 2 ) - ( txt_sz[ idx ].cx / 2 ),
      obj[ idx ].r.top  + ( ( obj[ idx ].r.bottom - obj[ idx ].r.top ) / 2 ) - ( txt_sz[ idx ].cy / 2 ),
      obj[ idx ].txt, strlen( obj[ idx ].txt )
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