#pragma once

class CSCROLL {
public:
  s32 curr_line;
  s32 line_count;
  s32 line_first;
  s32 line_last;
  s32 lines_vis;
  SIZE line_sz;
  s32 scroll_h;
  s32 scroll_y;
  s32 m_delta;
public:
  HWND parent;
  RECT bkrect;
  RECT txt_rect;
  RECT rect;
  bool hovered;
public:
  bool dragging;
  POINT duser_start;
  bool mdragging;
  POINT mduser_start;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;
    txt_rect = get_wnd_sz( parent );
    bkrect = rect = {
      txt_rect.right + 1,
      txt_rect.top - 1,
      txt_rect.right + 25,
      txt_rect.bottom + 1
    };

    cscroll_draw();
  }
  void cscroll_draw( bool update_info = true, bool redraw = false ) {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush( hovered ? COL_L_GRY : COL_M_GRY );

    GetTextExtentPoint32W( hdc, L"A", 1, &line_sz );
    cscroll_setinfo( update_info, redraw );

    FillRect( hdc, &bkrect, dbrush );
    FillRect( hdc, &rect  , lbrush );
    
    DeleteObject( dbrush );
    DeleteObject( lbrush );
    ReleaseDC( parent, hdc );
  }
public:
  void cscroll_drag_off() {
    if( dragging ) {
      cscroll_setinfo( true, true );
      dragging = false;
      m_delta = 0;
    }
  }
  void cscroll_drag_on( POINT m_pos ) {
    if( cscroll_ishovered( m_pos ) ) {
      duser_start = m_pos;
      dragging = true;
    }
  }
  void cscroll_drag( POINT m_pos ) {
    if( !GetAsyncKeyState( MK_LBUTTON ) || !dragging ) {
      duser_start = m_pos;
      dragging = false;
      return;
    }

    m_delta = m_pos.y - duser_start.y;

    s32 scroll_dir;
    if( m_delta <= -bkrect.bottom / line_count )
      scroll_dir = -1;
    else if( m_delta >= bkrect.bottom / line_count )
      scroll_dir = 1;
    else
      scroll_dir = 0;

    if( scroll_dir ) {
      m_delta = 0;
      duser_start = m_pos;
      curr_line += scroll_dir;
      line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;
      
      if( scroll_dir == 1 && curr_line <= line_last ) {
        if( curr_line == line_last )
          curr_line = line_first + lines_vis - 1;
        else
          curr_line++;
      } else if( scroll_dir == -1 && curr_line > 1 ) {
        if( curr_line >= lines_vis ) {
          SendMessageW( parent, EM_SCROLL, SB_LINEUP, 0 );
          curr_line = line_first + lines_vis - 2;
        } else
          curr_line--;
      }

      s32 char_idx = (s32)SendMessageW( parent, EM_LINEINDEX, (u64)curr_line - 1, 0 );
      SendMessageW( parent, EM_SETSEL, char_idx, char_idx );
    }

    SendMessageW( parent, EM_SCROLLCARET, 0, 0 );
      
    cscroll_draw();
  }
  void cscroll_hover_scroll( MSLLHOOKSTRUCT* p_mouse ) {
    if( !txt_box )
      return;

    POINT cm_pos = p_mouse->pt;
    ScreenToClient( txt_box, &cm_pos );
    if( !PtInRect( &txt_rect, cm_pos ) )
      return;

    if( HIWORD( p_mouse->mouseData ) == 120 ) {
      if( curr_line > lines_vis )
        SendMessageW( parent, EM_SCROLL, SB_LINEUP, 0 );
      if( curr_line != 1 )
        curr_line--;
    } else {
      if( curr_line < line_last )
        SendMessageW( parent, EM_SCROLL, SB_LINEDOWN, 0 );
      if( curr_line != line_count )
        curr_line++;
    }

    s32 char_idx = (s32)SendMessageW( parent, EM_LINEINDEX, (u64)curr_line - 1, 0 );
    SendMessageW( parent, EM_SETSEL, char_idx, char_idx );

    SendMessageW( parent, EM_SCROLLCARET, 0, 0 );

    cscroll_draw();
  }
  void cscroll_mbutton_off() {
    if( mdragging ) {
      mdragging = false;
      m_delta = 0;
    }
  }
  void cscroll_mbutton_on( MSLLHOOKSTRUCT* p_mouse ) {
    POINT cm_pos = p_mouse->pt;
    ScreenToClient( parent, &cm_pos );
    if( PtInRect( &txt_rect, cm_pos ) ) {
      mdragging = true;
      mduser_start = cm_pos;
    }
  }
  void cscroll_mbutton_scroll( MSLLHOOKSTRUCT* p_mouse ) {
    if( !parent )
      return;

    POINT cm_pos = p_mouse->pt;
    ScreenToClient( parent, &cm_pos );

    POINT md_delta = cm_pos - mduser_start;
    /*
        MAKE SURE TO ADD CHECKS FOR BOTH THAT WE AREN'T GOING OFF-SCREEN (Y) OR INTO THE NEXT LINE (X)
        LOOK @ CARET POS FOR REMINDERS
    */
    if( md_delta.x >= line_sz.cx || md_delta.x <= -line_sz.cx ) {
      // scroll left or right by 1 char depending on if negative or positive

      mduser_start = cm_pos;
    } else if( md_delta.y >= line_sz.cy || md_delta.y <= -line_sz.cy ) {
      // scroll by line up or down depending on if negative or positive

      mduser_start = cm_pos;
    }
  }
  bool cscroll_ishovered( POINT m_pos ) {
    if( dragging )
      return false;

    m_pos.x -= 25; m_pos.y -= 50;
    hovered = PtInRect( &rect, m_pos );
    cscroll_draw( false, false );
    return hovered;
  }
public:
  void cscroll_setinfo( bool update_info, bool redraw ) {
    if( update_info ) {
      curr_line  = (s32)SendMessageW( parent, EM_LINEFROMCHAR, -1, 0 ) + 1;
      line_count = (s32)SendMessageW( parent, EM_GETLINECOUNT, 0, 0 );
      line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;

      txt_rect = get_wnd_sz( parent );
      lines_vis = (s32)( to_sz_point( txt_rect ).y / line_sz.cy );
      line_last = line_first + lines_vis - 1;

      if( line_count <= lines_vis )
        scroll_h = bkrect.bottom + 1;
      else
        scroll_h = max( bkrect.bottom - ( line_sz.cy * ( line_count - lines_vis ) ), 50 );

      scroll_y = (s32)( (f32)( bkrect.bottom - scroll_h ) * ( (f32)curr_line / (f32)line_count ) );

      if( scroll_y < -1 || curr_line == 1 )
        scroll_y = -1;
      else if( scroll_y + scroll_h > bkrect.bottom || curr_line == line_count )
        scroll_y = bkrect.bottom - scroll_h;

      rect = {
        txt_rect.right + 1,
        scroll_y,
        txt_rect.right + 25,
        scroll_y + scroll_h
      };
    }

    if( redraw ) {
      RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE );
      cscroll_draw( false, false );
    }
  }
  void cscroll_update() {
    if( parent ) {
      static s64 prev_sel = 0;
      s64 sel = SendMessageW( parent, EM_GETSEL, 0, 0 );

      if( sel != prev_sel ) {
        cscroll_draw( true, true );
        prev_sel = sel;
      }
    }
  }
};