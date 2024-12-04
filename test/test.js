const wmnp = require('..');

// For testing: Open Notepad with the title "Untitled - Notepad"

function writeTestHeader(testHeader) {
	console.log(`\n\n===============\n${testHeader}\n===============\n`);
}

// Test getting the active window
writeTestHeader('Test getting the active window');
// Get the active window
const activeWindow = wmnp.getActiveWindow();


// Tests getting the active window dimensions
writeTestHeader('Test getting the active window dimensions');
console.log(activeWindow.dimensions());


// Tests enumerating windows
writeTestHeader('Test enumerating windows');
wmnp.enumerateWindows(w => {
	console.log('\n\n');
	console.log(`Title: "${w.getTitle()}"`);
	console.log('-------------------------------------------------')
	console.log(w.dimensions());
	console.log('.');
});


// Tests getting a window by class name
writeTestHeader('Test getting a window by class name');
let notepadWin = wmnp.getWindowByClassName('Notepad');
console.log(notepadWin.getTitle());
console.log(notepadWin.dimensions());


// Tests getting a window by title
writeTestHeader('Test getting a window by title');
notepadWin = wmnp.getWindowByTitle('Untitled - Notepad');
console.log(notepadWin.getTitle());
console.log(notepadWin.dimensions());


// Test if active window exists
writeTestHeader('Test if active window exists');
let wnd = wmnp.getActiveWindow();
console.log(wnd.exists());


// Test if Window constructor works
writeTestHeader('Test if Window constructor works');
wnd = new wmnp.Window(0x00000000); // Create a new window object with an invalid hwnd
console.log(wnd.exists());  // Should return false

wnd = wmnp.getActiveWindow();
const hwnd = wnd.getHwnd();
console.log(hwnd);
wnd = new wmnp.Window(hwnd); // Create a new window object with the existing hwnd
console.log(wnd.exists()); // Should return true


// Test getting the class name of the active window
writeTestHeader('Test getting the class name of the active window');
wnd = wmnp.getActiveWindow();
console.log(wnd.getClassName());

// Test getting the process id of the active window
writeTestHeader('Test getting the process id of the active window');
wnd = wmnp.getActiveWindow();
console.log(wnd.getPid());

// Test getting the parent hwnd of the window
writeTestHeader('Test getting the parent hwnd of the window');
wnd = wmnp.getActiveWindow();
console.log(wnd.getParentHwnd());
console.log(wmnp.getWindowByClassName('Notepad').getHwnd());
wnd = wmnp.getWindowByTitle('Open'); // Click File -> Open in Notepad
console.log(wnd.getParentHwnd());


// Test getting the grandparent hwnd of the window
writeTestHeader('Test getting the grandparent hwnd of the window');
GA_PARENT = 1, GA_ROOT = 2, GA_ROOTOWNER = 3;
wnd = wmnp.getActiveWindow();
console.log(wnd.getAncestor(GA_PARENT));
console.log(wnd.getAncestor(GA_ROOT));
console.log(wnd.getAncestor(GA_ROOTOWNER));


// Test getting the monitor of the window
writeTestHeader('Test getting the monitor of the window');
wnd = wmnp.getActiveWindow();
console.log(wnd.getMonitor());
console.log(wmnp.getWindowByClassName('Notepad').getMonitor());

// Test setting the foreground window
writeTestHeader('Test setting the foreground window');
wnd = wmnp.getWindowByClassName('Notepad');
wnd.setForegroundWindow();


// Test setting the window position
writeTestHeader('Test setting the window position');
// hwndInsertAfter: HWND_TOP = 0, HWND_BOTTOM = 1, HWND_TOPMOST = -1, HWND_NOTOPMOST = -2
// uFlags: SWP_NOSIZE = 0x0001, SWP_NOMOVE = 0x0002, SWP_NOZORDER = 0x0004, SWP_NOREDRAW = 0x0008, SWP_NOACTIVATE = 0x0010, SWP_FRAMECHANGED = 0x0020, SWP_SHOWWINDOW = 0x0040, SWP_HIDEWINDOW = 0x0080, SWP_NOCOPYBITS = 0x0100, SWP_NOOWNERZORDER = 0x0200, SWP_NOSENDCHANGING = 0x0400, SWP_DRAWFRAME = SWP_FRAMECHANGED, SWP_NOREPOSITION = SWP_NOOWNERZORDER, SWP_DEFERERASE = 0x2000, SWP_ASYNCWINDOWPOS = 0x4000
const hwndInsertAfter = 0;
const uFlags = 0x0000 | 0x0040;
win = wmnp.getWindowByClassName('Notepad');
let x = 100, y = 100, width = 400, height = 400;
win.setWindowPos(hwndInsertAfter, x, y, width, height, uFlags);


// Test showing the window
writeTestHeader('Test showing the window');
// nCmdShow: SW_HIDE = 0, SW_SHOWNORMAL = 1, SW_NORMAL = 1, SW_SHOWMINIMIZED = 2, SW_SHOWMAXIMIZED = 3, SW_MAXIMIZE = 3, SW_SHOWNOACTIVATE = 4, SW_SHOW = 5, SW_MINIMIZE = 6, SW_SHOWMINNOACTIVE = 7, SW_SHOWNA = 8, SW_RESTORE = 9, SW_SHOWDEFAULT = 10, SW_FORCEMINIMIZE = 11, SW_MAX = 11
const nCmdShow = 1;
win = wmnp.getWindowByClassName('Notepad');
win.showWindow(nCmdShow);

// Test moving the window
writeTestHeader('Test moving the window');
x = 100, y = 100, width = 400, height = 400;
win = wmnp.getWindowByClassName('Notepad');
win.move(x, y, width, height);


// Test moving the window relative to its current position
writeTestHeader('Test moving the window relative to its current position');
let dx = 100, dy = 100, dw = 40, dh = 40;
win = wmnp.getWindowByClassName('Notepad');
win.moveRelative(dx, dy, dw, dh);
