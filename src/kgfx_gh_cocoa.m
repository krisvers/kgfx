#include "kgfx_gh.h"
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	KGFXwindow win;
	win.window = glfwGetCocoaWindow(window);
	win.contentView = [CAMetalLayer layer];
	return win;
}
