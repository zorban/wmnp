#ifndef NATIVE_EXTENSION_GRAB_W
#define NATIVE_EXTENSION_GRAB_W

#include <napi.h>
#include <windows.h>

class Window : public Napi::ObjectWrap<Window> {
public:
	Window(const Napi::CallbackInfo& info);
	~Window() = default;

	static Napi::Function GetConstructor(Napi::Env env);

	static Napi::Value GetActiveWindow(const Napi::CallbackInfo& info);
	static Napi::Value GetWindowByClassName(const Napi::CallbackInfo& info);
	static Napi::Value GetWindowByTitle(const Napi::CallbackInfo& info);
	static void EnumerateWindows(const Napi::CallbackInfo& info);

	Napi::Value Exists(const Napi::CallbackInfo& info);
	Napi::Value IsVisible(const Napi::CallbackInfo& info);
	Napi::Value GetTitle(const Napi::CallbackInfo& info);
	Napi::Value GetHwnd(const Napi::CallbackInfo& info);
	Napi::Value GetClassName(const Napi::CallbackInfo& info);
	Napi::Value GetPid(const Napi::CallbackInfo& info);
	Napi::Value GetParentHwnd(const Napi::CallbackInfo& info);
	Napi::Value GetAncestor(const Napi::CallbackInfo& info);
	Napi::Value GetMonitor(const Napi::CallbackInfo& info);

	void SetForegroundWindow(const Napi::CallbackInfo& info);
	void SetWindowPos(const Napi::CallbackInfo& info);
	void ShowWindow(const Napi::CallbackInfo& info);
	void Move(const Napi::CallbackInfo& info);
	void MoveRelative(const Napi::CallbackInfo& info);
	Napi::Value Dimensions(const Napi::CallbackInfo& info);

private:
	HWND windowHandle;
};

#endif
