#include "menu.h"

bool macro_recording = false;

// i will never use this again
using namespace std;

void wnd_menu_create_bar( const HDC hdc,
                          const vector<s32> u_pos,
                          const vector<const char*> txt ) {
  HBRUSH bg_brush = CreateSolidBrush( COL_M_GRY );
  HBRUSH brush = CreateSolidBrush( COL_D_GRY );
  RECT menu_rect = {
    -1, WND_BTN_SZ,
    WND_BTN_SZ * 2 * static_cast<salt> ( txt.size() ),
    WND_BTN_SZ * 2
  };
  
  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );
  
  FillRect( hdc, &menu_rect, bg_brush );
  
  vector<SIZE> txt_sz( txt.size() );
  for( s8 idx = 0; idx < txt.size(); ++idx ) {
    RECT item_rect = {
      WND_BTN_SZ * 2 * idx,
      WND_BTN_SZ,
      ( WND_BTN_SZ * 2 * ( idx + 1 ) ) - 1,
      ( WND_BTN_SZ * 2 ) - 1
    };
    FillRect( hdc, &item_rect, brush );

    GetTextExtentPoint32A( hdc,
      txt[ idx ],
      static_cast<s32>( strlen( txt[ idx ] ) ),
      &txt_sz[ idx ]
    );

    TextOutA( hdc,
      hcenter_text( item_rect, txt_sz[ idx ] ),
      vcenter_text( item_rect, txt_sz[ idx ] ),
      txt[ idx ], static_cast<s32>( strlen( txt[ idx ] ) )
    );

    for( s32 u_idx = 0; u_idx < 10; u_idx++ ) {
      SetPixel( hdc, u_pos[ idx ] + u_idx,
        ( WND_BTN_SZ * 2 ) - 6, COL_M_GRY
      );
    }
  }

  DeleteObject( brush );
  DeleteObject( bg_brush );
}

void wnd_menu_draw( const HDC hdc ) {
  vector<const char*> txt{ "File", "Tools", "Style" };
  vector<s32> underline_pos{ 10, 55, 107 };
  wnd_menu_create_bar( hdc,
    underline_pos, txt
  );
}

RECT wnd_menu_create_vert( const HDC hdc,
                           const s32 width,
                           const POINT start,
                           const vector<s32> u_pos,
                           const vector<const char*> txt ) {
  HBRUSH brush = CreateSolidBrush( COL_D_GRY );
  HBRUSH bg_brush = CreateSolidBrush( COL_M_GRY );
  RECT menu_rect = {
    start.x,
    start.y - 1,
    start.x + width,
    start.y + ( WND_BTN_SZ * static_cast<salt>( txt.size() ) )
  };

  SetBkMode( hdc, TRANSPARENT );
  SetTextColor( hdc, COL_M_GRY );

  FillRect( hdc, &menu_rect, bg_brush );

  vector<SIZE> txt_sz( txt.size() );
  vector<s32> underline_dim( txt.size() );
  for( s8 idx = 0; idx < txt.size(); ++idx ) {
    underline_dim[ idx ] = start.y + ( WND_BTN_SZ * ( idx + 1 ) ) - 5;

    RECT item_rect {
      ( start.x + 1 ),
      ( start.y ) + ( WND_BTN_SZ * idx ),
      ( start.x + width - 1 ),
      ( start.y - 1 ) + ( WND_BTN_SZ * ( idx + 1 ) )
    };
    FillRect( hdc, &item_rect, brush );

    GetTextExtentPoint32A( hdc,
      txt[ idx ],
      static_cast<s32>( strlen( txt[ idx ] ) ),
      &txt_sz[ idx ]
    );

    TextOutA( hdc,
      hcenter_text( item_rect, txt_sz[ idx ] ),
      vcenter_text( item_rect, txt_sz[ idx ] ),
      txt[ idx ], static_cast<s32>( strlen( txt[ idx ] ) )
    );

    if( !strncmp( txt[ idx ], "Macro", 5 ) ) {
      SIZE lt_sz{};
      GetTextExtentPoint32A( hdc, ">", 1, &lt_sz );

      TextOutA( hdc,
        item_rect.right - 15,
        vcenter_text( item_rect, lt_sz ),
        ">", 1
      );
    }
  }

  for( s32 u_idx = 0; u_idx < underline_dim.size(); u_idx++ ) {
    for( s32 uw_idx = 0; uw_idx < 9; uw_idx++ ) {
      SetPixel( hdc,
        u_pos[ u_idx ] + uw_idx,
        underline_dim[ u_idx ],
        COL_M_GRY
      );
    }  
  }

  ValidateRect( h_global, &menu_rect );

  DeleteObject( bg_brush );
  DeleteObject( brush );
  return menu_rect;
}

// remove menu_rect from here ?
void wnd_menu_draw_dropdown( const HWND hwnd, const s8 idx ) {
  wnd_clear_menus( idx, false );

  HDC hdc = GetDC( hwnd );

  RECT menu_rect{};
  vector<s32> underline_pos;

  switch( idx ) { // "Anotha one" - DJ Khaled, 2015 ( sin #1 ) partial fix 1.4.24
  case 0: {
    underline_pos.resize(4);
    underline_pos[0] = 10;
    underline_pos[1] = 7;
    underline_pos[2] = 8;
    underline_pos[3] = 12;

    vector<const char*> txt{ "New", "Open", "Save", "Exit" };
    menu_rect = wnd_menu_create_vert( hdc,
      WND_BTN_SZ * 2 + 1, { -1, WND_BTN_SZ * 2 },
      underline_pos, txt
    );
  } break;
  case 1: {
    underline_pos.resize(2);
    underline_pos[0] = 85;
    underline_pos[1] = 81;

    vector<const char*> txt{ "Find", "Macro" };
    menu_rect = wnd_menu_create_vert( hdc,
      WND_BTN_SZ * 4, { WND_BTN_SZ * 2, WND_BTN_SZ * 2 },
      underline_pos, txt
    );    
  } break;
  case 2: {
    underline_pos.resize(3);
    underline_pos[0] = 146;
    underline_pos[1] = 167;
    underline_pos[2] = 126;

    vector<const char*> txt{
      menu_style_toggle[ FPS ]        ? "[x] FPS"        : "[  ] FPS",
      menu_style_toggle[ CARET ]      ? "[x] Caret Pos"  : "[  ] Caret Pos",
      menu_style_toggle[ LINE_COUNT ] ? "[x] Line Count" : "[  ] Line Count"
    };
    menu_rect = wnd_menu_create_vert( hdc,
      WND_BTN_SZ * 4 + 1, { WND_BTN_SZ * 4 , WND_BTN_SZ * 2 },
      underline_pos, txt
    );
  } break;
  default:
    break;
  }

  ReleaseDC( hwnd, hdc );
}

void wnd_menu_draw_sub_dropdown( const HWND hwnd ) {

  HDC hdc = GetDC( hwnd );

  vector<const char*> txt{
    macro_recording ? "[X] Record" : "[  ] Record",
    "Playback"
  };
  vector<s32> underline_pos{ 174, 157 };
  (void)wnd_menu_create_vert( hdc,
    WND_BTN_SZ * 3, { WND_BTN_SZ * 6 - 1, WND_BTN_SZ * 3 },
    underline_pos, txt
  );

  ReleaseDC( hwnd, hdc );
}