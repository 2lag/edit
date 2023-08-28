todo :
- finish scrollbar
- - [read](https://learn.microsoft.com/en-us/windows/win32/gdi/about-painting-and-drawing)
- - [read](https://learn.microsoft.com/en-us/windows/win32/gdi/using-the-wm-paint-message)
- - [read](https://learn.microsoft.com/en-us/windows/win32/gdi/painting-and-drawing-reference)
- - flicker when typing, scrolling more than 1 line
- add line count ( use em_ )
- minimize variable scope all around
- clean up everything in wnd_type(_scroll) & tps
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
Optimizing Drawing: involves minimizing redundant operations and making your code more efficient.
For example, you can reduce the number of times you call FillRect by checking if the region actually needs to be redrawn.
Here's an example of how you might optimize your WM_PAINT case:
```cpp
case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // Only redraw if necessary (e.g., when the window is resized)
    if (IsRectEmpty(&ps.rcPaint)) {
        EndPaint(hwnd, &ps);
        break;
    }

    // Rest of your drawing code here

    EndPaint(hwnd, &ps);
} break;
```