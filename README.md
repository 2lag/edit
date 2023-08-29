todo :
- review scrollbar notes and make plan to finish scrollbar
- - flicker when typing, scrolling more than 1 line, hovering over it, etc
- add line count ( use em_ )
- minimize variable scope all around
- clean up & optimize everything in wnd_type & tps
- add icon
- add menu bar
- - file
- - macro
- - theme


### notes :
Double Buffering: a technique where you paint your graphics off-screen first and then draw the entire prepared image on-screen.
This can reduce flickering.
Here's how you can implement it in your WM_PAINT case:
```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Create an off-screen bitmap and DC for double buffering
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, pwnd_sz.x, pwnd_sz.y);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

    // Now, draw everything on the off-screen bitmap
    // ...

    // BitBlt the off-screen image to the screen
    BitBlt(hdc, 0, 0, pwnd_sz.x, pwnd_sz.y, memDC, 0, 0, SRCCOPY);

    // Clean up
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);

    EndPaint(hwnd, &ps);
} break;
```

use GetDC/GetDCEx to get dc outside of wndproc
check rcPaint size as well in beginpaint calls where scrollbar may be drawn ?
InvalidateRect does not generate WM_PAINT messages, it just adds to the update region for the next wm_paint message in queue
maybe use ValidateRect to keep scrollbar drawn on moments where it bugs and displays one underneath ( hover, over-scroll, etc )
to remove delay between sending invalidate, and it being drawn, use Update/RedrawWindow for syncronous drawing ( try both )