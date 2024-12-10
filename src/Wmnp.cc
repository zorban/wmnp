#include "WmnpWindow.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
	Window::GetConstructor(env);

	exports.Set(
		Napi::String::New(env, "Window"),
		Window::GetConstructor(env)
	);

	exports.Set(
		Napi::String::New(env, "getActiveWindow"),
		Napi::Function::New(env, Window::GetActiveWindow)
	);

	exports.Set(
		Napi::String::New(env, "getWindowByClassName"),
		Napi::Function::New(env, Window::GetWindowByClassName)
	);

	exports.Set(
		Napi::String::New(env, "getWindowByTitle"),
		Napi::Function::New(env, Window::GetWindowByTitle)
	);

	exports.Set(
		Napi::String::New(env, "enumerateWindows"),
		Napi::Function::New(env, Window::EnumerateWindows)
	);
	
	return exports;
}

NODE_API_MODULE(winctl, InitAll)
