#include "WmnpWindow.h"

Window::Window(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Window>(info) {
	Napi::Env env = info.Env();
	
	if (info.Length() < 1 || !info[0].IsNumber()) {
		throw Napi::TypeError::New(env, "Window handle must be a number");
	}
	
	this->windowHandle = reinterpret_cast<HWND>(static_cast<intptr_t>(info[0].As<Napi::Number>().Int64Value()));
}

Napi::Function Window::GetConstructor(Napi::Env env) {
	return DefineClass(env, "Window", {
		StaticMethod("enumerateWindows", &Window::EnumerateWindows),
		StaticMethod("getActiveWindow", &Window::GetActiveWindow),
		StaticMethod("getWindowByClassName", &Window::GetWindowByClassName),
		StaticMethod("getWindowByTitle", &Window::GetWindowByTitle),

		InstanceMethod("exists", &Window::Exists),
		InstanceMethod("isVisible", &Window::IsVisible),
		InstanceMethod("getTitle", &Window::GetTitle),
		InstanceMethod("getHwnd", &Window::GetHwnd),
		InstanceMethod("getClassName", &Window::GetClassName),
		InstanceMethod("getPid", &Window::GetPid),
		InstanceMethod("getParentHwnd", &Window::GetParentHwnd),
		InstanceMethod("getAncestor", &Window::GetAncestor),
		InstanceMethod("getMonitor", &Window::GetMonitor),
		InstanceMethod("setForegroundWindow", &Window::SetForegroundWindow),
		InstanceMethod("setWindowPos", &Window::SetWindowPos),
		InstanceMethod("showWindow", &Window::ShowWindow),
		InstanceMethod("move", &Window::Move),
		InstanceMethod("moveRelative", &Window::MoveRelative),
		InstanceMethod("dimensions", &Window::Dimensions)
	});
}

// Static methods
Napi::FunctionReference enumCallback;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	Napi::Env env = enumCallback.Env();
	Napi::HandleScope scope(env);

	Napi::Function constructor = Window::GetConstructor(env);
	Napi::Object windowObj = constructor.New({ Napi::Number::New(env, reinterpret_cast<int64_t>(hwnd)) });

	enumCallback.Call({ windowObj });

	return TRUE;
}

void Window::EnumerateWindows(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 1 || !info[0].IsFunction()) {
		throw Napi::TypeError::New(env, "Callback function expected");
	}

	enumCallback = Napi::Persistent(info[0].As<Napi::Function>());

	EnumWindows(EnumWindowsProc, 0);
}

Napi::Value Window::GetActiveWindow(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	HWND fgWin = GetForegroundWindow();
	
	Napi::Function constructor = Window::GetConstructor(env);
	return constructor.New({ Napi::Number::New(env, reinterpret_cast<int64_t>(fgWin)) });
}

Napi::Value Window::GetWindowByClassName(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 1 || !info[0].IsString()) {
		throw Napi::TypeError::New(env, "Class name must be a string");
	}

	std::string sClassname = info[0].As<Napi::String>().Utf8Value();
	std::wstring wsClassname(sClassname.begin(), sClassname.end());
	HWND fgWin = FindWindowExW(0, 0, wsClassname.c_str(), 0);

	Napi::Function constructor = Window::GetConstructor(env);
	return constructor.New({ Napi::Number::New(env, reinterpret_cast<int64_t>(fgWin)) });
}

Napi::Value Window::GetWindowByTitle(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 1 || !info[0].IsString()) {
		throw Napi::TypeError::New(env, "Title must be a string");
	}

	std::string sExactTitle = info[0].As<Napi::String>().Utf8Value();
	std::wstring wsExactTitle(sExactTitle.begin(), sExactTitle.end());
	HWND fgWin = FindWindowW(NULL, wsExactTitle.c_str());

	Napi::Function constructor = Window::GetConstructor(env);
	return constructor.New({ Napi::Number::New(env, reinterpret_cast<int64_t>(fgWin)) });
}


// Instance methods
Napi::Value Window::Exists(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	return Napi::Boolean::New(env, IsWindow(this->windowHandle));
}

Napi::Value Window::IsVisible(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	return Napi::Boolean::New(env, IsWindowVisible(this->windowHandle));
}

Napi::Value Window::GetTitle(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	wchar_t wnd_title[256];
	GetWindowTextW(this->windowHandle, wnd_title, sizeof(wnd_title));
	std::wstring ws(wnd_title);
	std::string str(ws.begin(), ws.end());
	return Napi::String::New(env, str);
}

Napi::Value Window::GetHwnd(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	return Napi::Number::New(env, reinterpret_cast<int64_t>(this->windowHandle));
}

Napi::Value Window::GetClassName(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	wchar_t wnd_cn[256];
	::GetClassNameW(this->windowHandle, wnd_cn, sizeof(wnd_cn));
	std::wstring ws(wnd_cn);
	std::string str(ws.begin(), ws.end());
	return Napi::String::New(env, str);
}

Napi::Value Window::GetPid(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(this->windowHandle, &lpdwProcessId);

	return Napi::Number::New(env, static_cast<int>(lpdwProcessId));
}

Napi::Value Window::GetParentHwnd(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	HWND parentHwnd = ::GetParent(this->windowHandle);
	if (parentHwnd != NULL) {
		return Napi::Number::New(env, reinterpret_cast<int64_t>(parentHwnd));
	}
	return env.Null();
}

// GA_PARENT = 1, GA_ROOT = 2, GA_ROOTOWNER = 3
Napi::Value Window::GetAncestor(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 1 || !info[0].IsNumber()) {
		throw Napi::TypeError::New(env, "Ancestor flag must be a number");
	}

	UINT gaFlags = info[0].As<Napi::Number>().Uint32Value();
	HWND ancestorHwnd = ::GetAncestor(this->windowHandle, gaFlags);

	if (ancestorHwnd != NULL) {
		return Napi::Number::New(env, reinterpret_cast<int64_t>(ancestorHwnd));
	}
	return env.Null();
}

Napi::Value Window::GetMonitor(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	HMONITOR hMonitor = MonitorFromWindow(this->windowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX mi;
	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);

	Napi::Object result = Napi::Object::New(env);
	bool isPrimary = mi.dwFlags & MONITORINFOF_PRIMARY;
	std::wstring wsDevice(mi.szDevice, mi.szDevice + sizeof(mi.szDevice) / sizeof(wchar_t));
	std::string sDevice(wsDevice.begin(), wsDevice.end());
	result.Set("name", Napi::String::New(env, sDevice));
	result.Set("primary", Napi::Boolean::New(env, isPrimary));

	Napi::Object dim = Napi::Object::New(env);
	dim.Set("left", Napi::Number::New(env, mi.rcMonitor.left));
	dim.Set("top", Napi::Number::New(env, mi.rcMonitor.top));
	dim.Set("right", Napi::Number::New(env, mi.rcMonitor.right));
	dim.Set("bottom", Napi::Number::New(env, mi.rcMonitor.bottom));

	result.Set("dimensions", dim);

	return result;
}

void Window::SetForegroundWindow(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	::SetForegroundWindow(this->windowHandle);
}

// hwndInsertAfter: HWND_TOP = 0, HWND_BOTTOM = 1, HWND_TOPMOST = -1, HWND_NOTOPMOST = -2
// uFlags: SWP_NOSIZE = 0x0001, SWP_NOMOVE = 0x0002, SWP_NOZORDER = 0x0004, SWP_NOREDRAW = 0x0008, SWP_NOACTIVATE = 0x0010, SWP_FRAMECHANGED = 0x0020, SWP_SHOWWINDOW = 0x0040, SWP_HIDEWINDOW = 0x0080, SWP_NOCOPYBITS = 0x0100, SWP_NOOWNERZORDER = 0x0200, SWP_NOSENDCHANGING = 0x0400, SWP_DRAWFRAME = SWP_FRAMECHANGED, SWP_NOREPOSITION = SWP_NOOWNERZORDER, SWP_DEFERERASE = 0x2000, SWP_ASYNCWINDOWPOS = 0x4000
void Window::SetWindowPos(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 6 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber() || !info[4].IsNumber() || !info[5].IsNumber()) {
		throw Napi::TypeError::New(env, "Expected six arguments: hWndInsertAfter, X, Y, cx, cy, uFlags");
	}

	HWND hWndInsertAfter = reinterpret_cast<HWND>(static_cast<intptr_t>(info[0].As<Napi::Number>().Int64Value()));
	int X = info[1].As<Napi::Number>().Int32Value();
	int Y = info[2].As<Napi::Number>().Int32Value();
	int cx = info[3].As<Napi::Number>().Int32Value();
	int cy = info[4].As<Napi::Number>().Int32Value();
	int uFlags = info[5].As<Napi::Number>().Int32Value();

	::SetWindowPos(this->windowHandle, hWndInsertAfter, X, Y, cx, cy, uFlags);
}

// nCmdShow: SW_HIDE = 0, SW_SHOWNORMAL = 1, SW_NORMAL = 1, SW_SHOWMINIMIZED = 2, SW_SHOWMAXIMIZED = 3, SW_MAXIMIZE = 3, SW_SHOWNOACTIVATE = 4, SW_SHOW = 5, SW_MINIMIZE = 6, SW_SHOWMINNOACTIVE = 7, SW_SHOWNA = 8, SW_RESTORE = 9, SW_SHOWDEFAULT = 10, SW_FORCEMINIMIZE = 11, SW_MAX = 11
void Window::ShowWindow(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 1 || !info[0].IsNumber()) {
		throw Napi::TypeError::New(env, "Expected one argument: nCmdShow");
	}

	int nCmdShow = info[0].As<Napi::Number>().Int32Value();
	::ShowWindow(this->windowHandle, nCmdShow);
}

void Window::Move(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 4 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
		throw Napi::TypeError::New(env, "Expected four arguments: x, y, width, height");
	}

	int x = info[0].As<Napi::Number>().Int32Value();
	int y = info[1].As<Napi::Number>().Int32Value();
	int width = info[2].As<Napi::Number>().Int32Value();
	int height = info[3].As<Napi::Number>().Int32Value();

	MoveWindow(this->windowHandle, x, y, width, height, TRUE);
}

void Window::MoveRelative(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();

	if (info.Length() < 4 || !info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsNumber() || !info[3].IsNumber()) {
		throw Napi::TypeError::New(env, "Expected four arguments: dx, dy, dw, dh");
	}

	RECT dim;
	GetWindowRect(this->windowHandle, &dim);

	int dx = info[0].As<Napi::Number>().Int32Value();
	int dy = info[1].As<Napi::Number>().Int32Value();
	int dw = info[2].As<Napi::Number>().Int32Value();
	int dh = info[3].As<Napi::Number>().Int32Value();

	int x = dim.left + dx;
	int y = dim.top + dy;
	int w = (dim.right - dim.left) + dw;
	int h = (dim.bottom - dim.top) + dh;

	MoveWindow(this->windowHandle, x, y, w, h, TRUE);
}

Napi::Value Window::Dimensions(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	RECT dim;
	GetWindowRect(this->windowHandle, &dim);

	Napi::Object result = Napi::Object::New(env);
	result.Set("left", Napi::Number::New(env, dim.left));
	result.Set("top", Napi::Number::New(env, dim.top));
	result.Set("right", Napi::Number::New(env, dim.right));
	result.Set("bottom", Napi::Number::New(env, dim.bottom));
	return result;
}
