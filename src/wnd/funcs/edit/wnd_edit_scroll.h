#pragma once

enum SCROLL_DIR {
  WHEEL_DDELTA = 0x0078,
  WHEEL_UDELTA = 0xFF88
};

class CSCROLL {
public:
  s32 line_count;
  s32 line_first;
  s32 curr_line;
  s32 line_last;
  s32 lines_vis;

private:
  SIZE line_sz;

public:
  RECT txt_rect;
  RECT bkrect;

private:
  s32 scroll_h;
  s32 scroll_y;
  bool hovered;
  s32  m_delta;
  HWND parent;
  RECT rect;
  
private:
  POINT mduser_start;
  POINT duser_start;

public:
  bool mdragging;
  bool dragging;

public:
  void create( const HWND hwnd ) {
    while( !hwnd )
      Sleep( 500 );

    parent = hwnd;
    txt_rect = get_wnd_sz( parent );
    bkrect = rect = {
      txt_rect.right + 1,
      txt_rect.top - 1,
      txt_rect.right + WND_BTN_SZ,
      txt_rect.bottom + 1
    };

    draw( true, true );
  }
  void draw( const bool update_info = true,
             const bool redraw = false ) {
    if( !parent ) return;

    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush(
             hovered ? COL_L_GRY : COL_M_GRY
           );

    if( redraw ) GetTextExtentPoint32A( hdc, "A", 1, &line_sz );
    set_info( update_info, redraw );

    FillRect( hdc, &bkrect, dbrush );
    FillRect( hdc, &rect  , lbrush );
    
    DeleteObject( dbrush );
    DeleteObject( lbrush );
    ReleaseDC( parent, hdc );
  }
public:
  void drag_off() {
    if( dragging ) {
      set_info( true, true );
      dragging = false;
      m_delta = 0;
    }
  }
  void drag_on( const POINT m_pos ) {
    if( hovered ) {
      duser_start = m_pos;
      dragging = true;
    }
  }
  void drag( const MSLLHOOKSTRUCT* p_mouse,
             const bool resizing ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    if( !GetAsyncKeyState( MK_LBUTTON ) ||
        !dragging || resizing ) {
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
      line_first = Edit_GetFirstVisibleLine( parent ) + 1;
      
      if( scroll_dir == 1 && curr_line <= line_last ) {
        if( curr_line == line_last )
          curr_line = line_first + lines_vis - 1;
        else
          curr_line++;
      } else if( scroll_dir == -1 && curr_line > 1 ) {
        if( curr_line >= lines_vis ) {
          SendMessageA( parent, EM_SCROLL, SB_LINEUP, 0 );
          curr_line = line_first + lines_vis - 2;
        } else
          curr_line--;
      }

      s32 char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 );
      Edit_SetSel( parent, char_idx, char_idx );
    }

    Edit_ScrollCaret( parent );

    draw();
  }
 public:
  bool is_scrolling( const ptr m_data,
                     const bool up ) {
    return ( HIWORD( m_data ) == ( up ? WHEEL_UDELTA : WHEEL_DDELTA ) );
  }
  void scroll_horizontal( const ptr m_data,
                                s32& char_idx ) {
    s32 curr_line_start_idx = Edit_LineIndex( parent, -1 );
    s32 line_len = Edit_LineLength( parent, char_idx );
      
    if( is_scrolling( m_data, false ) && char_idx - curr_line_start_idx > 0 )
       char_idx--;
    else if( is_scrolling( m_data, true ) && char_idx < curr_line_start_idx + line_len )
      char_idx++;
  }
  void scroll_vertical( const ptr m_data,
                              s32& char_idx ) {
    if ( is_scrolling( m_data, false ) && curr_line != 1 ) {
      if( curr_line >= lines_vis ) {
        SendMessageA( parent, EM_SCROLL, SB_LINEUP, 0 );
        curr_line = line_first + lines_vis - 2;
      } else
        curr_line--;
    } else if ( is_scrolling( m_data, true ) && curr_line < line_count ) {
      if( curr_line == line_last ) {
        SendMessageA( parent, EM_SCROLL, SB_LINEDOWN, 0 );
        curr_line = line_first + lines_vis;
      } else
        curr_line++;
    }
    char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 );
  }
  void hover_scroll( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT cm_pos = p_mouse->pt;
    ScreenToClient( parent, &cm_pos );
    if( !PtInRect( &txt_rect, cm_pos ) )
      return;

    s32 char_idx = (s32)HIWORD( Edit_GetSel( parent ) );

    // this has got to be another sin ngl... #4 ( absolved 1.5.24 )
    if( GetAsyncKeyState( VK_MBUTTON ) || GetAsyncKeyState( VK_CONTROL ) )
      scroll_horizontal( p_mouse->mouseData, char_idx );
    else
      scroll_vertical( p_mouse->mouseData, char_idx );

    Edit_SetSel( parent, char_idx, char_idx );
    Edit_ScrollCaret( parent );

    draw();
  }
  void mdrag_off() {
    mdragging = false;
    m_delta = 0;
  }
  void mdrag_on( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    if( PtInRect( &txt_rect, m_pos ) ) {
      mdragging = true;
      mduser_start = m_pos;
    }
  }
  void mdrag_scroll_horizontal( const salt delta,
                                const POINT m_pos,
                                      s32& char_idx ) {
    s32 curr_caret_idx = char_idx + 1;
    s32 curr_line_start_idx = Edit_LineIndex( parent, -1 ) + 1;
    if( delta < 0 ) {
      if( curr_caret_idx - curr_line_start_idx <= 0 ) {
        mduser_start = m_pos;
        return;
      }
      char_idx--;
    } else {
      s32 line_len = Edit_LineLength( parent, curr_caret_idx );
      if( curr_caret_idx >= curr_line_start_idx + line_len ) {
        mduser_start = m_pos;
        return;
      }
      char_idx++;
    }
    mduser_start = m_pos;
  }
  void mdrag_scroll_vertical( const salt delta,
                              const POINT m_pos,
                                    s32& char_idx ) {
    s32 caret_line_offset = static_cast<s32>( HIWORD( Edit_GetSel( parent ) ) ) - Edit_LineIndex( parent, -1 );
    if( delta < 0 && curr_line > 1 ) {
      if( curr_line >= lines_vis ) {
        SendMessageA( parent, EM_SCROLL, SB_LINEUP, 0 );
        curr_line = line_first + lines_vis - 2;
      } else
        curr_line--;
    } else if( delta > 0 && curr_line < line_count ) {
      if( curr_line == line_last ) {
        SendMessageA( parent, EM_SCROLL, SB_LINEDOWN, 0 );
        curr_line = line_first + lines_vis;
      } else
        curr_line++;
    } else
      return;

    char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 ) + caret_line_offset;
    mduser_start = m_pos;
  }
  void mdrag_scroll( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    POINT md_delta = m_pos - mduser_start;
    s32 char_idx = static_cast<s32>( HIWORD( Edit_GetSel( parent ) ) );

    if( md_delta.x >= line_sz.cx || md_delta.x <= -line_sz.cx )
      mdrag_scroll_horizontal( md_delta.x, m_pos, char_idx );
    else if( md_delta.y >= line_sz.cy || md_delta.y <= -line_sz.cy )
      mdrag_scroll_vertical( md_delta.y, m_pos, char_idx );
    else
      return;

    Edit_SetSel( parent, char_idx, char_idx );
    Edit_ScrollCaret( parent );

    draw();
  }
  void get_hovered( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( h_global, &m_pos );

    if( dragging )
      return;

    m_pos.x -= WND_BTN_SZ;
    m_pos.y -= ( WND_BTN_SZ * 2 );
    hovered = PtInRect( &rect, m_pos );
    draw( false, false );
  }
public:
  void set_info( const bool update_info, const bool redraw ) {
    if( update_info ) {
      curr_line  = Edit_LineFromChar( parent, -1 ) + 1;
      line_count = Edit_GetLineCount( parent );
      line_first = Edit_GetFirstVisibleLine( parent ) + 1;

      lines_vis = (s32)( get_size( txt_rect ).y / line_sz.cy );
      line_last = line_first + lines_vis - 1;

      if( line_count <= lines_vis )
        scroll_h = bkrect.bottom + 1;
      else
        scroll_h = max( bkrect.bottom - ( line_sz.cy * ( line_count - lines_vis ) ), ( WND_BTN_SZ * 2 ) );

      scroll_y = (s32)( (f32)( bkrect.bottom - scroll_h ) * ( (f32)curr_line / (f32)line_count ) );

      if( scroll_y < -1 || curr_line == 1 )
        scroll_y = -1;
      else if( scroll_y + scroll_h > bkrect.bottom || curr_line == line_count )
        scroll_y = bkrect.bottom - scroll_h;

      rect = {
        txt_rect.right + 1,
        scroll_y,
        txt_rect.right + WND_BTN_SZ,
        scroll_y + scroll_h
      };
    }

    if( !redraw )
      return;

    RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE );
    draw( false, false );
  }
  void update() {
    if( !parent )
      return;

    static s64 prev_sel = 0;
    s64 sel = Edit_GetSel( parent );

    if( sel == prev_sel )
      return;

    draw( true, true );
    prev_sel = sel;
  }
};