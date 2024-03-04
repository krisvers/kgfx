package com.krisvers.kgfx;

public class KGFX {
	static {
		System.loadLibrary("kgfx");
		System.loadLibrary("kgfx_java");
	}

	public static native KGFXcontext createContext(int major, int minor, int patch, KGFXwindow window);
}