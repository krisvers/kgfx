package com.krisvers.kgfxgh;
import com.krisvers.kgfx.KGFXwindow;
import org.lwjgl.glfw.GLFWNativeCocoa;
import org.lwjgl.glfw.GLFWNativeWin32;
import org.lwjgl.glfw.GLFWNativeX11;

public class KGFXGHjni {
	private static boolean loaded = false;

	public static KGFXwindow kgfxWindowFromGLFW(long window) {
		/* get platform */
		String os = System.getProperty("os.name").toLowerCase();
		KGFXwindow win = new KGFXwindow();
		if (os.contains("win")) {
			win.window = GLFWNativeWin32.glfwGetWin32Window(window);
		} else if (os.contains("nix") || os.contains("nux")) {
			win.window = GLFWNativeX11.glfwGetX11Window(window);
		} else if (os.contains("mac")) {
			if (!loaded) {
				System.loadLibrary("kgfx_gh_java");
				loaded = true;
			}

			win.window = GLFWNativeCocoa.glfwGetCocoaMonitor(window);
			kgfxWindowSetupCocoa(win);
		} else {
			throw new RuntimeException("Unsupported platform");
		}

		return win;
	}
	public static native void kgfxWindowSetupCocoa(KGFXwindow window);
}
