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

https://learn.microsoft.com/en-us/windows/win32/gdi/when-to-draw-in-a-window
If an action requires immediate feedback, the application can draw while the action takes place, without waiting for WM_PAINT.
For example, a typical application highlights the area the user selects rather than waiting for the next WM_PAINT message to update the area.
If an application draws at any other time, such as from within WinMain or during the processing of keyboard or mouse messages, it calls the GetDC or GetDCEx function to retrieve the display DC.


https://learn.microsoft.com/en-us/windows/win32/gdi/the-wm-paint-message
Only those portions of the window that have changed are redrawn. Attempts to draw outside this region are clipped and do not appear on the screen.

The system can also send WM_NCPAINT and WM_ERASEBKGND messages to the wndproc before BeginPaint returns, they direct the application to draw the nonclient area and window background.
The nonclient area is the part of a window that is outside of the client area, including features such as the title bar, window menu (also known as the System menu), and scroll bars.
Most applications rely on the default window function, DefWindowProc, to draw this area and therefore pass the WM_NCPAINT message to this function.
If a window belongs to a window class having a class background brush, the DefWindowProc function draws the window background automatically.

BeginPaint fills PAINTSTRUCT with information including dimensions of the portion of the window to be updated and a flag indicating whether the window background has been drawn.
The application can use this information to optimize drawing.
For example, it can use the dimensions of the update region, specified by the rcPaint member, to limit drawing to only those portions of the window that need updating.


https://learn.microsoft.com/en-us/windows/win32/gdi/the-update-region
The update region identifies the portion of a window that is out-of-date or invalid and in need of redrawing.
The system uses the update region to generate WM_PAINT messages for applications and to minimize the time applications spend bringing the contents of their windows up to date.

When the system determines that a window needs updating, it sets the dimensions of the update region to the invalid portion of the window.
Setting the update region does not immediately cause the application to draw, it continues retrieving messages from the message queue until no messages remain.
The system then checks the update region, and if the region is not empty (non-NULL), it sends a WM_PAINT message to the wndproc.

An application can use the update region to generate its WM_PAINT messages.
For example, an application that loads data from open files typically sets the update region while loading so that new data is drawn during processing of the next WM_PAINT message.


https://learn.microsoft.com/en-us/windows/win32/gdi/invalidating-and-validating-the-update-region
The InvalidateRect and InvalidateRgn functions do not generate WM_PAINT messages.
Instead, the system accumulates the changes made by these functions and its own changes while a window processes other messages in its message queue.
By accumulating changes, a window processes all changes at once instead of updating bits and pieces one step at a time.

The ValidateRect and ValidateRgn functions validate a portion of the window by removing a specified rectangle or region from the update region.
These functions are typically used when the window has updated a specific part of the screen in the update region before receiving the WM_PAINT message.
  - use validaterect for custom scrollbar background fill to keep it drawn?
  
  
https://learn.microsoft.com/en-us/windows/win32/gdi/retrieving-the-update-region
BeginPaint retrieves the dimensions of the smallest rectangle enclosing the current update region, copying the dimensions to the rcPaint member in the PAINTSTRUCT structure.


https://learn.microsoft.com/en-us/windows/win32/gdi/excluding-the-update-region
  - maybe this too for custom scrollbar
  
https://learn.microsoft.com/en-us/windows/win32/gdi/synchronous-and-asynchronous-drawing
Most drawing carried out during processing of the WM_PAINT message is asynchronous.
There is a delay between the time a portion of the window is invalidated and the time WM_PAINT is sent.
During the delay, the application typically retrieves messages from the queue and carries out other tasks.
This is because the system treats window drawing as a low-priority task and assumes user-input messages/messages that may affect pos&size of a window will be processed before WM_PAINT.

In some cases, it is necessary for an application to draw synchronously, immediately after invalidating a portion of the window.
A typical app draws its main window immediately after creating the window to signal the user that the application has started successfully.
The system draws some control windows synchronously, such as buttons, because such windows serve as the focus for user input.
Although any window with a basic drawing routine can be drawn sync'd, it should be done quickly and to not interfere with the app's ability to respond to user input.

The UpdateWindow and RedrawWindow functions allow for synchronous drawing.
UpdateWindow sends a WM_PAINT message directly to the window if the update region is not empty.
RedrawWindow sends a WM_PAINT message, but gives the app more control over how to draw the window.
  - Whether or not to draw the nonclient area & window background or whether to send the message regardless of whether the update region is empty.
These functions send the WM_PAINT message directly to the window, regardless of the number of other messages in the application message queue.

Any window requiring time-consuming operations should be asynchronous to prevent pending messages from being blocked as the window is drawn.
Also, any app that frequently invalidates small portions should allow these portions to consolidate into a single async WM_PAINT message over a series of synchronous WM_PAINT messages.


https://learn.microsoft.com/en-us/windows/win32/gdi/nonclient-area
The system sends a WM_NCPAINT message to the window whenever a part of the nonclient area of the window, such as the title/menu bar, or window frame, must be updated.
In general, processing these messages for standard windows is not recommended, because the application must be able to draw all the required parts of the nonclient area for the window.
For this reason, most applications pass these messages to DefWindowProc for default processing.

An application that creates custom nonclient areas for its windows must process these messages, and it must use a window device context to carry out drawing in the window.
The window device context enables the application to draw in all portions of the window, including the nonclient area.
Use GetWindowDC or GetDCEx function and, when drawing is complete, must release the window device context by using the ReleaseDC function.

The system maintains an update region for the nonclient area.
When an app gets a WM_NCPAINT message, the wParam is a handle to a region defining the dimensions of the update region.
The app can use the handle to combine the update region with the clipping region for the window device context.
The system doesn't automatically combine the update region when getting the window device context unless it uses GetDCEx & specifies both the region handle & DCX_INTERSECTRGN flag.
If the application doesn't combine the update region, only drawing operations that would otherwise extend outside the window are clipped.
The application is not responsible for clearing the update region, regardless of whether it uses the region.

If an application processes the WM_NCACTIVATE message, after processing it must return TRUE to direct the system to complete the change of active window.
If the window is minimized when the application receives the WM_NCACTIVATE message, it should pass the message to DefWindowProc to redraw the label for the icon.


https://learn.microsoft.com/en-us/windows/win32/gdi/child-window-update-region
The system does not set the parent's update region when the child's is set.
An application cannot generate a WM_PAINT message for the parent window by invalidating the child window.
Similarly, an application cannot generate a WM_PAINT message for the child by invalidating a portion of the parent's client area that lies entirely under the child window.
In such cases, neither window receives a WM_PAINT message.


https://learn.microsoft.com/en-us/windows/win32/gdi/window-display-device-contexts
A window device context enables an app to draw anywhere in a window, including the nonclient area.
Window device contexts are typically used by apps that process the WM_NCPAINT and WM_NCACTIVATE messages for windows with custom nonclient areas.
Using a window device context is not recommended for any other purpose.

An application can retrieve a window device context by using the GetWindowDC or GetDCEx function with the DCX_WINDOW option specified.
The function retrieves a window device context from the display device context cache.
A window that uses a window device context must release it after drawing by using the ReleaseDC function as soon as possible.
Window device contexts are always from the cache; the CS_OWNDC and CS_CLASSDC class styles do not affect the device context.


https://learn.microsoft.com/en-us/windows/win32/gdi/using-the-wm-paint-message
You can use the WM_PAINT message to carry out the drawing necessary for displaying information.
Since WM_PAINT messages are sent when your window must be updated or when you explicitly request an update, you can consolidate the code for drawing in your app's wndproc.
You can then use this code whenever your application must draw either new or existing information.


https://learn.microsoft.com/en-us/windows/win32/gdi/invalidating-the-client-area
If you don't want the app to wait until the message queue has no other messages, use UpdateWindow to force the WM_PAINT to be sent immediately.
If there is any invalid part of the client area, UpdateWindow sends the WM_PAINT message for the specified window directly to the wndproc.


https://learn.microsoft.com/en-us/windows/win32/gdi/drawing-a-custom-window-background
You can draw your own window background rather than having the system draw it for you.
Most applications specify a brush handle or sys color for the class background brush when registering the window class; this is used to draw the background.
If you set the class background brush to NULL, the system sends a WM_ERASEBKGND message to your wndproc whenever the background must be drawn, letting you draw a custom background.


https://learn.microsoft.com/en-us/windows/win32/gdi/wm-paint
The WM_PAINT message is generated by the system and should not be sent by an application.
To force a draw into a specific device context, use WM_PRINT/WM_PRINTCLIENT messages, it requires the target window to support WM_PRINTCLIENT which most common controls do.

The DefWindowProc function validates the update region.
The function may also send WM_NCPAINT to the wndproc if the window frame must be painted and send the WM_ERASEBKGND message if the window background must be erased.

A window may get internal paint messages from calling RedrawWindow with the RDW_INTERNALPAINT flag set, for this, the window may not have an update region.
An app may call the GetUpdateRect function to determine whether the window has an update region.
If GetUpdateRect returns zero, the application need not call the BeginPaint and EndPaint functions.

Apps must check needed internal paints by checking internal data structs for all WM_PAINT messages, as it may be from non-NULL update region & RedrawWindow call with RDW_INTERNALPAINT.

The system sends an internal WM_PAINT message only once.
After internal WM_PAINT is returned from Get/PeekMessage or by UpdateWindow, the system won't send WM_PAINT till the window is invalidated/ a RedrawWindow call with RDW_INTERNALPAINT.

For some common controls, the default WM_PAINT processing checks wParam, if non-NULL, the control assumes that the value is an HDC and paints using that device context.


https://learn.microsoft.com/en-us/windows/win32/gdi/wm-print
DefWindowProc processes this based on which drawing option is specified.
	PRF_CHECKVISIBLE, draws only if the window is visible
	PRF_NONCLIENT, draw the nonclient area in the specified device context
	PRF_ERASEBKGND, send the window a WM_ERASEBKGND message
	PRF_CLIENT is specified, send the window a WM_PRINTCLIENT message
	PRF_CHILDREN is set, send each visible child window a WM_PRINT message
	PRF_OWNED, send each visible owned window a WM_PRINT message


https://learn.microsoft.com/en-us/windows/win32/gdi/wm-printclient // use me
A window can process this message in much the same manner as WM_PAINT, except without Begin/EndPaint since a device context is provided.
The window should draw its entire client area rather than just the invalid region.

Windows that can be used anywhere in the system, such as controls, should process this message.


- try returning 0 after doing scrollbar painting
- also at the end of wm_paint calls
- try setting fErase as well between beginpaint calls
- use one or the other
  - https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-redrawwindow
  - https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-updatewindow