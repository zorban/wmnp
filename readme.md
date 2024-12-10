# wmnp

Windows Manipulator - A Node.js module for manipulating windows in Windows OS.

## Description

**wmnp** is a Node.js addon that provides functionalities to interact with Windows OS windows. It allows you to get information about windows, manipulate their positions, sizes, visibility, and more.

## Installation
### Prerequisites
- Node.js 18.x or later
- Python 2.7 or later (or Python 3.x)
- Visual Studio Build Tools 2015 or later installed

  You can download and install the Visual Studio Build Tools from [here](https://visualstudio.microsoft.com/visual-cpp-build-tools/). During installation, select the "Desktop development with C++" workload.

- Node-gyp installed globally

  ```sh
  npm install -g node-gyp

### Install this library
```bash
npm install wmnp
```

## Usage

```javascript
const wmnp = require('wmnp');

// Get the active window
const activeWindow = wmnp.getActiveWindow();

// Get window dimensions
console.log(activeWindow.dimensions());

// Enumerate all windows
wmnp.enumerateWindows(window => {
  console.log(`Title: "${window.getTitle()}"`);
  console.log(window.dimensions());
});

// Get a window by class name
let notepadWin = wmnp.getWindowByClassName('Notepad');
console.log(notepadWin.getTitle());
console.log(notepadWin.dimensions());

// Get a window by title
notepadWin = wmnp.getWindowByTitle('Untitled - Notepad');
console.log(notepadWin.getTitle());
console.log(notepadWin.dimensions());

// Check if a window exists
let wnd = wmnp.getActiveWindow();
console.log(wnd.exists());

// Use Window constructor
wnd = new wmnp.Window(0x00000000); // Invalid hwnd
console.log(wnd.exists());  // false

wnd = wmnp.getActiveWindow();
const hwnd = wnd.getHwnd();
wnd = new wmnp.Window(hwnd); // Valid hwnd
console.log(wnd.exists()); // true

// Get window class name
console.log(wnd.getClassName());

// Get process ID of the window
console.log(wnd.getPid());

// Get parent window handle
console.log(wnd.getParentHwnd());

// Get ancestor window
const GA_PARENT = 1, GA_ROOT = 2, GA_ROOTOWNER = 3;
console.log(wnd.getAncestor(GA_PARENT));
console.log(wnd.getAncestor(GA_ROOT));
console.log(wnd.getAncestor(GA_ROOTOWNER));

// Get monitor information
console.log(wnd.getMonitor());

// Set a window to the foreground
wnd.setForegroundWindow();

// Set window position
const hwndInsertAfter = 0; // HWND_TOP
const uFlags = 0x0000 | 0x0040; // SWP_SHOWWINDOW
let x = 100, y = 100, width = 400, height = 400;
wnd.setWindowPos(hwndInsertAfter, x, y, width, height, uFlags);

// Show the window
const nCmdShow = 1; // SW_SHOWNORMAL
wnd.showWindow(nCmdShow);

// Move the window
wnd.move(x, y, width, height);

// Move the window relative to its current position
let dx = 100, dy = 100, dw = 40, dh = 40;
wnd.moveRelative(dx, dy, dw, dh);
```

## API Reference

### wmnp.getActiveWindow()

Returns the active window as a Window object.

### wmnp.enumerateWindows(callback)

Enumerates all top-level windows and calls the provided `callback` function with each window.

### wmnp.getWindowByClassName(className)

Returns a Window object for the first window that matches the given class name.

### wmnp.getWindowByTitle(title)

Returns a Window object for the first window that matches the given title.

### wmnp.Window(hwnd)

Creates a new Window object with the specified window handle (hwnd).

### Window Methods

- `getHwnd()`: Returns the window handle.
- `getTitle()`: Returns the window title.
- `getClassName()`: Returns the window class name.
- `getPid()`: Returns the process ID of the window.
- `exists()`: Checks if the window exists.
- `dimensions()`: Returns an object containing the window's dimensions.
- `getParentHwnd()`: Returns the parent window handle.
- `getAncestor(gaFlags)`: Returns the ancestor window handle based on the `gaFlags`.
- `getMonitor()`: Returns information about the monitor that the window is on.
- `setForegroundWindow()`: Sets the window to the foreground.
- `setWindowPos(hwndInsertAfter, x, y, cx, cy, uFlags)`: Sets the window's position.
- `showWindow(nCmdShow)`: Shows or hides the window based on `nCmdShow`.
- `move(x, y, width, height)`: Moves and resizes the window.
- `moveRelative(dx, dy, dw, dh)`: Moves and resizes the window relative to its current position and size.

## Constants

### gaFlags Values

- `GA_PARENT = 1`
- `GA_ROOT = 2`
- `GA_ROOTOWNER = 3`

### hwndInsertAfter Values

- `HWND_TOP = 0`
- `HWND_BOTTOM = 1`
- `HWND_TOPMOST = -1`
- `HWND_NOTOPMOST = -2`

### uFlags Values

- `SWP_NOSIZE = 0x0001`
- `SWP_NOMOVE = 0x0002`
- `SWP_NOZORDER = 0x0004`
- `SWP_NOREDRAW = 0x0008`
- `SWP_NOACTIVATE = 0x0010`
- `SWP_FRAMECHANGED = 0x0020`
- `SWP_SHOWWINDOW = 0x0040`
- `SWP_HIDEWINDOW = 0x0080`
- `SWP_NOCOPYBITS = 0x0100`
- `SWP_NOOWNERZORDER = 0x0200`
- `SWP_NOSENDCHANGING = 0x0400`
- `SWP_DRAWFRAME = SWP_FRAMECHANGED`
- `SWP_NOREPOSITION = SWP_NOOWNERZORDER`
- `SWP_DEFERERASE = 0x2000`
- `SWP_ASYNCWINDOWPOS = 0x4000`

### nCmdShow Values

- `SW_HIDE = 0`
- `SW_SHOWNORMAL = 1`
- `SW_SHOWMINIMIZED = 2`
- `SW_SHOWMAXIMIZED = 3`
- `SW_SHOWNOACTIVATE = 4`
- `SW_SHOW = 5`
- `SW_MINIMIZE = 6`
- `SW_SHOWMINNOACTIVE = 7`
- `SW_SHOWNA = 8`
- `SW_RESTORE = 9`
- `SW_SHOWDEFAULT = 10`
- `SW_FORCEMINIMIZE = 11`

## Development

To build the project, run:

```bash
npm run build
```

## Dependencies

- `node-addon-api`
- `node-gyp`

## Author

**Zoran Banković** - [zoranbankovic@gmail.com](mailto:zoranbankovic@gmail.com)

## License

MIT
