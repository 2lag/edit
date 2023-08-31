todo :
- redo scrollbar
- add line count ( use edit_getlinecount/edit_getfirstvisibleline )
- minimize variable scope, clean up, & optimize everything in all files
- add icon
- add menu bar & functionality
- - file
- - macro
- - theme

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#scroll-box-position-and-scrolling-range
The position must be within the minimum and maximum values of the scrolling range. For example, in a scroll bar with a range of 0 through 100, position 50 is in the middle, with the remaining positions distributed equally along the scroll bar. The initial range depends on the scroll bar. Standard scroll bars have an initial range of 0 through 100; scroll bar controls have an empty range (both minimum and maximum values are zero), unless you supply an explicit range when the control is created. You can change the range at any time. You can use the SetScrollInfo function to set the range values, and the GetScrollInfo function to retrieve the current range values.
An application typically adjusts the scroll range to convenient integers, making it easy to translate the scroll box position into a value corresponding to the data object to be scrolled. For example, if an application must display 260 lines of a text file in a window that can show only 16 lines at a time, the vertical scroll bar range can be set to 1 through 244. If the scroll box is at position 1, the first line will be at the top of the window. If the scroll box is at position 244, the last line (line 260) will be at the bottom of the window. If an application attempts to specify a position value that is less than the minimum or more than the maximum, the minimum or maximum scrolling range value is used instead.
You can set a page size for a scroll bar. The page size represents the number of data units that can fit in the client area of the owner window given its current size. For example, if the client area can hold 16 lines of text, an application would set the page size to 16. The system uses the page size, along with the scrolling range and length of the scroll bar shaft, to set the size of the scroll box. Whenever a window containing a scroll bar is resized, an application should call the SetScrollInfo function to set the page size. An application can retrieve the current page size by calling the sending GetScrollInfo function.
To establish a useful relationship between the scroll bar range and the data object, an application must adjust the range whenever the size of the data object changes.
As the user moves the scroll box in a scroll bar, the scroll bar reports the scroll box position as an integer in the scrolling range. If the position is the minimum value, the scroll box is at the top of a vertical scroll bar or at the left end of a horizontal scroll bar. If the position is the maximum value, the scroll box is at the bottom of a vertical scroll bar or at right end of a horizontal scroll bar.
The maximum value that a scroll bar can report (that is, the maximum scrolling position) depends on the page size. If the scroll bar has a page size greater than one, the maximum scrolling position is less than the maximum range value. You can use the following formula to calculate the maximum scrolling position:
```MaxScrollPos = MaxRangeValue - (PageSize - 1)```
An application must move the scroll box in a scroll bar. Although the user makes a request for scrolling in a scroll bar, the scroll bar does not automatically update the scroll box position. Instead, it passes the request to the parent window, which must scroll the data and update the scroll box position. An application uses the SetScrollInfo function to update the scroll box position; otherwise, it uses the SetScrollPos function. Because it controls the scroll box movement, the application can move the scroll box in increments that work best for the data being scrolled.

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#scroll-bar-visibility
The system hides and disables a standard scroll bar when equal minimum and maximum values are specified. The system also hides and disables a standard scroll bar if you specify a page size that includes the entire scroll range of the scroll bar. This is the way to temporarily hide a scroll bar when it is not needed for the content of the client area. There is no need to make scrolling requests through the scroll bar when it is hidden. The system enables the scroll bar and shows it again when you set the minimum and maximum values to unequal values and when the page size does not include the entire scroll range. The ShowScrollBar function can also be used to hide or show a scroll bar. It does not affect the scroll bar's range, page size, or scroll box position.
The EnableScrollBar function can be used to disable one or both arrows of a scroll bar. An application displays disabled arrows in gray and does not respond to user input.

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#scroll-bar-requests
The user makes scrolling requests by clicking various parts of a scroll bar. The system sends the request to the specified window in the form of a WM_HSCROLL or WM_VSCROLL message. A horizontal scroll bar sends the WM_HSCROLL message; a vertical scroll bar sends the WM_VSCROLL message. Each message includes a request code that corresponds to the user's action, to the handle to the scroll bar (scroll bar controls only), and, in some cases, to the position of the scroll box.
The SB_ values specify the action the user takes. An application examines the codes that accompany the WM_HSCROLL and WM_VSCROLL messages and then performs the appropriate scrolling operation. In the following table, the user's action is specified for each value, followed by the application's response. In each case, a unit is defined by the application as appropriate for the data. For example, the typical unit for scrolling text vertically is a line of text.

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#keyboard-interface-for-a-scroll-bar
A scroll bar control provides a built-in keyboard interface that enables the user to issue scrolling requests by using the keyboard; a standard scroll bar does not. When a scroll bar control has the keyboard focus, it sends WM_HSCROLL and WM_VSCROLL messages to its parent window when the user presses the arrow keys. The request code is sent with each message corresponding to the arrow key the user has pressed. Following are the arrow keys and their corresponding request codes.

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#scrolling-the-client-area
The simplest way to scroll the content of a client area is to erase and then redraw it. This is the method an application is likely to use with SB_PAGEUP, SB_PAGEDOWN, and SB_TOP request codes, which typically require completely new content.
For some request codes, such as SB_LINEUP and SB_LINEDOWN, not all the content need be erased, because some remains visible after scrolling occurs. The ScrollWindowEx function preserves a portion of the client area's content, moves the preserved portion a specified amount, and then prepares the rest of the client area for painting new information. ScrollWindowEx uses the BitBlt function to move a specific part of the data object to a new location within the client area. Any uncovered part of the client area (anything not preserved) is invalidated, erased, and painted when the next WM_PAINT message occurs.
The ScrollWindowEx function can be used to exclude a portion of the client area from the scrolling operation. This keeps items with fixed positions, such as child windows, from moving within the client area. It automatically invalidates the portion of the client area that is to receive the new information, so the application does not have to compute its own clipping regions. For more information on clipping, see Clipping.
Usually an application scrolls the content of a window in the direction opposite that indicated by the scroll bar. For example, when the user clicks the scroll bar shaft in the area below the scroll box, an application scrolls the object in the window upward to reveal a portion of the object that is below the visible portion.

https://learn.microsoft.com/en-us/windows/win32/controls/about-scroll-bars#scroll-bar-colors-and-metrics
The system-defined color value, COLOR_SCROLLBAR, controls the color within a scroll bar shaft. Use the GetSysColor function to determine the color of the scroll bar shaft and the SetSysColors function to set the color of the scroll bar shaft. Note, however, that this change of color affects all scroll bars in the system.
You can get the dimensions of the bitmaps that the system uses in standard scroll bars by calling the GetSystemMetrics function. Following are the system metric values associated with scroll bars.

use SB_CTL/OBJID_CLIENT with hwnd to scrollbar control
use ScrollDC to scroll a device context
ScrollWindow(Ex) scrolls a clients area
SetScrollInfo to set params of scrollbar & can force redraw of scrollbar
try hiding with ShowScrollBar

create SCROLLINFO for scrollbar structure
use SCROLLBARINFO for the custom draw

do WM_CTLCOLORSCROLLBAR to set color
read how WM_VSCROLL functions for setup
send SBM_ messages for various setups/retrievals ( or use macros )

last shot : https://www.codeguru.com/cplusplus/multiline-edit-box-with-automatic-scroll-bar-display/