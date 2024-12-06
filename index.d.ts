declare module "wmnp" {
	export function getActiveWindow(): Window;
	export function enumerateWindows(callback: (window: Window) => void): void;
	export function getWindowByClassName(className: string): Window;
	export function getWindowByTitle(title: string): Window;

	export class Window {
		constructor(hwnd: number);

		exists(): boolean;
		isVisible(): boolean;
		getTitle(): string;
		getHwnd(): number;
		getClassName(): string;
		getPid(): number;
		getParentHwnd(): number | null;
		getAncestor(gaFlags: number): number | null;
		getMonitor(): {
			name: string;
			primary: boolean;
			dimensions: {
				left: number;
				top: number;
				right: number;
				bottom: number;
			};
		};
		setForegroundWindow(): void;
		setWindowPos(hwndInsertAfter: number, x: number, y: number, cx: number, cy: number, uFlags: number): void;
		showWindow(nCmdShow: number): void;
		move(x: number, y: number, width: number, height: number): void;
		moveRelative(dx: number, dy: number, dw: number, dh: number): void;
		dimensions(): {
			left: number;
			top: number;
			right: number;
			bottom: number;
		};
	}
}
