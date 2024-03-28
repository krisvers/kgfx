#include <kgfx_gh/kgfx_gh.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	@autoreleasepool {
		KGFXwindow win;
		win.window = glfwGetCocoaWindow(window);
		return win;
	}
}

void kgfxWindowFromGLFWToPointer(GLFWwindow* window, KGFXwindow* outWindow) {
	if (outWindow == NULL) {
		return;	
	}
	KGFXwindow w = kgfxWindowFromGLFW(window);
	*outWindow = w;
}
