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
  RECT rect;
  
  bool hovered;
  bool dragging;
  POINT duser_start;

public:
  void cscroll_create( HWND hwnd ) {
    parent = hwnd;

    RECT r = get_wnd_sz( parent );
    bkrect = rect = {
      r.right + 1,
      -1,
      r.right + 25,
      r.bottom + 1
    };
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
    if( !dragging )
      return;

    cscroll_setinfo( true, true );
    dragging = false;
    m_delta = 0;
  }
  void cscroll_drag_on( POINT m_pos ) {
    if( !cscroll_ishovered( m_pos ) )
      return;

    duser_start = m_pos;
    dragging = true;
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
      line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 );
      
      if( scroll_dir == 1 && curr_line <= line_last ) {
        if( curr_line == line_last )
          curr_line = line_first + lines_vis;
        else
          curr_line++;
      } else if( scroll_dir == -1 && curr_line > 1 ) {
        if( curr_line >= lines_vis ) {
          SendMessageW( parent, EM_SCROLL, SB_LINEUP, 0 );
          curr_line = line_first + lines_vis - 1;
        } else
          curr_line--;
      }

      s32 char_idx = (s32)SendMessageW( parent, EM_LINEINDEX, curr_line - 1, 0 );
      SendMessageW( parent, EM_SETSEL, char_idx, char_idx );
    }

    SendMessageW( parent, EM_SCROLLCARET, 0, 0 );
      
    cscroll_draw( true, false );
  }
  bool cscroll_ishovered( POINT m_pos ) {
    m_pos.x -= 25; m_pos.y -= 50;
    if( PtInRect( &rect, m_pos ) ) {
      hovered = true;
      cscroll_draw( false, true );
      return true;
    }
    hovered = false;
    cscroll_draw( false, true );
    return false;
  }
public:
  void cscroll_setinfo( bool update_info, bool redraw ) {
    if( update_info ) {
      curr_line  = (s32)SendMessageW( parent, EM_LINEFROMCHAR, -1, 0 ) + 1;
      line_count = (s32)SendMessageW( parent, EM_GETLINECOUNT, 0, 0 );
      line_first = (s32)SendMessageW( parent, EM_GETFIRSTVISIBLELINE, 0, 0 ) + 1;
      lines_vis = to_sz_point( get_wnd_sz( parent ) ).y / line_sz.cy;
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

      RECT r = get_wnd_sz( parent );

      rect = {
        r.right + 1,
        scroll_y,
        r.right + 25,
        scroll_y + scroll_h
      };

#ifdef _DEBUG
      printf(
        "line count     : %d\nline first vis : %d\nline last vis  : %d\nline total vis : %d\nscroll height  : %d\nscroll y top   : %d\nscroll hover   : %d\nscroll drag    : %d\nm_delta        : %d\ncurrent line   : %d\n\n\n\n\n\n\n",
        line_count, line_first, line_last, lines_vis, scroll_h, scroll_y, hovered, dragging, m_delta, curr_line
      );
#endif
    }

    if( redraw ) {
      RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE );
      cscroll_draw( false, false );
    }
  }
};