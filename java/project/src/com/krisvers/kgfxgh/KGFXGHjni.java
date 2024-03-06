package com.krisvers.kgfxgh;
import com.krisvers.kgfx.KGFXwindow;

public class KGFXGHjni {
	static {
		System.loadLibrary("kgfx_gh");
		System.loadLibrary("kgfx_gh_java");
	}

	public static native KGFXwindow kgfxWindowFromGLFW(long window);
}
