#include <kgfx_gh/kgfx_gh.h>
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

KGFXwindow kgfxWindowFromGLFW(GLFWwindow* window) {
	@autoreleasepool {
		KGFXwindow win;
		NSWindow* nswin = glfwGetCocoaWindow(window);
		win.window = nswin;

		int w, h;
		glfwGetWindowSize(window, &w, &h);
		
		CGSize size;
		size.width = w;
		size.height = h;

		NSBundle* bundle = [NSBundle bundleWithPath:@"/System/Library/Frameworks/QuartzCore.framework"];
		CAMetalLayer* layer = [[bundle classNamed:@"CAMetalLayer"] layer];
		[layer setDrawableSize:size];
		win.contentView = [nswin contentView];
		win.layer = layer;
		
		return win;
	}
}
