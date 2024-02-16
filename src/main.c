#include "kgfx/kgfx.h"

int main(int argc, char** argv) { 
	KGFXcontext ctx;
	if (kgfxCreateContext(KGFX_ANY_VERSION, &ctx) != KGFX_SUCCESS) {
		return 1;
	}

	kgfxDestroyContext(ctx);
	return 0;
}