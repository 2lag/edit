#pragma once

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
  void cscroll_create( const HWND hwnd ) {
    parent = hwnd;
    txt_rect = get_wnd_sz( parent );
    bkrect = rect = {
      txt_rect.right + 1,
      txt_rect.top - 1,
      txt_rect.right + WND_BTN_SZ,
      txt_rect.bottom + 1
    };

    cscroll_draw( true, true );
  }
  void cscroll_draw( bool update_info = true, bool redraw = false ) {
    HDC hdc = GetDC( parent );
    HBRUSH dbrush = CreateSolidBrush( COL_D_GRY ),
           lbrush = CreateSolidBrush(
             hovered ? COL_L_GRY : COL_M_GRY
    );

    if( redraw )
      GetTextExtentPoint32A( hdc, "A", 1, &line_sz );
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
  void cscroll_drag_on( const POINT m_pos ) {
    if( hovered ) {
      duser_start = m_pos;
      dragging = true;
    }
  }
  void cscroll_drag( const MSLLHOOKSTRUCT* p_mouse, bool resizing ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    if( !GetAsyncKeyState( MK_LBUTTON ) || !dragging || resizing ) {
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

    cscroll_draw();
  }
  void cscroll_hover_scroll( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT cm_pos = p_mouse->pt;
    ScreenToClient( parent, &cm_pos );
    if( !PtInRect( &txt_rect, cm_pos ) )
      return;

    s32 char_idx = (s32)HIWORD( Edit_GetSel( parent ) );

    if( GetAsyncKeyState( VK_MBUTTON ) || GetAsyncKeyState( VK_CONTROL ) ) {
      s32 line_len = Edit_LineLength( parent, char_idx );
      s32 curr_line_start_idx = Edit_LineIndex( parent, -1 );

#define WHEEL_DDELTA 0x0078
#define WHEEL_UDELTA 0xFF88

      switch( HIWORD( p_mouse->mouseData ) ) {
      case WHEEL_DDELTA: {
        if( char_idx - curr_line_start_idx > 0 )
          char_idx--;
      } break;
      case WHEEL_UDELTA: {
        if( char_idx < curr_line_start_idx + line_len )
          char_idx++;
      } break;
      default:
        return;
      }
    } else {
      switch( HIWORD( p_mouse->mouseData ) ) {
      case WHEEL_DDELTA: {
        if( curr_line != 1 ) {
          if( curr_line >= lines_vis ) {
            SendMessageA( parent, EM_SCROLL, SB_LINEUP, 0 );
            curr_line = line_first + lines_vis - 2;
          } else
            curr_line--;
          char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 );
        }
      } break;
      case WHEEL_UDELTA: {
        if( curr_line < line_count ) {
          if( curr_line == line_last ) {
            SendMessageA( parent, EM_SCROLL, SB_LINEDOWN, 0 );
            curr_line = line_first + lines_vis;
          } else
            curr_line++;
          char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 );
        }
      } break;
      default:
        return;
      }
    }

    Edit_SetSel( parent, char_idx, char_idx );
    Edit_ScrollCaret( parent );

    cscroll_draw();
  }
  void cscroll_mbutton_off() {
    mdragging = false;
    m_delta = 0;
  }
  void cscroll_mbutton_on( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    if( PtInRect( &txt_rect, m_pos ) ) {
      mdragging = true;
      mduser_start = m_pos;
    }
  }
  void cscroll_mbutton_scroll( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( parent, &m_pos );

    POINT md_delta = m_pos - mduser_start;
    s32 char_idx = static_cast<s32>( HIWORD( Edit_GetSel( parent ) ) );

    if( md_delta.x >= line_sz.cx || md_delta.x <= -line_sz.cx ) {
      s32 curr_caret_idx = char_idx + 1;
      if( md_delta.x < 0 ) {
        s32 curr_line_start_idx = Edit_LineIndex( parent, -1 ) + 1;
        if( curr_caret_idx - curr_line_start_idx <= 0 ) {
          mduser_start = m_pos;
          return;
        }
        char_idx--;
      } else {
        s32 curr_line_start_idx = Edit_LineIndex( parent, -1 ) + 1;
        s32 line_len = Edit_LineLength( parent, curr_caret_idx );
        if( curr_caret_idx >= curr_line_start_idx + line_len ) {
          mduser_start = m_pos;
          return;
        }
        char_idx++;
      }
      mduser_start = m_pos;
    } else if( md_delta.y >= line_sz.cy || md_delta.y <= -line_sz.cy ) {
      s32 caret_line_offset = static_cast<s32>( HIWORD( Edit_GetSel( parent ) ) ) - Edit_LineIndex( parent, -1 );
      if( md_delta.y < 0 && curr_line > 1 ) {
        if( curr_line >= lines_vis ) {
          SendMessageA( parent, EM_SCROLL, SB_LINEUP, 0 );
          curr_line = line_first + lines_vis - 2;
        } else
          curr_line--;
      } else if( md_delta.y > 0 && curr_line < line_count ) {
        if( curr_line == line_last ) {
          SendMessageA( parent, EM_SCROLL, SB_LINEDOWN, 0 );
          curr_line = line_first + lines_vis;
        } else
          curr_line++;
      } else
        return;

      char_idx = Edit_LineIndex( parent, (u64)curr_line - 1 ) + caret_line_offset;
      mduser_start = m_pos;
    } else
      return;

    Edit_SetSel( parent, char_idx, char_idx );
    Edit_ScrollCaret( parent );

    cscroll_draw();
  }
  void cscroll_ishovered( const MSLLHOOKSTRUCT* p_mouse ) {
    POINT m_pos = p_mouse->pt;
    ScreenToClient( h_global, &m_pos );

    if( dragging )
      return;

    m_pos.x -= WND_BTN_SZ;
    m_pos.y -= ( WND_BTN_SZ * 2 );
    hovered = PtInRect( &rect, m_pos );
    cscroll_draw( false, false );
  }
public:
  void cscroll_setinfo( const bool update_info, const bool redraw ) {
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

    if( redraw ) {
      RedrawWindow( parent, 0, 0, RDW_ERASE | RDW_INVALIDATE );
      cscroll_draw( false, false );
    }
  }
  void cscroll_update() {
    if( parent ) {
      static s64 prev_sel = 0;
      s64 sel = Edit_GetSel( parent );

      if( sel != prev_sel ) {
        cscroll_draw( true, true );
        prev_sel = sel;
      }
    }
  }
};