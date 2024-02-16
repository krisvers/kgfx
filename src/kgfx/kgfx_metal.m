#include "kgfx.h"
#include <stdlib.h>
#include <stdio.h>

#import <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/QuartzCore.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#define KGFX_IMPL_VER KGFX_MAKE_VERSION(1, 0, 0)

@interface Metal : NSObject
@property (assign) id<MTLDevice> device;
@property (assign) NSWindow* window;
@property (assign) GLFWwindow* glfwWindow;
@property (assign) CAMetalLayer* layer;

- (int) initialize;
- (void) destroy;
@end

typedef struct KGFXcontext_t {
	u32 version;

	Metal* metal;
	GLFWwindow* window;
} KGFXcontext_t;

KGFXresult kgfxCreateContext(u32 version, KGFXcontext* context) {
	if (context == NULL) {
		return KGFX_NULL_OUTPUT_ARGUMENT;
	}

	if ((KGFX_MAJOR(version) > KGFX_MAJOR(KGFX_IMPL_VER) && KGFX_MAJOR(version) != KGFX_ANY_MAJOR) || (KGFX_MINOR(version) > KGFX_MINOR(KGFX_IMPL_VER) && KGFX_MINOR(version) != KGFX_ANY_MINOR) || (KGFX_PATCH(version) > KGFX_PATCH(KGFX_IMPL_VER) && KGFX_PATCH(version) != KGFX_ANY_PATCH)) {
		return KGFX_VERSION_NOT_SUPPORTED;
	}

	KGFXcontext ctx = malloc(sizeof(KGFXcontext_t));
	if (ctx == NULL) {
		return KGFX_GENERIC_ERROR;
	}
	ctx->version = (version == KGFX_ANY_VERSION) ? KGFX_IMPL_VER : version;
	if (!glfwInit()) {
		free(ctx);
		return KGFX_GENERIC_ERROR;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	ctx->window = glfwCreateWindow(800, 600, "kgfx", NULL, NULL);
	if (ctx->window == NULL) {
		glfwTerminate();
		free(ctx);
		return KGFX_GENERIC_ERROR;
	}

	ctx->metal = [Metal new];
	ctx->metal.glfwWindow = ctx->window;
	if ([ctx->metal initialize] != 0) {
		return KGFX_GENERIC_ERROR;
	}

	*context = ctx;
	return KGFX_SUCCESS;
}

void kgfxDestroyContext(KGFXcontext context) {
	if (context == NULL) {
		return;
	}

	[context->metal release];
	context->metal = nil;
	glfwDestroyWindow(context->window);
	glfwTerminate();
	free(context);
}

u8 kgfxGetImplementationVersion() {
	return KGFX_IMPL_VER;
}

@implementation Metal
- (int) initialize {
	_device = MTLCreateSystemDefaultDevice();
	_window = glfwGetCocoaWindow(_glfwWindow);
	_layer = [CAMetalLayer layer];
	[_layer device:_device];

	return 0;
}

- (void) destroy {
	[_device release];
	_device = nil;
}
@end
